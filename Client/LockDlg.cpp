// LOCKDlg.cpp : implementation file
// Download by http://www.codefans.net

#include "stdafx.h"
#include "Client.h"
//#include "ClientDlg.h"
#include "LockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBrush m_bkbrush;

int user=0;

CString PASSWORD1;
CString	PASSWORD2;
CString PASSWORD3;

extern BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CLOCKDlg dialog

CLockDlg::CLockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLOCKDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLOCKDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLockDlg, CDialog)
	//{{AFX_MSG_MAP(CLOCKDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(ID_SETLOCK, OnSetlock)
	ON_BN_CLICKED(ID_QUIT, OnQuit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLOCKDlg message handlers

BOOL CLockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
/*	m_bkbrush.CreateSolidBrush(RGB(0,0,0));*/
	
	SendDlgItemMessage(IDC_UNLOCK,EM_SETREADONLY,1); 
	
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLockDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLockDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLockDlg::OnSetlock() 
{
    if(user==0)
	{
	    GetDlgItemText(IDC_SET,PASSWORD1);
	    GetDlgItemText(IDC_SETAGAIN,PASSWORD2);
	    if(PASSWORD1!=PASSWORD2)
		{
			SetDlgItemText(IDC_MESSAGE,"两次的密码不相同\n请重新输入");
			SetDlgItemText(IDC_SET,"");
		    SetDlgItemText(IDC_SETAGAIN,"");
			GetDlgItem(IDC_SET)->SetFocus();
		    return;
		}
		if(PASSWORD1=="")
		{
			SetDlgItemText(IDC_MESSAGE,"密码设为空\n这样不太安全!");
			GetDlgItem(IDC_SET)->SetFocus();
			return;
		}
		SetDlgItemText(IDC_MESSAGE,"我先帮你看着\n快点回来.");
		
		SendDlgItemMessage(IDC_SET,EM_SETREADONLY,1);
		SendDlgItemMessage(IDC_SETAGAIN,EM_SETREADONLY,1);
		SendDlgItemMessage(IDC_UNLOCK,EM_SETREADONLY,0);
		SetDlgItemText(ID_SETLOCK,"解锁");
		SetDlgItemText(IDC_SET,"");
		SetDlgItemText(IDC_SETAGAIN,"");
		GetDlgItem(ID_QUIT)->EnableWindow(false);
		user=1;
		
		CRect rct;
		PIDWNDINFO pwi;
	    GetWindowRect(rct);
	    ClipCursor(rct);
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING,true,0,SPIF_UPDATEINIFILE);
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
		pwi.dwProcessId = 0;
		pwi.hWnd = GetSafeHwnd();
		EnumWindows(EnumWindowsProc, (LPARAM)&pwi);
		::ShowWindow(::FindWindow("Progman",NULL),SW_HIDE);
		::ShowWindow(::FindWindow("Button","开始"),SW_HIDE);
		::ShowWindow(::FindWindow("Shell_TrayWnd",NULL),SW_HIDE);
		
		GetDlgItem(IDC_UNLOCK)->SetFocus();
		
		return;
	}
	if(user==1)
	{
        GetDlgItemText(IDC_UNLOCK,PASSWORD3);
		if(PASSWORD1!=PASSWORD3)
		{
			SetDlgItemText(IDC_MESSAGE,"密码输入错误\n你无权解锁!");
			SetDlgItemText(IDC_UNLOCK,"");
			GetDlgItem(IDC_UNLOCK)->SetFocus();
			return;
		}
		SetDlgItemText(IDC_MESSAGE,"不用我帮忙了?");
		
		SendDlgItemMessage(IDC_SET,EM_SETREADONLY,0);
		SendDlgItemMessage(IDC_SETAGAIN,EM_SETREADONLY,0);
		SendDlgItemMessage(IDC_UNLOCK,EM_SETREADONLY,1);
		SetDlgItemText(ID_SETLOCK,"加锁");
		SetDlgItemText(IDC_UNLOCK,"");
		GetDlgItem(ID_QUIT)->EnableWindow(true);
		user=0;
		
		ClipCursor(NULL);
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING,false,0,SPIF_UPDATEINIFILE);
		::ShowWindow (::FindWindow("Shell_TrayWnd",NULL),SW_SHOW);
		::ShowWindow (::FindWindow("Button","开始"),SW_SHOW);
		::ShowWindow(::FindWindow("Progman",NULL),SW_SHOW);
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
	}
}

void CLockDlg::OnQuit() 
{
    OnOK();	
}

void CLockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE)
		return;
	
	CDialog::OnSysCommand(nID, lParam);
}

BOOL CLockDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg -> message == WM_KEYDOWN)
	{
        if(pMsg -> wParam == VK_ESCAPE)
			return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CLockDlg::OnClose()
{	
}
