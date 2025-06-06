// Client.h : main header file for the Client application
//

#if !defined(AFX_Client_H__C0496689_B41C_45DE_9F46_75A916C86D38__INCLUDED_)
#define AFX_Client_H__C0496689_B41C_45DE_9F46_75A916C86D38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "SplashScreenEx.h"
/////////////////////////////////////////////////////////////////////////////
// CClientApp:
// See Client.cpp for the implementation of this class
//
#include "IniFile.h"
#include "SeuQQwry.h"

class CClientApp : public CWinApp
{
public:
	CIniFile m_IniFile;
	CSeuQQwry m_SeuQQwry;
	bool m_bIsDisablePopTips;
	bool m_bisDisableSound;
	CView* m_pConnectView; // 主连接视图
	CSplashScreenEx *pSplash;
	CClientApp();
	HMODULE m_hModule;
	DWORD GetRand();
	char AppPath[MAX_PATH];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CClientApp)
	afx_msg void OnAppAbout();
 	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Client_H__C0496689_B41C_45DE_9F46_75A916C86D38__INCLUDED_)
