//-------------------------------------------------------------------
// CCaptureVideo视频捕捉类实现文件CaptureVideo.cpp
//-------------------------------------------------------------------
// CaptureVideo.cpp: implementation of the CCaptureVideo class.
//
/////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#define STRSAFE_NO_DEPRECATE
#include "CaptureVideo.h"
// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

CSampleGrabberCB mCB;
#define printf
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCaptureVideo::CCaptureVideo()
{
	m_bIsStarted = false;
	m_nWidth = 0;
	m_nHeight = 0;
	m_bIsConnected = false;
	m_pEnc = NULL;
	InitializeCriticalSection(&m_cs);
  //COM Library Intialization
	m_hCaptureEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  if(FAILED(CoInitialize(NULL))) /*, COINIT_APARTMENTTHREADED)))*/
  {
    return;
  }
  m_pMC = NULL;
  m_pGB = NULL;
  m_pBF = NULL; 
  m_pGrabber = NULL; 
  m_pCapture = NULL; 
  EnumDevices(&m_devInfo);
}

CCaptureVideo::~CCaptureVideo()
{
  // Stop media playback
	// Stop media playback
	
	if (m_pEnc)
	{
		if (m_bIsStarted)
		{
			m_pEnc->Close() ;
		}
		delete m_pEnc ;
        m_pEnc = NULL;
	}
	while (!m_lpBuf.empty())
	{
		delete[] m_lpBuf.front();
		m_lpBuf.pop_front();
	}
	if (m_bIsConnected)
	{	
		if (m_lpbmi)
			delete m_lpbmi;
		if (m_lpDIB)
			delete[] m_lpDIB;
		m_bIsConnected = false;
	}
//	CloseWindow(m_hWnd);
//	DestroyWindow(m_hWnd);
	CloseHandle(m_hCaptureEvent);
	DeleteCriticalSection(&m_cs);
// 	if(m_pMC)m_pMC->StopWhenReady();
// 	if(m_pVW){
// 		m_pVW->put_Visible(OAFALSE);
// 		m_pVW->put_Owner(NULL);
// 	}
// 	SAFE_RELEASE(m_pMC);
// 	SAFE_RELEASE(m_pVW); 
// 	SAFE_RELEASE(m_pGB);
// 	SAFE_RELEASE(m_pBF);
// 	SAFE_RELEASE(m_pGrabber); 
// 	SAFE_RELEASE(m_pCapture);
	CoUninitialize() ; 
}
bool CCaptureVideo::IsWebCam()
{
	if (m_bIsConnected)
		return false;
// 	if (0 >= EnumDevices())
// 	{
// 		return false;
// 	}
	return true;
}

void CCaptureVideo::CamFrameData(double dblSampleTime, BYTE * pBuffer, long lBufferSize)
{
//	static unsigned int tickcount = 2;
	if (m_pEnc != NULL)
	{
		m_pEnc->Encode(pBuffer);
	}
}
void CCaptureVideo::PostEncHandler(unsigned char * xvid, int key, int xvid_len)
{
	char *pByte = new char[xvid_len + 4];
	memcpy(pByte,&xvid_len,sizeof(int));
	memcpy(pByte+sizeof(int), xvid, xvid_len);
	EnterCriticalSection(&m_cs);
	m_lpBuf.push_back(pByte);
	LeaveCriticalSection(&m_cs);
 	SetEvent(m_hCaptureEvent);
}

bool CCaptureVideo::CheckEncodeNull()
{
    return m_pEnc == NULL;
}
int CCaptureVideo::EnumDevices(DeviceInfo *head)
{
  if (!head)
		return 0;
  DeviceInfo *ptr = head;
  int id = 0;
  //枚举视频扑捉设备
  ICreateDevEnum *pCreateDevEnum;
  
  ICaptureGraphBuilder2 *pBuilder = NULL;
  IBaseFilter *pVCap = NULL;
  IAMStreamConfig *pVSC = NULL;
//  char cResolution[MAX_PATH] = {0};
  HRESULT hr = CoCreateInstance((REFCLSID)CLSID_CaptureGraphBuilder2,
		NULL, CLSCTX_INPROC, (REFIID)IID_ICaptureGraphBuilder2,
		(void **)&pBuilder);
  if (hr != NOERROR)
  {
	  return -1;
  }
  hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void**)&pCreateDevEnum);

  if (hr != NOERROR)
  {
	  return -1;
  }
  CComPtr<IEnumMoniker> pEm;
  hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);

  if (hr != NOERROR)
  {
	  return -1;
  }
  pEm->Reset();
  ULONG cFetched;
  IMoniker *pM;
  while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK) {
    IPropertyBag *pBag;
    hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
    if(SUCCEEDED(hr)) {
      VARIANT var;
      var.vt = VT_BSTR;
      hr = pBag->Read(L"FriendlyName", &var, NULL);
      if (hr == NOERROR) {
        char str[2048]; 
		if (ptr->deviceId != -1)
		{
			ptr->next = new DeviceInfo();
			ptr = ptr->next;
		}


		ptr->deviceId = id++;
		WideCharToMultiByte(CP_ACP,0,var.bstrVal, -1, str, 2048, NULL, NULL);
		ptr->friendlyName = str;
        SysFreeString(var.bstrVal);
      }
      pBag->Release();
    }
	hr = pM->BindToObject(0, 0, IID_IBaseFilter, (void**)&pVCap);
	if(SUCCEEDED(hr))
        {
			hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,
				pVCap, IID_IAMStreamConfig, (void **)&pVSC);
			if(hr != NOERROR)
			{
				hr = pBuilder->FindInterface(&PIN_CATEGORY_PREVIEW,&MEDIATYPE_Video,
					pVCap, IID_IAMStreamConfig, (void **)&pVSC);
			}

			if(SUCCEEDED(hr))
			{
				int iCount = 0, iSize = 0;
				hr = pVSC->GetNumberOfCapabilities(&iCount, &iSize);
				ResolutionInfo** pRes = &(ptr->resInfo);
				if(iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
				{
					int iformatindex;
					for (int iFormat = 0; iFormat < iCount; iFormat++)
					{
						VIDEO_STREAM_CONFIG_CAPS scc;
						AM_MEDIA_TYPE *pmtConfig = NULL;
						hr = pVSC->GetStreamCaps(iFormat, &pmtConfig, (BYTE*)&scc);
						if (SUCCEEDED(hr))
						{
							//分辨率
							if(HEADER(pmtConfig->pbFormat)->biWidth != 0 && HEADER(pmtConfig->pbFormat)->biHeight != 0)
							{
								char cResolution[32] = {0};
								sprintf(cResolution,"%d * %d:",HEADER(pmtConfig->pbFormat)->biWidth,HEADER(pmtConfig->pbFormat)->biHeight);
								*pRes = new ResolutionInfo;
								(*pRes)->Resolution = cResolution;
								pRes = &((*pRes)->next);
//								strFormat.Format(_T("%d*%d "),HEADER(pmtConfig->pbFormat)->biWidth,HEADER(pmtConfig->pbFormat)->biHeight);
								
							}

						}
					}
				}
				pM->AddRef();
			}
		}
    pM->Release();
  }
  SAFE_RELEASE(pBuilder);
  SAFE_RELEASE(pVCap);
  SAFE_RELEASE(pVSC);
  return id;
}

HRESULT CCaptureVideo::Stop() 
{
	// Stop media playback
	if(m_pMC)m_pMC->StopWhenReady();

	SAFE_RELEASE(m_pMC);
	SAFE_RELEASE(m_pGB);
	SAFE_RELEASE(m_pBF);
	SAFE_RELEASE(m_pNullRenderer);
	SAFE_RELEASE(m_pGrabber); 
	SAFE_RELEASE(m_pCapture);

	m_pMC = NULL;
	m_pGB = NULL;
	m_pBF = NULL; 
	m_pNullRenderer = NULL;
	m_pGrabber = NULL; 
	m_pCapture = NULL; 
	return S_OK ; 
}

HRESULT CCaptureVideo::Start(int iDeviceID, HWND hWnd , int width, int height)
{
  HRESULT hr;
  
  hr = InitCaptureGraphBuilder();
  if (FAILED(hr)){
    return hr;
  }
  // Bind Device Filter. We know the device because the id was passed in
  if(!BindFilter(iDeviceID, &m_pBF))
  { 
	  return S_FALSE;
  }
  hr = m_pGB->AddFilter(m_pBF, L"Capture Filter");
  // create a sample grabber
  hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_ISampleGrabber, (void**)&m_pGrabber );
  if(FAILED(hr)){
    return hr;
  }

  hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
	  IID_IBaseFilter, (LPVOID*) &m_pNullRenderer);
  if(FAILED(hr)){
	  return hr;
  }

  hr = m_pGB->AddFilter(m_pNullRenderer, L"NullRenderer");
  CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase( m_pGrabber );
		
		//设置视频格式
  AM_MEDIA_TYPE mt; 
  ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
  mt.majortype = MEDIATYPE_Video;
#ifdef MY_TEST
  mt.subtype = MEDIASUBTYPE_RGB24; // MEDIASUBTYPE_RGB24 ; 
#else
  mt.subtype = MEDIASUBTYPE_RGB32; // MEDIASUBTYPE_RGB24 ; 
#endif
  hr = m_pGrabber->SetMediaType(&mt);
		
  if( FAILED(hr) )
  {
    return hr;
  }
  hr = m_pGB->AddFilter( pGrabBase, L"Grabber" );
  if( FAILED( hr ) ){
	return hr;
  }
  IAMStreamConfig *pConfig;
  hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Interleaved,m_pBF,IID_IAMStreamConfig,(void **)&pConfig);
  if( hr != NOERROR )
  {
    hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,m_pBF,IID_IAMStreamConfig,(void **)&pConfig);
 	if(hr != NOERROR)
		return hr;
  }	
  m_nWidth = width;
  m_nHeight = height;
  AM_MEDIA_TYPE   *mmt   =   NULL;   
  hr   =   pConfig-> GetFormat(&mmt); 
  VIDEOINFOHEADER *vvih = (VIDEOINFOHEADER*)mmt->pbFormat; 
  if(vvih == NULL)
  {
	  return S_FALSE;
  }
  AM_MEDIA_TYPE *pmt = mmt; 
  VIDEOINFOHEADER *pvih = (VIDEOINFOHEADER*)pmt->pbFormat; 
  if(pvih == NULL)
  {
	  return S_FALSE;
  }
  pvih->bmiHeader.biHeight=height;     //修改采集视频的高为240 
  pvih->bmiHeader.biWidth=width;       //修改采集视频的宽为320 
  pvih->AvgTimePerFrame = 10000000/10;
  pmt->pbFormat = (unsigned char*)pvih; 
  hr = pConfig->SetFormat(pmt);       //重新设置参数 
  if(FAILED(hr))
  {
	return hr;
  }
  SAFE_RELEASE(pConfig);
  // try to render preview/capture pin
  hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,m_pBF,pGrabBase,m_pNullRenderer);
  if( FAILED( hr ) )
  {
    hr = m_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,m_pBF,pGrabBase,m_pNullRenderer);

    if( FAILED( hr ) ){
      return hr;
    }
  }
#if 0
  hr = m_pGrabber->GetConnectedMediaType( &mt );
  if ( FAILED( hr) ){
    return hr;
  }

  VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mt.pbFormat;
  vih->bmiHeader.biWidth = width;
  vih->bmiHeader.biHeight = height;
  hr = m_pGrabber->SetMediaType(&mt);
#endif
  mCB.lWidth = width;
  mCB.lHeight = height;
  mCB.bGrabVideo = FALSE ; 
  mCB.frame_handler = NULL ; 
  m_lpbmi = new BITMAPINFO;
  m_lpbmi->bmiHeader.biWidth = mCB.lWidth;
  m_lpbmi->bmiHeader.biHeight = mCB.lHeight;
  m_lpbmi->bmiHeader.biPlanes = 1;
#ifdef MY_TEST
  m_lpbmi->bmiHeader.biBitCount = 24;
#else
  m_lpbmi->bmiHeader.biBitCount = 32;
#endif
  m_lpbmi->bmiHeader.biSizeImage = (((m_lpbmi->bmiHeader.biWidth * m_lpbmi->bmiHeader.biBitCount + 31) & ~31) >> 3) * m_lpbmi->bmiHeader.biHeight;
  m_lpbmi->bmiHeader.biCompression = BI_RGB;
  m_lpDIB = new BYTE[m_lpbmi->bmiHeader.biSizeImage + 4];

  FreeMediaType(mt);
  DeviceInfo* ptr = &m_devInfo;
  BOOL bUSBDevice = FALSE;
  while(ptr)
  {
	  if (ptr->deviceId == iDeviceID)
	  {
			if (strstr(ptr->friendlyName.GetBuffer(),"USB"))
			{
				bUSBDevice = TRUE;
			}
			break;
	  }
	  ptr = ptr->next;
  }
  hr = m_pGrabber->SetBufferSamples( FALSE );
  hr = m_pGrabber->SetOneShot( FALSE );
  hr = m_pGrabber->SetCallback( &mCB, 0 );
  int nCount = 0;
  while (m_pEnc == NULL && nCount < 5)
  {
    m_pEnc = new CXvidEnc;
    nCount++;
  }
  if (m_pEnc == NULL)
  {
      printf("[CCaptureVideo] :: m_pEnc == NULL\n");
      return S_FALSE;
  }
  m_pEnc->AttachCaller(mCB.lWidth, mCB.lHeight, this) ; 
  CXvidEnc::XVID_GLOBAL_INIT() ; 
  m_pEnc->Open();
  m_bIsStarted = true;
  //设置视频捕捉窗口
//  SetupVideoWindow();
  hr = m_pMC->Run();//开始视频捕捉
  if(FAILED(hr))
  {
	  return hr;
  }
  return S_OK;
}

bool CCaptureVideo::BindFilter(int deviceId, IBaseFilter **pFilter)
{
  if (deviceId < 0)
  return false;

  // enumerate all video capture devices
  CComPtr<ICreateDevEnum> pCreateDevEnum;
  HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
				IID_ICreateDevEnum, (void**)&pCreateDevEnum);
  if (hr != NOERROR)
  {
    return false;
  }
  CComPtr<IEnumMoniker> pEm;
  hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
  if (hr != NOERROR) 
  {
    return false;
  }
  pEm->Reset();
  ULONG cFetched;
  IMoniker *pM;
  int index = 0;
  while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= deviceId)
  {
    IPropertyBag *pBag;
    hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
    if(SUCCEEDED(hr)) 
    {
      VARIANT var;
      var.vt = VT_BSTR;
      hr = pBag->Read(L"FriendlyName", &var, NULL);
      if (hr == NOERROR) 
      {
        if (index == deviceId)
        {
          pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
        }
        SysFreeString(var.bstrVal);
      }
      pBag->Release();
    }
    pM->Release();
    index++;
  }
  return true;
}

HRESULT CCaptureVideo::InitCaptureGraphBuilder()
{
  HRESULT hr;

  // 创建IGraphBuilder接口
  hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGB);
  // 创建ICaptureGraphBuilder2接口
  hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
						IID_ICaptureGraphBuilder2, (void **) &m_pCapture);
  if (FAILED(hr))return hr;
  m_pCapture->SetFiltergraph(m_pGB);
  hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);
  if (FAILED(hr))return hr;
  return hr;
}

void CCaptureVideo::FreeMediaType(AM_MEDIA_TYPE& mt)
{
  if (mt.cbFormat != 0) {
    CoTaskMemFree((PVOID)mt.pbFormat);
    // Strictly unnecessary but tidier
    mt.cbFormat = 0;
    mt.pbFormat = NULL;
  }
  if (mt.pUnk != NULL) {
    mt.pUnk->Release();
    mt.pUnk = NULL;
  }
}

LPBYTE CCaptureVideo::GetDIB()
{
 	DWORD	dwRet = WaitForSingleObject(m_hCaptureEvent, INFINITE);
	EnterCriticalSection(&m_cs);
	if (m_lpBuf.empty())
	{
		LeaveCriticalSection(&m_cs);
		return NULL;
	}
	else
	{
		LPBYTE pByte = (LPBYTE)(m_lpBuf.front());
		m_lpBuf.pop_front();
		LeaveCriticalSection(&m_cs);
		return pByte;
	}
}

void CCaptureVideo::GrabVideoFrames(BOOL bGrabVideoFrames, CCamFrameHandler * frame_handler)
{
	mCB.frame_handler = frame_handler ; 
	mCB.bGrabVideo = bGrabVideoFrames ; 
}

