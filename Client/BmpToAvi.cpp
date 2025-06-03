// BmpToAvi.cpp: implementation of the CBmpToAvi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BmpToAvi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AVISTREAMINFO CBmpToAvi::m_si;

CBmpToAvi::CBmpToAvi()
{
	m_pfile = NULL;
	m_pavi = NULL;
	memset(&m_bHeader,0,sizeof(BITMAPINFOHEADER));
	m_bHeader.biSize = sizeof(BITMAPINFOHEADER); 
	m_bHeader.biWidth = 320;
	m_bHeader.biHeight = 240 ;
	m_bHeader.biPlanes = 1 ;
	m_bHeader.biBitCount = 24 ;
	m_bHeader.biCompression  = mmioFOURCC('X','V','I','D');//BI_RGB ;
	m_bHeader.biSizeImage = 320 * 240 * 3 ;
	

	AVIFileInit();
}

CBmpToAvi::~CBmpToAvi()
{

	AVIFileExit();
}

bool CBmpToAvi::Open( LPCTSTR szFile, LPBITMAPINFO lpbmi )
{
	if (szFile == NULL)
		return false;
	m_nFrames = 0;

	if (AVIFileOpen(&m_pfile, szFile, OF_WRITE | OF_CREATE, NULL))
		return false;
	m_bHeader.biWidth = lpbmi->bmiHeader.biWidth;
	m_bHeader.biHeight = lpbmi->bmiHeader.biHeight;
	m_bHeader.biSizeImage = lpbmi->bmiHeader.biSizeImage;
	m_si.fccType = streamtypeVIDEO;
	m_si.fccHandler = mmioFOURCC('X','V','I','D');

	m_si.dwScale = 1;
	m_si.dwRate = 10; // Ã¿Ãë5Ö¡
	SetRect(&m_si.rcFrame, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight);
	m_si.dwSuggestedBufferSize = lpbmi->bmiHeader.biSizeImage;
	
	
	if (AVIFileCreateStream(m_pfile, &m_pavi, &m_si))
			return false;

	

	return true;
}

bool CBmpToAvi::Write(LPVOID lpBuffer,int len)
{
	if (m_pfile == NULL || m_pavi == NULL)
		return false;
	AVIStreamSetFormat(m_pavi, m_nFrames, &m_bHeader, sizeof(BITMAPINFO));
	return AVIStreamWrite(m_pavi, m_nFrames++, 1, lpBuffer, len, AVIIF_KEYFRAME, NULL, NULL) == AVIERR_OK;
}


void CBmpToAvi::Close()
{
	if (m_pavi)
	{
		AVIStreamRelease(m_pavi);
		m_pavi = NULL;
	}
	if (m_pfile)
	{
		AVIFileRelease(m_pfile);
		m_pfile = NULL;
	}		
}