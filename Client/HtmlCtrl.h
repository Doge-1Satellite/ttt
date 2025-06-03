#if !defined(AFX_HTMLCTRL_H__C354493E_9E6F_44B0_B515_2E7DA381BCE1__INCLUDED_)
#define AFX_HTMLCTRL_H__C354493E_9E6F_44B0_B515_2E7DA381BCE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlCtrl html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#ifndef __AFXHTML_H__
#include <afxhtml.h>
#endif
#ifndef __mshtml_h__
#include <Mshtml.h>
#endif

class CMapDlg;

class CHtmlCtrl : public CHtmlView
{
public:
	CHtmlCtrl();           // protected constructor used by dynamic creation
	virtual ~CHtmlCtrl();
	DECLARE_DYNCREATE(CHtmlCtrl)

// html Data
public:
	//{{AFX_DATA(CHtmlCtrl)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CMapDlg* m_pMapDlg;
	BOOL CreateFromStatic(UINT nID, CWnd* pParent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlCtrl)
	public:
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnAppCmd(LPCTSTR lpszWhere);
	protected:
	//virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHtmlCtrl)
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	/*
	// Generated dispatch map functions
	//{{AFX_DISPATCH(CMyHtmlView)
	afx_msg BSTR GetLng();
	afx_msg void SetLng(BSTR newValue);
	afx_msg BSTR GetLat();
	afx_msg void SetLat(BSTR newValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLCTRL_H__C354493E_9E6F_44B0_B515_2E7DA381BCE1__INCLUDED_)
