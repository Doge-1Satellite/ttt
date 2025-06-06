#if !defined(AFX_MAPDLG_H__24E8C025_73BA_4C9B_AC7F_E2D6F50E325C__INCLUDED_)
#define AFX_MAPDLG_H__24E8C025_73BA_4C9B_AC7F_E2D6F50E325C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapDlg dialog

/////////////////
// Handle "app:ok" link by closing dialog
//

class CHtmlCtrl;
 
class CMapDlg : public CDialog
{
// Construction
public:
	CMapDlg(CWnd* pParent = NULL,int nItem = 0,ClientContext *pContext = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMapDlg)
	enum { IDD = IDD_MAP_DIALOG };
	CXTTabCtrl m_map_tab;
	CString m_1;
	CString m_3;
	//}}AFX_DATA
	ClientContext *m_pContext;
	CString m_IPAddress;
	CString m_x_baidu,m_y_baidu;
	CString m_x_gaode,m_y_gaode;
	BOOL	m_bBaidu,m_bGaode;
	char	m_szAppBaidu[MAX_PATH];
	char	m_szAppGaode[MAX_PATH];
	CString m_strLngLatBaidu;
	CString m_strAddressBaidu;
	CString m_strLngLatGaode;
	CString m_strAddressGaode;
	CString HtmlMapBaidu(BOOL bBaiduAvl);
	CString HtmlMapGaode(BOOL bGaodeAvl);
	CHtmlCtrl* m_pPage;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelchangeMapTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPDLG_H__24E8C025_73BA_4C9B_AC7F_E2D6F50E325C__INCLUDED_)
