#if !defined(AFX_CHECKPASS_H__BF4879F1_B1B3_4003_B9C6_C271286FC693__INCLUDED_)
#define AFX_CHECKPASS_H__BF4879F1_B1B3_4003_B9C6_C271286FC693__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckPass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckPass dialog

class CCheckPass : public CDialog
{
// Construction
public:
	CCheckPass(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCheckPass)
	enum { IDD = IDD_CHECK_PASS };
	CButton	m_disagree;
	CButton	m_agree;
	CString	m_pass;
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckPass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCheckPass)
	virtual void OnOK();
	afx_msg void OnExit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetfocusEdit1();
	afx_msg void OnBuy();
	afx_msg void OnAgree();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDisagree();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKPASS_H__BF4879F1_B1B3_4003_B9C6_C271286FC693__INCLUDED_)
