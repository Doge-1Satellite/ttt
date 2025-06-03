// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Setting.h"
#include "MainFrm.h"
#include "UpdateDlg.h"
#include "TabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUpdateDlg g_CUpdateDlg;
extern CTabView* g_pTabView;
CString prev_ComPass;
extern CMainFrame	*g_pFrame;
extern CIOCPServer	*m_iocpServer;
BOOL dawed_1,dawed_2,dawed_3,dawed_4,dawed_5,dawed_6,dawed_7,dawed_8,dawed_9,dawed_10,dawed_11,dawed_12,dawed_13,dawed_14,dawed_15,dawed_16,dawed_17,dawed_18,dawed_19;

/////////////////////////////////////////////////////////////////////////////
// CSetting dialog


CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting::IDD, pParent)
{
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SETTING));
	//{{AFX_DATA_INIT(CSetting)

	dawed_1 = m_dawed_1 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width01",TRUE);
	dawed_2 = m_dawed_2 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width02",TRUE);
	dawed_3 = m_dawed_3 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width03",TRUE);
	dawed_4 = m_dawed_4 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width04",FALSE);
	dawed_5 = m_dawed_5 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width05",TRUE);
	dawed_6 = m_dawed_6 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width06",TRUE);
	dawed_7 = m_dawed_7 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width07",TRUE);
	dawed_8 = m_dawed_8 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width08",TRUE);
	dawed_9 = m_dawed_9 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width09",FALSE);
	dawed_10 = m_dawed_10 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width10",TRUE);
	dawed_11 = m_dawed_11 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width11",TRUE);
	dawed_12 = m_dawed_12 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width12",FALSE);
	dawed_13 = m_dawed_13 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width13",FALSE);
	dawed_14 = m_dawed_14 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width14",TRUE);
	dawed_15 = m_dawed_15 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width15",TRUE);
	dawed_16 = m_dawed_16 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width16",FALSE);
	dawed_17 = m_dawed_17 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width17",TRUE);
	dawed_18 = m_dawed_18 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width18",TRUE);
	dawed_19 = m_dawed_19 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width19",FALSE);

	m_max_connections = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnection", 8000);
	m_connect_auto =((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnectionAuto", 1);
	m_bIsDisablePopTips =  ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "PopTips", false);
	m_listen_port = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 8080);
	m_bisDisableSound =  ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Sound", false);
	m_line  = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "gridlines", false);//网格线
	m_national  = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "national", true);//国旗
	m_color =  ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Color", 1);//列表颜色
	m_bisDisablePifu =  ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "SKIN", FALSE);//软件皮肤
	//}}AFX_DATA_INIT
}


void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting)
	DDX_Control(pDX, IDC_DISABLE_SOUND, m_bisDisableSound_XTP);
	DDX_Control(pDX, IDC_SHOWTIPS, m_showtips);
	DDX_Control(pDX, IDC_LISTEN_PORT, m_listen_port_XTP);
	DDX_Control(pDX, IDC_DISABLE_POPTIPS, m_disable_poptips_XTP);
	DDX_Control(pDX, IDC_CONNECT_MAX, m_connect_max_XTP);
	DDX_Control(pDX, IDC_CONNECT_AUTO, m_connect_auto_XTP);
	DDX_Text(pDX, IDC_CONNECT_MAX, m_max_connections);
	DDV_MinMaxUInt(pDX, m_max_connections, 1, 10000);
	DDX_Check(pDX, IDC_CONNECT_AUTO, m_connect_auto);
	DDX_Check(pDX, IDC_DISABLE_POPTIPS, m_bIsDisablePopTips);
	DDX_Text(pDX, IDC_LISTEN_PORT, m_listen_port);
	DDV_MinMaxUInt(pDX, m_listen_port, 1, 65535);
	DDX_Check(pDX, IDC_DISABLE_SOUND, m_bisDisableSound);
	DDX_Check(pDX, IDC_LINE, m_line);
	DDX_Check(pDX, IDC_NATIONAL, m_national);
	DDX_Control(pDX, IDC_COMBO_OPERATE, m_operateCmb);
	DDX_CBIndex(pDX, IDC_INSTALL_WAY, m_color);//列表颜色
	DDX_Check(pDX, IDC_DISABLE_Pifu, m_bisDisablePifu);//自定义皮肤
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_DAWED_1, m_dawed_1);
	DDX_Check(pDX, IDC_DAWED_2, m_dawed_2);
	DDX_Check(pDX, IDC_DAWED_3, m_dawed_3);
	DDX_Check(pDX, IDC_DAWED_4, m_dawed_4);
	DDX_Check(pDX, IDC_DAWED_5, m_dawed_5);
	DDX_Check(pDX, IDC_DAWED_6, m_dawed_6);
	DDX_Check(pDX, IDC_DAWED_7, m_dawed_7);
	DDX_Check(pDX, IDC_DAWED_8, m_dawed_8);
	DDX_Check(pDX, IDC_DAWED_9, m_dawed_9);
	DDX_Check(pDX, IDC_DAWED_10, m_dawed_10);
	DDX_Check(pDX, IDC_DAWED_11, m_dawed_11);
	DDX_Check(pDX, IDC_DAWED_12, m_dawed_12);
	DDX_Check(pDX, IDC_DAWED_13, m_dawed_13);
	DDX_Check(pDX, IDC_DAWED_14, m_dawed_14);
	DDX_Check(pDX, IDC_DAWED_15, m_dawed_15);
	DDX_Check(pDX, IDC_DAWED_16, m_dawed_16);
	DDX_Check(pDX, IDC_DAWED_17, m_dawed_17);
	DDX_Check(pDX, IDC_DAWED_18, m_dawed_18);
	DDX_Check(pDX, IDC_DAWED_19, m_dawed_19);

	DDX_Control(pDX, IDC_GROUP_STYLE, m_groupxtp);
	DDX_Control(pDX, IDC_GROUP_STYLE2, m_groupxtp2);
}


BEGIN_MESSAGE_MAP(CSetting, CDialog)
	//{{AFX_MSG_MAP(CSetting)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DISABLE_POPTIPS, OnDisablePoptips)
	ON_BN_CLICKED(IDC_CONNECT_AUTO, OnConnectAuto)
	ON_BN_CLICKED(IDC_DISABLE_SOUND, OnDisableSound)
	ON_BN_CLICKED(IDC_LINE, Online)
	ON_BN_CLICKED(IDC_NATIONAL, OnNational)
	ON_CBN_SELCHANGE(IDC_GROUP_STYLE, OnEditchangeGroupStyle)
	ON_CBN_SELCHANGE(IDC_GROUP_STYLE2, OnEditchangeGroupStyle2)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_CUSTOM_SKIN, OnCustomSkin)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_DISABLE_Pifu, OnDISABLEPifu)//自定义皮肤
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting message handlers

// void CSetting::rebootmeA()//重启本程序
// {
// 	PROCESS_INFORMATION   info;
// 	STARTUPINFO startup;
// 	char szPath[128];
// 	char *szCmdLine;
// 	
// 	GetModuleFileName(AfxGetApp()->m_hInstance,   szPath,   sizeof(szPath));
// 	szCmdLine   =   GetCommandLine();
// 	GetStartupInfo(&startup);
// 	BOOL   bSucc   =   CreateProcess(szPath,   szCmdLine,   NULL,   NULL,
// 		FALSE,   NORMAL_PRIORITY_CLASS,   NULL,   NULL, &startup,   &info);
// 	
// 	if(TRUE   &&   bSucc)
// 	{
// 		CWnd *pWnd   =   AfxGetMainWnd();
// 		
// 		if(pWnd   !=   NULL)
// 		{
// 			pWnd->PostMessage(WM_CLOSE,   0,   0);
// 		}   
// 		else
// 			ExitProcess(-1);
// 	}   
// 	else   
// 		ExitProcess(-1);
// }
void CSetting::OnApply() 
{
	// TODO: Add your control notification handler code here
	int prev_port = m_listen_port;
	int prev_max_connections = m_max_connections;
	
	UpdateData(TRUE);
	
	operate=m_operateCmb.GetCurSel();
	
	if (prev_max_connections != m_max_connections)
	{
		if (m_connect_auto)
			InterlockedExchange((LPLONG)&m_iocpServer->m_nMaxConnections, 8000);
		else
			InterlockedExchange((LPLONG)&m_iocpServer->m_nMaxConnections, m_max_connections);
	}
	
	CIniFile m_IniFile;//设置完成播放声音
	BOOL b_chk = m_IniFile.GetInt(_T("Settings"), _T("Sound"), false);
	
    if (b_chk != TRUE)
    {
		PlaySound(MAKEINTRESOURCE(IDR_WAVE3),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
	}
	
	if (prev_port != m_listen_port)
		g_pFrame->Activate(m_listen_port, m_iocpServer->m_nMaxConnections);
	
	((CClientApp *)AfxGetApp())->m_IniFile.SetDWORD("Settings","operate", operate);           //双击时的默认操作
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "ListenPort", m_listen_port);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnection", m_max_connections);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnectionAuto", m_connect_auto);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width01", m_dawed_1);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width02", m_dawed_2);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width03", m_dawed_3);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width04", m_dawed_4);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width05", m_dawed_5);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width06", m_dawed_6);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width07", m_dawed_7);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width08", m_dawed_8);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width09", m_dawed_9);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width10", m_dawed_10);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width11", m_dawed_11);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width12", m_dawed_12);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width13", m_dawed_13);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width14", m_dawed_14);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width15", m_dawed_15);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width16", m_dawed_16);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width17", m_dawed_17);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width18", m_dawed_18);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width19", m_dawed_19);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Color", m_color);
	((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "SKIN", m_bisDisablePifu);//软件皮肤
	
	SetDlgItemText(IDC_SHOWTIPS, "已保存设置,部分设置重启生效!");
	
	BOOL dawed_DATA = NULL;
	if((dawed_1 != m_dawed_1)||(dawed_2 != m_dawed_2)||(dawed_3 != m_dawed_3)||(dawed_4 != m_dawed_4)||(dawed_5 != m_dawed_5)||
		(dawed_6 != m_dawed_6)||(dawed_7 != m_dawed_7)||(dawed_8 != m_dawed_8)||(dawed_9 != m_dawed_9)||(dawed_10 != m_dawed_10)||
		(dawed_11 != m_dawed_11)||(dawed_12 != m_dawed_12)||(dawed_13 != m_dawed_13)||(dawed_14 != m_dawed_14)||(dawed_15 != m_dawed_15)
		||(dawed_16 != m_dawed_16)||(dawed_17 != m_dawed_17)||(dawed_18 != m_dawed_18)||(dawed_19 != m_dawed_19)
		)
	{
		dawed_DATA = TRUE;
	}
	
	if ((unsigned int)/*prev_port != m_listen_port)||(*/dawed_DATA == TRUE);  //比较端口是否改变
	{
		if(MessageBox("系统参数已经更改，需重启软件生效..."," 温馨提示！",MB_ICONQUESTION|MB_YESNO)==IDYES)
		{
			//	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "ListenPort", m_listen_port);
			//  		g_pFrame->m_bSetRestart = TRUE;
			// 
			//  		AfxGetMainWnd()->SendMessage(WM_CLOSE);
			//   
			/*rebootmeA();*/
			g_pFrame->m_bSetRestart = TRUE;
		    AfxGetMainWnd()->SendMessage(WM_CLOSE);
			ExitProcess(0);
		}
	}
}

void CSetting::OnDisablePoptips() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "PopTips", m_bIsDisablePopTips);
	((CClientApp *)AfxGetApp())->m_bIsDisablePopTips = m_bIsDisablePopTips;
	if ( ((CButton *)GetDlgItem(IDC_DISABLE_POPTIPS))->GetCheck() == TRUE )
	{
		SetDlgItemText(IDC_SHOWTIPS, "已关闭上线气泡显示");
	}
	else
	{
		SetDlgItemText(IDC_SHOWTIPS, "已开启上线气泡显示");
	}
}

void CSetting::OnConnectAuto() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(!m_connect_auto);
	m_connect_auto = ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnectionAuto", m_connect_auto);
	if ( ((CButton *)GetDlgItem(IDC_CONNECT_AUTO))->GetCheck() == TRUE )
	{
		SetDlgItemText(IDC_SHOWTIPS, "无限上线数量");
	}
	else
	{
		SetDlgItemText(IDC_SHOWTIPS, "有限上线数量，请自定义");
	}
}

void CSetting::OnDisableSound() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
//	((CClientApp *)AfxGetApp())->m_bisDisableSound = m_bisDisableSound;
	if ( ((CButton *)GetDlgItem(IDC_DISABLE_SOUND))->GetCheck() == TRUE )
	{
		SetDlgItemText(IDC_SHOWTIPS, "已关闭提示音");
	}
	else
	{
		SetDlgItemText(IDC_SHOWTIPS, "已开启提示音");
	}
	m_bisDisableSound=((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Sound", m_bisDisableSound);
}

BOOL CSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_connect_auto==TRUE)
		GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(TRUE);

	SetDlgItemText(IDC_SHOWTIPS, "远程管理\r\n");
	
	m_listen_port_XTP.SetTheme(xtpControlThemeOfficeXP);
	m_connect_max_XTP.SetTheme(xtpControlThemeOfficeXP);
	m_disable_poptips_XTP.SetTheme(xtpControlThemeOffice2003);
	m_bisDisableSound_XTP.SetTheme(xtpControlThemeOffice2003);
	m_connect_auto_XTP.SetTheme(xtpControlThemeOffice2003);
	
	m_listen_port_XTP.SetFlatStyle(TRUE);
	m_connect_max_XTP.SetFlatStyle(TRUE);
	m_disable_poptips_XTP.SetFlatStyle(TRUE);
	m_bisDisableSound_XTP.SetFlatStyle(TRUE);
	m_connect_auto_XTP.SetFlatStyle(TRUE);
	
	InitOperate();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetting::OnEditchangeGroupStyle() 
{
	// TODO: Add your control notification handler code here
//	::MessageBox(NULL,NULL,NULL,NULL);
	CString Group;
	GetDlgItemText(IDC_GROUP_STYLE,Group);

	if(Group=="SRAT")
	{
		g_pTabView->m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPageFlat);
     	g_pTabView->m_wndTabControl.GetPaintManager()->SetColor(xtpTabColorVisualStudio2005);	//标签颜色
		g_pTabView->m_wndTabControl.Reposition();
		((CClientApp *)AfxGetApp())->m_IniFile.SetString("Settings", "GroupStyle", "SRAT");
		SetDlgItemText(IDC_SHOWTIPS, "SRAT分组样式保存成功");
	}
	else if(Group=="GH0ST1.0")
	{
		g_pTabView->m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearanceExcel);
    	g_pTabView->m_wndTabControl.GetPaintManager()->SetColor(xtpTabColorVisualStudio2005);	//标签颜色
		g_pTabView->m_wndTabControl.Reposition();
		((CClientApp *)AfxGetApp())->m_IniFile.SetString("Settings", "GroupStyle", "GH0ST1.0");
		SetDlgItemText(IDC_SHOWTIPS, "GH0ST1.0分组样式保存成功");
	}
	else
	{
		g_pTabView->m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);
    	g_pTabView->m_wndTabControl.GetPaintManager()->SetColor(xtpTabColorVisualStudio2005);	//标签颜色
		g_pTabView->m_wndTabControl.Reposition();
		((CClientApp *)AfxGetApp())->m_IniFile.SetString("Settings", "GroupStyle", "PCSHARE");
		SetDlgItemText(IDC_SHOWTIPS, "PCSHARE分组样式保存成功");
	}
// 	CString	m_in ;
// 	::MessageBox(NULL,((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "GroupStyle", ""),NULL,NULL);

}

void CSetting::OnEditchangeGroupStyle2() 
{
	// TODO: Add your control notification handler code here
	CString Group;
	GetDlgItemText(IDC_GROUP_STYLE2,Group);
	
	if(Group=="上边")
	{
    	g_pTabView->m_wndTabControl.SetPosition(xtpTabPositionTop);
//		g_pTabView->m_wndTabControl.GetPaintManager()->SetColor(xtpTabColorOffice2003);	//标签颜色
		g_pTabView->m_wndTabControl.Reposition();
		((CClientApp *)AfxGetApp())->m_IniFile.SetString("Settings", "GroupPos", "Top");
	}
	else
	{
    	g_pTabView->m_wndTabControl.SetPosition(xtpTabPositionBottom);
//		g_pTabView->m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabPositionBottom);
//		g_pTabView->m_wndTabControl.GetPaintManager()->SetColor(xtpTabColorOffice2003);	//标签颜色
		g_pTabView->m_wndTabControl.Reposition();
		((CClientApp *)AfxGetApp())->m_IniFile.SetString("Settings", "GroupPos", "Bot");
	}
	/*
	xtpTabPositionTop,          // 上
	xtpTabPositionLeft,         // 左
	xtpTabPositionBottom,       // 下
	xtpTabPositionRight         // 右
	*/
	SetDlgItemText(IDC_SHOWTIPS, "分组位置保存成功");
}


//新添加
void CSetting::Online()
{
	UpdateData();
	m_line = ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "gridlines", m_line);
	SetDlgItemText(IDC_SHOWTIPS, "列表网格设置成功");
// 	if(MessageBox("系统参数已经更改，需重启软件生效..."," 温馨提示！",MB_ICONQUESTION|MB_YESNO)==IDYES)
// 	{
// 		g_pFrame->m_bSetRestart = TRUE;
// 		AfxGetMainWnd()->SendMessage(WM_CLOSE);
// 	}
}

void  CSetting::OnNational() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
// 	if(MessageBox("系统参数已经更改，需重启软件生效..."," 温馨提示！",MB_ICONQUESTION|MB_YESNO)==IDYES)
// 	{
	    ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "National", m_national);
	    SetDlgItemText(IDC_SHOWTIPS, "显示国旗图标设置成功");
// 		g_pFrame->m_bSetRestart = TRUE;
// 		AfxGetMainWnd()->SendMessage(WM_CLOSE);
// 	}
}



void CSetting::OnCustomSkin() //载入皮肤
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strCurrentPath;
	GetCurrentDirectory(200,strCurrentPath.GetBuffer(200)); //获取当前路径
	strCurrentPath.ReleaseBuffer();
    char szFilters[]= "skins (*.she)|*.she|All Files (*.*)|*.*||";
    CFileDialog fileDlg (TRUE, "*", "", OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	fileDlg.m_ofn.lpstrInitialDir = strCurrentPath; //初始化当前路径
	if( IDOK == fileDlg.DoModal() )
	{
		((CClientApp *)AfxGetApp())->m_IniFile.SetString("Settings", "Skin", fileDlg.GetPathName());   
		CString fileName = fileDlg.GetFileName();
		::SkinH_AttachEx(fileName, NULL);
		SetDlgItemText(IDC_SKIN_PATH, fileDlg.GetPathName());
	} 
}


void CSetting::OnExit() 
{
	// TODO: Add your control notification handler code here
	HWND King = ::FindWindow(NULL,_T("系统设置"));
	::PostMessage(King,WM_CLOSE,1,0);
}

// 初始化双击时执行的操作
void CSetting::InitOperate(void)
{
	CString str;
	str.LoadString(IDM_FILEMANAGER);  //="文件管理";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,0);
	str.LoadString(IDM_KEYBOARD);     //="键盘记录";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,1);
	str.LoadString(IDM_SCREENSPY);    //="远程桌面";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,2);
	str.LoadString(IDM_SYSTEM);       //="系统管理";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,3);
	str.LoadString(IDM_WEBCAM);       //="视频查看";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,4);
	str.LoadString(IDM_AUDIO_LISTEN); //="语音监听";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,5);
	str.LoadString(IDM_REMOTESHELL);  //="远程终端";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,6);
	str.LoadString(IDM_SYSINFO);      //="主机管理";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,7);
	str.LoadString(IDM_SERMANAGER);   //="服务管理";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,8);
	str.LoadString(IDM_REGEDIT);      //="查注册表";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,9);
	str.LoadString(IDM_CHAT);         //="远程交谈";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,10);
	str.LoadString(IDM_MAP);          //="远程定位";
	m_operateCmb.AddString(str);
	m_operateCmb.SetItemData(0,11);
	operate = ((CClientApp *)AfxGetApp())->m_IniFile.GetDWORD("Settings", "operate", 3);
	if (operate<0 || operate>=12)
    {
		 operate = 3;
	}
	m_operateCmb.SetCurSel(operate);
}

HBRUSH CSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if ((pWnd->GetDlgCtrlID() == IDC_SHOWTIPS) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(255, 0, 0);
		pDC->SetTextColor(clr);   //设置白色的文本
		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);     //设置黑色的背景
		return CreateSolidBrush(clr);  //作为约定，返回背景色对应的刷子句柄
	}
	else
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CSetting::OnDISABLEPifu() //显示软件皮肤
{	
	UpdateData(true);
	if ( ((CButton *)GetDlgItem(IDC_DISABLE_Pifu))->GetCheck() == TRUE )
	{
		char Path[200]={0};
		GetModuleFileName(NULL,Path,200);
		*strrchr(Path,'\\')=0;
		strcat(Path,"\\Skins\\SkinH.she");
		SkinH_AttachEx(Path,NULL);
		SetDlgItemText(IDC_SHOWTIPS, "开启软件皮肤成功");
		
	}
	else
	{
		SkinH_Detach();//关闭皮肤效果
		SetDlgItemText(IDC_SHOWTIPS, "已关闭软件皮肤");
	}
	m_bisDisablePifu=((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "SKIN", m_bisDisablePifu);
}
