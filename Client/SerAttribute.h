#if !defined(AFX_SERATTRIBUTE_H__DF556FBD_DE5A_4BCD_8A67_F947BE0DBDFC__INCLUDED_)
#define AFX_SERATTRIBUTE_H__DF556FBD_DE5A_4BCD_8A67_F947BE0DBDFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerAttribute.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSerAttribute dialog

class CSerAttribute : public CDialog
{
// Construction
public:
	CListCtrl* m_list_service;
	CSerAttribute(CWnd* pParent = NULL, CListCtrl* pService = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSerAttribute)
	enum { IDD = IDD_SERATTR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerAttribute)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSerAttribute)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnPrev();
	afx_msg void OnBtnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERATTRIBUTE_H__DF556FBD_DE5A_4BCD_8A67_F947BE0DBDFC__INCLUDED_)
