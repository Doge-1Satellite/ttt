// LOCKDlg.h : header file
//

#if !defined(AFX_LOCKDLG_H__C2951A1B_246B_4FD3_8C68_DFCA2BE8C8A4__INCLUDED_)
#define AFX_LOCKDLG_H__C2951A1B_246B_4FD3_8C68_DFCA2BE8C8A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct PIDWNDINFO
{
	DWORD dwProcessId;
	HWND hWnd;
};

/////////////////////////////////////////////////////////////////////////////
// CLOCKDlg dialog

class CLockDlg : public CDialog
{
// Construction
public:
	CLockDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLOCKDlg)
	enum { IDD = IDD_LOCK};
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLOCKDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CLOCKDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetlock();
	afx_msg void OnQuit();
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCKDLG_H__C2951A1B_246B_4FD3_8C68_DFCA2BE8C8A4__INCLUDED_)
