// serverthread.cpp

#include "stdafx.h"
#include "blocksock.h"
#define SERVERMAXBUF 5000
#define MAXLINELENGTH 100


#pragma comment(lib, "ws2_32.lib")
#include   <iostream> 
#include   <string> 
#include   <stdio.h> 
#include   <list> 

volatile int g_nConnection=0;//���ӵĸ���
volatile BOOL g_bListening=FALSE;//�����׽���״̬
volatile UINT g_nPortServer=80;//����˿�
CString g_strDirect="c:\\WebSite\\";//����·��
CString g_strIPServer;//��������ַ
CString g_strDefault="index.htm";//ȱʡ��ҳ������
CMyBlockSocket g_sListen;//�����׽���

//��������
BOOL Parse(char* pStr, char** ppToken1, char** ppToken2)
{
	*ppToken1=pStr;
	char* pch=strchr(pStr, ' ');
	if(pch) 
	{
		*pch='\0';
		pch++;
		*ppToken2=pch;
		pch=strchr(pch, ' ');
		if(pch) {
			*pch='\0';
			return TRUE;
		}
	}
	return FALSE;
}

//������ļ���д��־
void LogRequest(LPVOID pParam, char* pch, CSocketAddress sa)
{
	//pParam����������CListBox�����ָ��
	CString strList;
	CListBox* pList=(CListBox*)pParam;
	CString strGmt=CTime::GetCurrentTime().FormatGmt("%m/%d/%y %H:%M:%S");
	strList.Format("[%s] ���������� # %d ",strGmt,g_nConnection);
	pList->AddString(strList);
	strList.Format("[%s] IP��ַ:%s �˿�:%d", strGmt,sa.DottedDecimal(), sa.Port());
	pList->AddString(strList);
	//strList.Format("ʱ��:%s",strGmt);
	//pList->AddString(strList);
	strList.Format("[%s] ����:%s", strGmt,pch);
	pList->AddString(strList);
}

//����·��ת��ʵ·�������ļ������ؾ��
CFile* OpenFile(const char* pName)
{
	//���ļ�
	//ע�⣬��������˷�����·��
	CFileException e;
	CFile* pFile=new CFile();
	if(*pName=='/') pName++;
	CString strName=pName;
	strName.Replace('/','\\');
	if(pFile->Open(g_strDirect+strName, CFile::modeRead, &e)) {
		return pFile;
	}
	if((e.m_cause==CFileException::accessDenied) ||
			(e.m_cause==CFileException::badPath)) 
	{
		//������
		int nLength;
		// add a \ unless it's the "root" directory
		if((nLength=strName.GetLength()) > 1) 
		{
			if(strName[nLength-1]!='\\') 
			{
				strName+='\\';
			}
		}
		//ָ��ȱʡ���ļ���
		strName+=g_strDefault;
		if(pFile->Open(g_strDirect+strName, CFile::modeRead, &e)) 
			return pFile;
	}
	delete pFile;
	return NULL;
}

//��¼������Ϣ
void LogBlockingSocketException(LPVOID pParam, char* pch, CMyBlockSocketException* pe)
{
	//pParam����������CListBox�����ָ��
	CListBox* pList=(CListBox*)pParam;
	CString strGmt = CTime::GetCurrentTime().FormatGmt("%m/%d/%y %H:%M:%S");
	char text2[50];
	pe->GetErrorMessage(text2, 49);
	CString strList;
	pList->AddString(strList);
	strList.Format("WINSOCK����!");
	pList->AddString(strList);
	strList.Format("%s",pch);
	pList->AddString(strList);
	strList.Format("������Ϣ:%s",text2);
	pList->AddString(strList);
	strList.Format("ʱ��:%s",strGmt);
	pList->AddString(strList);
}

//�����߳�
UINT ServerThreadProc(LPVOID pParam)
{
	CSocketAddress saClient;
	CMyHttpBlockSocket sConnect;
	CListBox* pList=(CListBox*)pParam;
	//������
	char* buffer=new char[SERVERMAXBUF];
	char headers[500], 
		 request1[MAXLINELENGTH],  
		 request2[MAXLINELENGTH];
	//���Ӵ��󷵻ص��������Ϣ
	char hdrErr[]=
		"HTTP/1.0 404 ����û���ҵ�\r\n"
		"Server: MySocket Server\r\n"
		"Content-Type: text/html\r\n"
		"Accept-Ranges: bytes\r\n"
		"Content-Length: 66\r\n\r\n"
		"<html><h1><body>HTTP/1.0 404 ����û���ҵ�</h1></body></html>\r\n";
	//������ȷʱ���ص���Ϣ
	char hdrFmt[]=
		"HTTP/1.0 200 OK\r\n"
		"Server: MySocket Server\r\n"
		"Date: %s\r\n"
		"Content-Type: text/html\r\n"
		"Accept-Ranges: bytes\r\n"
		"Content-Length: %d\r\n";
	//Ĭ�ϵ�ҳ��--default HTML page
	char CustomHtml[]=
		"<html>\r\n"
		"<head>\r\n"
		"<title></title>\r\n"
		"</head>\r\n"
		"<body>\r\n"
		"<p align=\"center\">��ӭ�����ҵ���ҳ</p>\r\n"
		"<h3 align=\"center\"><a href=\"Default.htm\">�������</a></h3>\r\n"
		"<p>����</p>\r\n"
		"<p>��</p>\r\n"
		"</body></html>\r\n\r\n";
	
	CString strGmtNow=
		CTime::GetCurrentTime().FormatGmt("%a, %d %b %Y %H:%M:%S");
	int nBytesSent=0;
	CFile* pFile=NULL;
	char* pToken1;
	char* pToken2;

	try 
	{
		//��ʼ������������
		if(!g_sListen.Accept(sConnect, saClient)) 
		{
			//��Ӧ�ó���ر�ʱ�Ĵ���
			g_bListening=FALSE;
			delete [] buffer;
			return 0;
		}
		//����������һ��
		g_nConnection++;
		//��ʼ��һ���������߳�
		AfxBeginThread(ServerThreadProc, pParam, THREAD_PRIORITY_NORMAL);
		//�ӿͻ���(�����)��ȡ����
		sConnect.ReadHttpHeaderLine(request1, MAXLINELENGTH, 10);
		//��¼����
		LogRequest(pParam, request1, saClient);
		//������������Ӧ�Ĵ���
		if(Parse(request1, &pToken1, &pToken2))
		{
			//�����GET��ʽ
			if(!_stricmp(pToken1, "GET")) 
			{
				do 
				{	
					//��ȡ�����ʣ�ಿ��
					sConnect.ReadHttpHeaderLine(request2, MAXLINELENGTH, 10);
				}
				while(strcmp(request2, "\r\n"));
				if(!_stricmp(pToken2, "/custom"))// || !stricmp(pToken2, "/"))
				{
					//����Ĭ�ϵ�ҳ��--default HTML page
					wsprintf(headers, hdrFmt, (const char*) strGmtNow, strlen(CustomHtml));
					strcat(headers, "\r\n");
					sConnect.Write(headers, strlen(headers), 10);
					sConnect.Write(CustomHtml, strlen(CustomHtml), 10);
				}
				else if(strchr(pToken2, '?')) 
				{
					//CGI����
					//��HTTP�����������ܶ�CGI����������Ӧ
				}
				else
				{
					//�ļ�����
					//ע��Է�����Ŀ¼���趨
					//����ļ��ڷ������ϵ�·��
					if((pFile=OpenFile(pToken2))!=NULL) 
					{
						//�ļ���
						CFileStatus fileStatus;
						pFile->GetStatus(fileStatus);
						CString strGmtMod=fileStatus.m_mtime.FormatGmt("%a, %d %b %Y %H:%M:%S");
						char hdrModified[50];
						wsprintf(hdrModified, "Last-Modified: %s\r\n\r\n", (const char*) strGmtMod);
						DWORD dwLength=pFile->GetLength();
						wsprintf(headers, hdrFmt,  (const char*) strGmtNow, dwLength);
						strcat(headers, hdrModified);
						nBytesSent=sConnect.Write(headers, strlen(headers), 10);
						//���͵��ļ�Ӧ������ĳһ��ʱ��֮���޸Ĺ���
						//����ļ���ʱ��Ӧ����С�ڸ��趨ʱ��
						nBytesSent=0;
						DWORD dwBytesRead=0;
						UINT uBytesToRead;
						//���ļ���5k�Ĵ�СΪ��λ���ͣ������ڴ�������
						while(dwBytesRead < dwLength) 
						{
							uBytesToRead=min(SERVERMAXBUF, dwLength-dwBytesRead);
							VERIFY(pFile->Read(buffer, uBytesToRead)==uBytesToRead);
							nBytesSent+=sConnect.Write(buffer, uBytesToRead, 10);
							dwBytesRead+=uBytesToRead;
						}
					}
					else
					{
						//����������͡�������Ϣ��
						nBytesSent=sConnect.Write(hdrErr, strlen(hdrErr), 10);
					}
				}
			}
			else if(!_stricmp(pToken1, "POST"))
			{
				//�����POST��ʽ
				do 
				{
					//��ȡ�����ʣ�ಿ��
					sConnect.ReadHttpHeaderLine(request2, MAXLINELENGTH, 10);
				}
				while(strcmp(request2, "\r\n"));
				sConnect.ReadHttpHeaderLine(request2, MAXLINELENGTH, 10);
				LogRequest(pParam, request2, saClient);
				//����������͡�������Ϣ��
				nBytesSent=sConnect.Write(hdrErr, strlen(hdrErr), 10);
			}
			else 
			{
				//����������ʽ
			}
		}
		else 
		{
			//���������
		}
		//�ر��׽���
		sConnect.Close();
	}
	catch(CMyBlockSocketException* pe)
	{
		//������
		LogBlockingSocketException(pParam, "������:", pe);
		pe->Delete();
	}
	delete [] buffer;
	if(pFile) delete pFile;
	return 0;
}


