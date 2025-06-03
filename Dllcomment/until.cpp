#include "StdAfx.h"
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <Wtsapi32.h>
#include <wininet.h>
#include <stdio.h>
#pragma comment(lib, "Wtsapi32.lib")
#include "until.h"

DWORD WINAPI ThreadLoader(LPVOID lparam)
{
	unsigned int	nRet = 0;

 	try
 	{
		THREAD_ARGLIST	arg;
		memcpy(&arg, lparam, sizeof(arg));
		SetEvent(arg.hEventTransferArg);
		// 与卓面交互
		if (arg.bInteractive)
			SelectDesktop(NULL);

		nRet = arg.start_address(arg.arglist);

	}catch(...){};

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
	arg.hEventTransferArg = CreateEvent(NULL, false, false, NULL);
	hThread = CreateThread(lpThreadAttributes, dwStackSize, ThreadLoader, &arg, dwCreationFlags, lpThreadId);
	WaitForSingleObject(arg.hEventTransferArg, INFINITE);
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
	char	*lpUserName = NULL;
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
		
		DWORD	nNameLen = 256;
		lpUserName = new char[256];
		
		fResult = LookupAccountSid(NULL, pTokenUser->User.Sid, lpUserName, &nNameLen,
			szDomainName, &dwDmLen, &sn);
	}
	__finally
	{
		if (hProc)
			::CloseHandle(hProc);
		if (hToken)
			::CloseHandle(hToken);
		if (pTokenUser)
			delete[] (char*)pTokenUser;
		
		return lpUserName;
	}
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

BOOL SelectHDESK(HDESK new_desktop)
{
	HDESK old_desktop = GetThreadDesktop(GetCurrentThreadId());
	
	DWORD dummy;
	char new_name[256];
	
	if (!GetUserObjectInformation(new_desktop, UOI_NAME, &new_name, 256, &dummy)) {
		return FALSE;
	}
	
	// Switch the desktop
	if(!SetThreadDesktop(new_desktop)) {
		return FALSE;
	}
	
	// Switched successfully - destroy the old desktop
	CloseDesktop(old_desktop);
	
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
		// Attempt to open the named desktop
		desktop = OpenDesktop(name, 0, FALSE,
			DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW |
			DESKTOP_ENUMERATE | DESKTOP_HOOKCONTROL |
			DESKTOP_WRITEOBJECTS | DESKTOP_READOBJECTS |
			DESKTOP_SWITCHDESKTOP | GENERIC_WRITE);
	}
	else
	{
		// No, so open the input desktop
		desktop = OpenInputDesktop(0, FALSE,
			DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW |
			DESKTOP_ENUMERATE | DESKTOP_HOOKCONTROL |
			DESKTOP_WRITEOBJECTS | DESKTOP_READOBJECTS |
			DESKTOP_SWITCHDESKTOP | GENERIC_WRITE);
	}
	
	// Did we succeed?
	if (desktop == NULL) {
		return FALSE;
	}
	
	// Switch to the new desktop
	if (!SelectHDESK(desktop)) {
		// Failed to enter the new desktop, so free it!
		CloseDesktop(desktop);
		return FALSE;
	}
	
	// We successfully switched desktops!
	return TRUE;
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