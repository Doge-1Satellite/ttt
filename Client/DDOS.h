#if !defined(AFX_DDOS_H__F689AECB_5309_4640_8B65_BB7D10C6E440__INCLUDED_)
#define AFX_DDOS_H__F689AECB_5309_4640_8B65_BB7D10C6E440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DDOS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDDOS form view
#include "control\BtnST.h"
#include "control\WinXPButtonST.h"
#ifndef __AFXEXT_H__
#include <afxext.h>


#endif

class CDDOS : public CFormView
{
public:
	CDDOS();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDDOS)

// Form Data
public:
	bool isupdatedata;
	bool isupdatedata1;
	//{{AFX_DATA(CDDOS)
	enum { IDD = IDD_DDOS };

	CComboBox	m_ms1;
	CListCtrl	m_atklist;
	CComboBox	m_ms;
	BOOL	m_auto;
	CString	m_addr;
	CString	m_port;
	CString	m_thread;
	CString	m_time;
	CString	m_time1;
	CString	m_thread1;
	CString	m_port1;
	CString	m_addr1;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDDOS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDDOS();

#ifdef _DEBUG

	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDDOS)
	afx_msg void OnSelchangeComboMs();
	afx_msg void OnButtonBegin();
	afx_msg void OnButtonStop();
	afx_msg void OnRadio11();
	afx_msg void OnRadio12();
	afx_msg void OnButton1();
	afx_msg void OnRadio13();
	afx_msg void OnRadio14();
	afx_msg void OnRadio15();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnSelchangeComboMs1();
	afx_msg void OnButtonBegin1();
	afx_msg void OnButtonStop1();
	afx_msg void OnRadio16();
	afx_msg void OnRadio17();
	afx_msg void OnRadio18();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
		CButtonST m_Button1;
		CButtonST m_Delete;
		CButtonST m_Add;
		CButtonST	m_Stop1;
		CButtonST	m_Start1;
		CButtonST	m_Stop;
    	CButtonST	m_Start;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DDOS_H__F689AECB_5309_4640_8B65_BB7D10C6E440__INCLUDED_)
