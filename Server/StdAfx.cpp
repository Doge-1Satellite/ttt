// stdafx.cpp : source file that includes just the standard includes
//	Server.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
WORD    InstallMode;    // 运行模式
UINT    ConnectType;    // 上线类型
LPCTSTR lpConnInfos[2]; // 上线域名
DWORD   dwConnPorts[2]; // 上线端口
LPTSTR  lpszUserSid;    // 活动帐号
char *scEXEName;  //接入的参数
