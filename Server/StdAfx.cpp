// stdafx.cpp : source file that includes just the standard includes
//	Server.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
WORD    InstallMode;    // ����ģʽ
UINT    ConnectType;    // ��������
LPCTSTR lpConnInfos[2]; // ��������
DWORD   dwConnPorts[2]; // ���߶˿�
LPTSTR  lpszUserSid;    // ��ʺ�
char *scEXEName;  //����Ĳ���