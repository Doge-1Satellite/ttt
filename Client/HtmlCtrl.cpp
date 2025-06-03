// HtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "MapDlg.h"
#include "HtmlCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlCtrl

IMPLEMENT_DYNCREATE(CHtmlCtrl, CHtmlView)

CHtmlCtrl::CHtmlCtrl()
{
	//{{AFX_DATA_INIT(CHtmlCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	//EnableAutomation();
}

CHtmlCtrl::~CHtmlCtrl()
{
}

BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
	//{{AFX_MSG_MAP(CHtmlCtrl)
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
BEGIN_DISPATCH_MAP(CHtmlCtrl, CHtmlView)
	//{{AFX_DISPATCH_MAP(CMyHtmlView)
	DISP_PROPERTY_EX(CHtmlCtrl, "lng", GetLng, SetLng, VT_BSTR)
	DISP_PROPERTY_EX(CHtmlCtrl, "lat", GetLat, SetLat, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// 在JavaScript中调用document.body.external.lng获取或设置属性

void CHtmlCtrl::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	CHtmlView::DocumentComplete(pDisp, URL);
	IHTMLDocument2Ptr ipDocument = GetHtmlDocument();
	if(ipDocument)
	{
		IHTMLElementPtr ipBody;
		ipDocument->get_body(&ipBody);
		LPDISPATCH pDispatch = GetIDispatch(TRUE);
		ipBody->setAttribute(L"external", _variant_t(pDispatch, false), 0);
	}
}

BSTR CHtmlCtrl::GetLng()
{
	switch (m_pMapDlg->m_map_tab.GetCurSel())
	{
	case 0:
		return m_pMapDlg->m_x_baidu.AllocSysString();
	case 1:
		return m_pMapDlg->m_x_gaode.AllocSysString();
	}
}

void CHtmlCtrl::SetLng(BSTR newValue)
{
	//char *NewValue = _com_util::ConvertBSTRToString(newValue);
	switch (m_pMapDlg->m_map_tab.GetCurSel())
	{
	case 0:
		m_pMapDlg->m_x_baidu = (LPSTR)newValue;
		break;
	case 1:
		m_pMapDlg->m_x_gaode = (LPSTR)newValue;
		break;
	}
	//delete NewValue;
}

BSTR CHtmlCtrl::GetLat()
{
	switch (m_pMapDlg->m_map_tab.GetCurSel())
	{
	case 0:
		return m_pMapDlg->m_y_baidu.AllocSysString();
	case 1:
		return m_pMapDlg->m_y_gaode.AllocSysString();
	}
}

void CHtmlCtrl::SetLat(BSTR newValue)
{
	//char *NewValue = _com_util::ConvertBSTRToString(newValue);
	switch (m_pMapDlg->m_map_tab.GetCurSel())
	{
	case 0:
		m_pMapDlg->m_y_baidu = (LPSTR)newValue;
		break;
	case 1:
		m_pMapDlg->m_y_gaode = (LPSTR)newValue;
		break;
	}
	//delete NewValue;
}
*/

/////////////////////////////////////////////////////////////////////////////
// CHtmlCtrl diagnostics

#ifdef _DEBUG
void CHtmlCtrl::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlCtrl::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Create control in same position as an existing static control with
// the same ID (could be any kind of control, really)

BOOL CHtmlCtrl::CreateFromStatic(UINT nID, CWnd* pParent)
{
	CStatic wndStatic; // 静态控件对象
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;
	m_pMapDlg = (CMapDlg*)pParent;
	
	// 获取静态控制的矩形区域并转换为父窗口的客户区坐标
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();
	
	// create HTML control (CHtmlView)
	BOOL bRet = Create(NULL,				// class name
		NULL,								// title
		(WS_CHILD | WS_VISIBLE ),			// style
		rc,									// rectangle
		pParent,							// parent
		nID,								// control ID
		NULL);								// frame/doc context not used
	m_pBrowserApp->put_Silent(TRUE);		// 禁止弹出脚本错误提示
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Override to avoid CView stuff that assumes a frame.

int CHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
{
	// bypass CView doc/frame stuff
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

/////////////////////////////////////////////////////////////////////////////
// Override to avoid CView stuff that assumes a frame.

void CHtmlCtrl::OnDestroy() 
{
	// This is probably unecessary since ~CHtmlView does it, but
	// safer to mimic CHtmlView::OnDestroy.
	if (m_pBrowserApp) {
		m_pBrowserApp->Release();
		m_pBrowserApp = NULL;
	}
	CWnd::OnDestroy(); // bypass CView doc/frame stuff
}

void CHtmlCtrl::PostNcDestroy() 
{

}

/////////////////////////////////////////////////////////////////////////////
// Override navigation handler to pass to "app:" links to virtual handler.
// Cancels the navigation in the browser, since app: is a pseudo-protocol.

void CHtmlCtrl::OnBeforeNavigate2(LPCTSTR lpszURL,
	DWORD nFlags,
	LPCTSTR lpszTargetFrameName,
	CByteArray& baPostedData,
	LPCTSTR lpszHeaders,
	BOOL* pbCancel)
{
	const char APP_PROTOCOL[] = "app:";
	int len = _tcslen(APP_PROTOCOL);
	if (_tcsnicmp(lpszURL, APP_PROTOCOL, len)==0) {
		OnAppCmd(lpszURL + len);
		*pbCancel = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Called when the browser attempts to navigate to "app:foo"
// with "foo" as lpszWhere. Override to handle app commands.

void CHtmlCtrl::OnAppCmd(LPCTSTR lpszWhere)
{
	// default: do nothing
}
