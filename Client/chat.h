﻿#if !defined(AFX_CHAT_H__9C1E4A0B_E2CB_4CA7_9458_CA834FB28F32__INCLUDED_)
#define AFX_CHAT_H__9C1E4A0B_E2CB_4CA7_9458_CA834FB28F32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChat dialog

class CChat : public CDialog
{
// Construction
public:
	CChat(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
	void OnReceiveComplete();

// Dialog Data
	//{{AFX_DATA(CChat)
	enum { IDD = IDD_CHAT };
	CEdit	m_editTip;
	CEdit	m_editNewMsg;
	CEdit	m_editChatLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChat)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CString m_IPAddress;
	// Generated message map functions
	//{{AFX_MSG(CChat)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonEnd();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetfocusEditChatLog();
	afx_msg void OnKillfocusEditChatLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAT_H__9C1E4A0B_E2CB_4CA7_9458_CA834FB28F32__INCLUDED_)
