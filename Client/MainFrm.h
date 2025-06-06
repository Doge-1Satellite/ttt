// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3F531B4D_21DF_49EA_B4AF_63AA5555104F__INCLUDED_)
#define AFX_MAINFRM_H__3F531B4D_21DF_49EA_B4AF_63AA5555104F__INCLUDED_

//#include "TrayIcon.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CMainFrameBase CXTPFrameWnd, CXTPOffice2007FrameHook

class CMainFrame : public CMainFrameBase
{	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
//	int nCount,nWinNT,nWinXP,nWin2K,nWin2K3,nWin2K8,nVista,nWin7,nWin8,nWin2012;
	CMap<UINT,UINT, CWnd* , CWnd*> m_mapPanes;
	int m_nCount;
// Operations
public:
CXTPTabControl m_wndTabControl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	int nOSCount[10];
	void ShowOSCount();
	void ShowToolTips(LPCTSTR lpszText);
	void ShowConnectionsNumber();
	static void ProcessReceiveComplete(ClientContext *pContext);
	static void ProcessReceive(ClientContext *pContext);
	void Activate(UINT nPort, UINT nMaxConnections);
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	void SetTheme(int iTheme);
	virtual ~CMainFrame();
	BOOL m_bSetRestart;


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif //_DEBUG

protected:  // control bar embedded members
	CStatusBar	m_wndStatusBar;
	int         m_iTheme;
	CXTTrayIcon m_TrayIcon;

	CXTPDockingPane*        m_pActivePane;
	CXTPDockingPaneManager  m_paneManager;
	CXTPDockingPane* CreatePane(int x, int y, CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour = NULL);
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);//就这个函数了
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnUpdateStatusBar(CCmdUI *pCmdUI);
	afx_msg void OnShow();
	afx_msg void OnHide();
	afx_msg void OnExit();
	afx_msg void OnAppExit();
	afx_msg void OnSetting();
	afx_msg void OnBuild();
	afx_msg void OnDnsUpdate();
	afx_msg void OnLock();
	afx_msg void OnTools();
	afx_msg void OnSz();
	afx_msg void LoadIcons();
	//}}AFX_MSG
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCustomize();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3F531B4D_21DF_49EA_B4AF_63AA5555104F__INCLUDED_)
