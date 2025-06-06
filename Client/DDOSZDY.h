#if !defined(AFX_DDOSZDY_H__C62BA182_2CA3_4C62_958B_4EC2D3A5024B__INCLUDED_)
#define AFX_DDOSZDY_H__C62BA182_2CA3_4C62_958B_4EC2D3A5024B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DDOSZDY.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDDOSZDY form view
#include "control\BtnST.h"
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDDOSZDY : public CFormView
{
public:
	CDDOSZDY();           // protected constructor used by dynamic creation
	void ConvertHexToData(const CString &sHex);
	DECLARE_DYNCREATE(CDDOSZDY)

// Form Data
public:
	bool isupdatedata;
	//{{AFX_DATA(CDDOSZDY)
	enum { IDD = IDD_DDOSZDY };
	CButtonST	m_Stop1;
	CButtonST	m_Start1;
	CButtonST	m_Stop;
	CButtonST	m_Start;
	CString	m_zdy;
	CString	m_addr;
	CString	m_addr2;
	int		m_port;
	int		m_port2;
	int		m_s1;
	int		m_s2;
	int		m_thread1;
	int		m_thread2;
	int		m_time;
	int		m_time2;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDDOSZDY)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDDOSZDY();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDDOSZDY)
	afx_msg void OnButtonBegin();
	afx_msg void OnButtonBegin2();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonStop2();
	afx_msg void OnRadioAnsi();
	afx_msg void OnRadioHex();
	afx_msg void OnRadioUdp();
	afx_msg void OnRadioTcp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DDOSZDY_H__C62BA182_2CA3_4C62_958B_4EC2D3A5024B__INCLUDED_)
