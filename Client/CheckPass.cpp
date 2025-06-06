// CheckPass.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "CheckPass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckPass dialog


CCheckPass::CCheckPass(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckPass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckPass)
	m_pass = _T("");
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CCheckPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckPass)
	DDX_Control(pDX, IDC_DISAGREE, m_disagree);
	DDX_Control(pDX, IDC_AGREE, m_agree);
	DDX_Text(pDX, IDC_EDIT3, m_pass);
	DDX_Text(pDX, IDC_SHOW, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckPass, CDialog)
	//{{AFX_MSG_MAP(CCheckPass)
	ON_BN_CLICKED(IDCANCEL, OnExit)
	ON_WM_LBUTTONDOWN()
	ON_EN_SETFOCUS(IDC_EDIT2, OnSetfocusEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBuy)
	ON_BN_CLICKED(IDC_AGREE, OnAgree)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_DISAGREE, OnDisagree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckPass message handlers

static char basea64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64_encodea(const void *data, int size, char **str)
{
  char *s, *p;
  int i;
  int c;
  const unsigned char *q;

  p = s = (char*)malloc(size*4/3+4);
  if (p == NULL)
      return -1;
  q = (const unsigned char*)data;
  i=0;
  for(i = 0; i < size;){
    c=q[i++];
    c*=256;
    if(i < size)
      c+=q[i];
    i++;
    c*=256;
    if(i < size)
      c+=q[i];
    i++;
    p[0]=basea64[(c&0x00fc0000) >> 18];
    p[1]=basea64[(c&0x0003f000) >> 12];
    p[2]=basea64[(c&0x00000fc0) >> 6];
    p[3]=basea64[(c&0x0000003f) >> 0];
    if(i > size)
      p[3]='=';
    if(i > size+1)
      p[2]='=';
    p+=4;
  }
  *p=0;
  *str = s;
  return strlen(s);
}

char* MyEncodea(char *str)
{
	int		i, len;
	char	*p;
	char	*s, *data;
	len = strlen(str) + 1;
	s = (char *)malloc(len);
	memcpy(s, str, len);
	for (i = 0; i < len; i++)
	{
		s[i] ^= 0x19;
		s[i] += 0x86;
	}
	base64_encodea(s, len, &data);
	free(s);
	return data;
}


int CCheckPass::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	////////////////////////////////////////////
	char SysPath[MAX_PATH];
	GetSystemDirectory(SysPath,sizeof(SysPath));
	strcat(SysPath,"\\SkinPPWTL.dll");
	CopyFile("SkinPPWTL.dll", SysPath,false);
	TCHAR szTempPath[_MAX_PATH];
	GetTempPath(_MAX_PATH, szTempPath);
	CString strSkinName = szTempPath;
	strSkinName += _T("\\Longhorn Silver.ssk"); //Ƥ����
	HRSRC hExeFile = ::FindResource(NULL,"IDR_Longhorn_Silver",_T("SSK")); 
	HGLOBAL hRes = ::LoadResource(NULL, hExeFile); 
	DWORD dwSize = ::SizeofResource(NULL, hExeFile); 
	if (hRes != NULL) 
	{ 
		UINT FAR* lpnRes = (UINT FAR*)::LockResource(hRes); 

		TRY 
		{ 
			CFile file(strSkinName, CFile::modeCreate | CFile::modeWrite);
			file.WriteHuge(lpnRes, dwSize); 
			file.Flush(); 
		} 
		CATCH(CFileException, e){} 
		END_CATCH 
		::FreeResource(hRes); 
	}
	DeleteFile(strSkinName);
	////////////////////////////////////////////	
	return 0;
}
char *username()	// GetVolumeInformation + GetComputerName
{
	LPCTSTR   new_name = "hbturkey";
	LPTSTR    pcname   = new   char[256];
	LPDWORD   nSize    = new   unsigned   long(256);
	BOOL   nRet=GetComputerName(pcname,nSize);

	CString strValue       = _T("");
	//LPTSTR   lpBuffer=new   char[256];
	//������������к�   
	LPCTSTR   lpRootPathName   =   "C:\\";   
	LPTSTR   lpVolumeNameBuffer=new   char[12];   
	DWORD   nVolumeNameSize=12;   
	DWORD   VolumeSerialNumber;   
	DWORD   MaximumComponentLength;   
	DWORD   FileSystemFlags;   
	LPTSTR   lpFileSystemNameBuffer=new   char[10];   
	DWORD   nFileSystemNameSize=10;   
	GetVolumeInformation(lpRootPathName,lpVolumeNameBuffer,nVolumeNameSize,&VolumeSerialNumber,     
						&MaximumComponentLength,&FileSystemFlags,lpFileSystemNameBuffer,nFileSystemNameSize);   
	//��ʾ���������к�   
	CString   str;   
	//str.Format("������%s�����к�Ϊ%x",lpRootPathName,VolumeSerialNumber);
	str.Format("%x",VolumeSerialNumber);
    char *lpBuffer = str.GetBuffer(str.GetLength() + 1);
    str.ReleaseBuffer();
	//::MessageBox(0,lpBuffer,"GetVolumeInformation",MB_ICONINFORMATION);
	//::MessageBox(0,pcname,"GetComputerName",MB_ICONINFORMATION);
	strcat(lpBuffer, pcname);
	//::MessageBox(0,lpBuffer,"GetVolumeInformation + GetComputerName",MB_ICONINFORMATION);
	return lpBuffer;
}

void DoXOR1(DWORD key, char *data, int len)
{
	while(len--)
		*(data++) ^= key;
}


void EncryptRecord1(char *szRec, unsigned long nLen, char *szKey)	/*���ܺ���*/ 
{
	unsigned long i;
	char *p;
	p = szKey;
	for(i = 0; i < nLen; i++)
	{
	if(!(*p))
		p = szKey;
	*szRec ^= *p;
	*szRec += *p;
	szRec++;
	p++;
	}
}


void rebootmeA()//����������
{
	PROCESS_INFORMATION   info;
	STARTUPINFO startup;
	char szPath[128];
	char *szCmdLine;

	GetModuleFileName(AfxGetApp()->m_hInstance,   szPath,   sizeof(szPath));
	szCmdLine   =   GetCommandLine();
	GetStartupInfo(&startup);
	BOOL   bSucc   =   CreateProcess(szPath,   szCmdLine,   NULL,   NULL,
	FALSE,   NORMAL_PRIORITY_CLASS,   NULL,   NULL, &startup,   &info);

	if(TRUE   &&   bSucc)
	{
		CWnd *pWnd   =   AfxGetMainWnd();
    
	if(pWnd   !=   NULL)
	{
		pWnd->PostMessage(WM_CLOSE,   0,   0);
    }   
    else
    ExitProcess(-1);
   }   
   else   
   ExitProcess(-1);
}

void CCheckPass::OnOK() //(FuckTheCreaker)
{
    CCheckPass dlg;
	this->UpdateData(true);

	char strNAME[9] = {'U','S','E','R','N','A','M','E','\0'}; //����һ��10���ַ�������,��������ַ������ƽ�
	char strPASS[9] = {'P','A','S','S','W','O','R','D','\0'};
	char stradd[8]  = {'E','n','c','r','y','p','t','\0'};
	//::MessageBox(0,strPASS,"ok", 0);//��MessageBox����ʾ������������ص��ַ���
	((CClientApp *)AfxGetApp())->m_IniFile.SetString(strNAME, strPASS, m_pass);
	if 	(((CClientApp *)AfxGetApp())->m_IniFile.GetString(strNAME, strPASS, "") == "")
		return;

	char pass[200];
	memset(pass,0,sizeof(pass));
	char *lpBuffer = NULL;
	wsprintf(pass,"%s%s",pass,username());
	//::MessageBox(0,pass,"GetVolumeInformation + GetComputerName",MB_ICONINFORMATION);
	wsprintf(pass,"%s%s",pass,stradd);
	//::MessageBox(0,pass,"GetVolumeInformation + GetComputerName + Torrent",MB_ICONINFORMATION);
	EncryptRecord1((char*)&pass,strlen(pass)-1,"15");
	//::MessageBox(0,pass,"Encrypt_1", 0);
	lpBuffer = MyEncodea(pass);
	//::MessageBox(0,lpBuffer,"Encrypt_2", 0);
	//GetDlgItem(IDC_EDIT3)->SetWindowText(pass);

	if (m_pass != lpBuffer)
	{
		//MessageBox("��������Ȩ�û������������롣","����",MB_ICONINFORMATION);		//ȥ����䣬���ƽ�
		m_pass="";
		return;
		this->UpdateData(false);
	}
	else
	{
		rebootmeA();
		ExitProcess(0);
		//this->EndDialog(true);  //�ɹ���¼�����ٶԻ���
	}
	CDialog::OnOK();
}

void CCheckPass::OnExit()
{
	ExitProcess(0);
	CDialog::OnCancel();	
}


void CCheckPass::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//this->UpdateData(true);
	GetDlgItem(IDC_EDIT2)->SetWindowText(username());		
	CDialog::OnLButtonDown(nFlags, point);
}

void CCheckPass::OnSetfocusEdit1() 
{
	//this->UpdateData(true);
	GetDlgItem(IDC_EDIT2)->SetWindowText(username());
}

void CCheckPass::OnBuy() 
{
	ShellExecute(0, NULL,"tencent://message/?uin=550067654&Site=550067654&Menu=yes",NULL,NULL, SW_SHOW);
}

void CCheckPass::OnAgree() 
{
	m_agree.ShowWindow(SW_HIDE);		//����   
	m_disagree.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SHOW)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_USER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ME)->ShowWindow(SW_SHOW);
	//::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC), SW_HIDE);
}



void CCheckPass::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	SetDlgItemText(IDC_SHOW,
"\t\t\t�û�ʹ��Э��:\r\n\r\n"
"\t��Զ�̿��������������ʹ�ã��Ͻ����ڷǷ���;��\r\n"
"�κθ��ˡ����塢��֯���ý������ڷǷ�Ŀ�ģ���������\r\n"
"�����û�һ����װ�����ƻ���������ʽʹ�ñ������Ʒ����\r\n"
"��ʾͬ�����Э�����������Լ��������û���ͬ��Э���\r\n"
"�������벻Ҫʹ�ñ������Ʒ��\r\n\r\n"
"\t\t\t�û�ʹ����֪:\r\n\r\n"
"\t�ر������û���ʹ�û������������ع����йص����ߺͷ��ɣ�\r\n"
"���̷������Ұ�ȫ�������ܷ����������Ϣϵͳ��ȫ��������\r\n"
"�ȣ������������棬�������Ұ�ȫ������Υ��ʹ�û��������\r\n"
"�����һ�����Σ����û���ȫ�����Ρ�\r\n"
);
	GetDlgItem(IDC_STATIC_USER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ME)->ShowWindow(SW_HIDE);
}

void CCheckPass::OnDisagree() 
{
	OnExit();
}

HBRUSH CCheckPass::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	/*
HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
if(CTLCOLOR_EDIT == nCtlColor) //EDIT�ؼ�
{
	pDC->SetTextColor(RGB(255,0,0)); 
}
*/
/*
HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
switch(pWnd->GetDlgCtrlID())
{     
	case   IDC_SHOW:     
	pDC->SetTextColor(RGB(200,100,128));   //�޸�������ɫ     
}
*/

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor); 
	
	switch(nCtlColor) 
	{ 
	case CTLCOLOR_EDIT: 
		if(pWnd->GetDlgCtrlID() == IDC_SHOW) 
		{ 
			static HBRUSH hbrEdit = ::CreateSolidBrush( RGB(0, 255, 0) ); 
			pDC->SetBkColor( RGB(255, 0, 0) ); 
			pDC->SetTextColor( RGB(0, 255, 0) ); 
			return hbrEdit; 
		} 
	} 
	return   hbr;
}


BOOL CCheckPass::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT2)->SetWindowText(username());		
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
