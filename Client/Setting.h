#if !defined(AFX_Setting_H__CA28388C_8306_4FD8_84E0_AE175B7C76C8__INCLUDED_)
#define AFX_Setting_H__CA28388C_8306_4FD8_84E0_AE175B7C76C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetting dialog

class CSetting : public CDialog
{
// Construction
public:
	CSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetting)
	enum { IDD = IDD_SETTING };
	CXTPButton	m_bisDisableSound_XTP;
	CXTPEdit	m_showtips;
	CXTPEdit	m_listen_port_XTP;
	CXTPButton	m_disable_poptips_XTP;
	CXTPEdit	m_connect_max_XTP;
	CXTPButton	m_connect_auto_XTP;
	UINT	m_max_connections;
	BOOL	m_connect_auto;
	BOOL	m_bIsDisablePopTips;
	UINT	m_listen_port;
	BOOL	m_bisDisableSound;
	BOOL	m_line;
	BOOL	m_national;
	CXTPComboBox	m_groupxtp;
    CXTPComboBox	m_groupxtp2;
	DWORD operate;                 //双击时的默认操作
	CComboBox m_operateCmb;
	void InitOperate(void);
//    void rebootmeA();
	int     m_color;
	BOOL    m_bisDisablePifu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting)
	afx_msg void OnApply();
	afx_msg void OnDisablePoptips();
	afx_msg void OnConnectAuto();
	virtual BOOL OnInitDialog();
	afx_msg void OnDisableSound();
	afx_msg void OnEditchangeGroupStyle();
	afx_msg void OnEditchangeGroupStyle2();
	//新添加
	afx_msg void OnCustomSkin();
	afx_msg void Online();
	afx_msg void OnNational();
	afx_msg void OnExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDISABLEPifu();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcon;
public:
	
	BOOL m_dawed_1;
	BOOL m_dawed_2;
	BOOL m_dawed_3;
	BOOL m_dawed_4;
	BOOL m_dawed_5;
	BOOL m_dawed_6;
	BOOL m_dawed_7;
	BOOL m_dawed_8;
	BOOL m_dawed_9;
	BOOL m_dawed_10;
	BOOL m_dawed_11;
	BOOL m_dawed_12;
	BOOL m_dawed_13;
	BOOL m_dawed_14;
	BOOL m_dawed_15;
	BOOL m_dawed_16;
	BOOL m_dawed_17;
	BOOL m_dawed_18;
	BOOL m_dawed_19;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Setting_H__CA28388C_8306_4FD8_84E0_AE175B7C76C8__INCLUDED_)
