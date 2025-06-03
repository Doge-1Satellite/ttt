// Signature.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Signature.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSignature

IMPLEMENT_DYNCREATE(CSignature, CFormView)

CSignature::CSignature()
	: CFormView(CSignature::IDD)
{
	//{{AFX_DATA_INIT(CSignature)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSignature::~CSignature()
{
}

void CSignature::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSignature)
	DDX_Control(pDX, IDC_Log, m_Log);
	DDX_Text(pDX, IDC_EDIT1, m_path);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSignature, CFormView)
	//{{AFX_MSG_MAP(CSignature)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
 	ON_WM_DROPFILES()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_xuanwu, Onxuanwu)
	ON_BN_CLICKED(IDC_weishi, Onweishi)
	ON_BN_CLICKED(IDC_kugou, Onkugou)
	ON_BN_CLICKED(IDC_liulanqi, Onliulanqi)
	ON_BN_CLICKED(IDC_xunlei, Onxunlei)
	ON_BN_CLICKED(IDC_tengxun, Ontengxun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSignature diagnostics

#ifdef _DEBUG
void CSignature::AssertValid() const
{
	CFormView::AssertValid();
}

void CSignature::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSignature message handlers

void CSignature::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	::DragAcceptFiles(GetSafeHwnd(),true);
	m_dropEdit.SubclassDlgItem(IDC_EDIT1, this);
	m_dropEdit.SetUseDir(FALSE);
	GetDlgItem(IDC_EDIT1)->SetWindowText("把文件拖到这里点击签名....");

}


HBRUSH CSignature::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if ((pWnd->GetDlgCtrlID() == IDC_Log) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(0, 255, 0);
		pDC->SetTextColor(clr);   //设置白色的文本
		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);     //设置黑色的背景
		return CreateSolidBrush(clr);  //作为约定，返回背景色对应的刷子句柄
	}
	else
	{
		return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


void CSignature::OnDropFiles( HDROP hDropInfo )
{
	UINT cFiles = ::DragQueryFile( hDropInfo, 0xffffffff, 0, 0);    //取得文件数
	CString szText;
	char szFile[MAX_PATH];
	
	for( UINT count = 0; count < cFiles; count++ )
	{
		if(cFiles>0)
		{
			::DragQueryFile(hDropInfo, count, szFile, sizeof(szFile));
			szText += szFile;       //绝对路径+文件名存于szFile
			GetDlgItem(IDC_EDIT1)->SetWindowText(szText);
		}
	}
}
void error(LPSTR lpszFunction) 
{ 
    CHAR szBuf[80]; 
    DWORD dw = GetLastError(); 
	
    sprintf(szBuf, "%s failed: GetLastError returned %u\n", 
        lpszFunction, dw); 
	
    MessageBox(NULL, szBuf, "Error", MB_OK); 
} 

void SafeCloseHandle(FILE *hHandle)
{
	if (hHandle!=NULL)
	{
		fclose(hHandle);
	} 
}


void GetFileSizeAndName(const TCHAR * szTagFile,TCHAR *szFileName,DWORD *pFileSize)
{
	HANDLE hFile;
	
	if ((hFile = CreateFile(szTagFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0)) == INVALID_HANDLE_VALUE)
	{
		return;
	}
	
	*pFileSize=GetFileSize(hFile,NULL);
	CloseHandle(hFile);
	
}

void AttachFile(const TCHAR *szBind,const TCHAR *szAttach)
{
	
	FILE* fpWrite	= _tfopen(szBind, _T("ab+")); 
	FILE* fpRead	= _tfopen(szAttach,_T("rb"));
	
	if (fpWrite == NULL || fpRead == NULL)	
	{
		
		SafeCloseHandle(fpRead);
		SafeCloseHandle(fpWrite);
		return;
	}
	
	fseek(fpWrite,0,SEEK_END);
	fseek(fpRead,0,SEEK_SET);
	
	
	DWORD c;
	while ((c=fgetc(fpRead))!=EOF)
	{
		fputc(c,fpWrite);
	}
	
	fclose(fpWrite);
	fclose(fpRead);
}
void * GetMemoryFilePoint(const TCHAR* szFile)
{
	HANDLE hFile, hMapping;
	void *pPE;
	
    hFile = CreateFile(szFile,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL) ;
	
	if (hFile==INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	// 创建一个映射文件.
	if (!(hMapping = CreateFileMapping(hFile, 0, PAGE_READWRITE, 0, 0, 0)))
	{
		OutputDebugStringA("CreateMapping Failed.");
		CloseHandle(hFile);
		return NULL;
	}
	
	// 把文件头映象存入 pPE
	if (!(pPE = MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0)))
	{
		OutputDebugStringA("MapViewOfFile Failed.");
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return NULL;
	}
	
	CloseHandle(hMapping);
	CloseHandle(hFile);
	return pPE;
}
void CreateSingedFile(TCHAR szFile[],void *pMemFile,DWORD dwLen)
{
	char * pMemData=(char *)pMemFile;
	
	FILE* fpFile	= _tfopen(szFile, _T("ab+")); 
	
	if (fpFile == NULL)	
	{
		OutputDebugStringA("CreatePartFile Open Me File Error");
		SafeCloseHandle(fpFile);
		UnmapViewOfFile(pMemData);
		return;
	}
	
	DWORD dwSize=0;
	
	for (dwSize;dwSize<dwLen;dwSize++)//File1
	{
		DWORD c=pMemData[dwSize];
		fputc(c,fpFile);
	}
	SafeCloseHandle(fpFile);
}
// void CreateSign()
// {
// 	DWORD dwSize,dwWritten;
// 	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_QQxuanwu),"SIGN");//查找资源
// 	if(hr==NULL) return;
// 	dwSize=SizeofResource(NULL,hr);
// 	HGLOBAL hg=LoadResource(NULL,hr);//加载资源
// 	if(hg==NULL) return;
// 	LPSTR lp=(LPSTR)LockResource(hg);//锁定资源
// 	if(lp==NULL) return;
// 	
// 	HANDLE hFile;
// 	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//创建文件
// 	if(hFile == NULL)	return;
// 	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
// 	CloseHandle(hFile);	
// }


void CSignature::Onxuanwu() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_QQxuanwu),"SIGN");//查找资源
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//加载资源
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//锁定资源
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//创建文件
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);	
}

void CSignature::Onweishi() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_360weishi),"SIGN");//查找资源
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//加载资源
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//锁定资源
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//创建文件
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);	
}

void CSignature::Onkugou() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_kugou),"SIGN");//查找资源
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//加载资源
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//锁定资源
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//创建文件
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);	
}

void CSignature::Onliulanqi() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_liulanqi),"SIGN");//查找资源
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//加载资源
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//锁定资源
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//创建文件
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);	
}

void CSignature::Onxunlei() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_xunlei),"SIGN");//查找资源
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//加载资源
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//锁定资源
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//创建文件
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);
}

void CSignature::Ontengxun() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_tengxun),"SIGN");//查找资源
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//加载资源
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//锁定资源
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//创建文件
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);
}

void CSignature::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
// 	CreateSign();
	
	DWORD dwSingSize=0;
	DWORD dwSingOffe=0;
	TCHAR szText[MAX_PATH]={0};
	
	GetFileSizeAndName(m_path.LockBuffer(),NULL,&dwSingOffe);
	GetFileSizeAndName("C:\\SIGN",NULL,&dwSingSize);
	

	CopyFile(m_path.LockBuffer(),"C:\\TEMP",TRUE);
	
	
	AttachFile("C:\\TEMP","C:\\SIGN");
	
	
	void * pFile=GetMemoryFilePoint("C:\\TEMP");
	
	if (pFile==NULL)
	{
		return;
	}
	
	PIMAGE_DOS_HEADER pImageDosHeader ;
	PIMAGE_NT_HEADERS pImageNtHeaders ;
	
    pImageDosHeader = (PIMAGE_DOS_HEADER)pFile ;
	
/*	AddText("开始检测文件的有效性");*/
// 	m_Log.ReplaceSel( "<- 开始检测文件的有效性...\r\n");
//     if (pImageDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
//     {
// 		UnmapViewOfFile(pFile);
// //		AddText("您选择的文件是无效的");
// 		m_Log.ReplaceSel( "<- 您选择的文件是无效的...\r\n");
// 		DeleteFile("C:\\QQxuanwu.cer");
// 		DeleteFile("C:\\360weishi.cer");
// 		DeleteFile("C:\\liulanqi.cer");
// 		DeleteFile("C:\\kugou.cer");
// 		DeleteFile("C:\\tengxun.cer");
// 		DeleteFile("C:\\xunlei.cer");
// 		DeleteFile("C:\\TEMP");
// 		DeleteFile("C:\\SIGN");
// 		return ;
// 	}
	
// 	pImageNtHeaders = (PIMAGE_NT_HEADERS)((DWORD)pFile+pImageDosHeader->e_lfanew) ;
// 	
// 	if (pImageNtHeaders->Signature!=IMAGE_NT_SIGNATURE)
// 	{
// 		UnmapViewOfFile(pFile);
// 		m_Log.ReplaceSel( "<- 您选择的文件是无效的...\r\n");
// 		DeleteFile("C:\\QQxuanwu.cer");
// 		DeleteFile("C:\\360weishi.cer");
// 		DeleteFile("C:\\liulanqi.cer");
// 		DeleteFile("C:\\kugou.cer");
// 		DeleteFile("C:\\tengxun.cer");
// 		DeleteFile("C:\\xunlei.cer");
// 		DeleteFile("C:\\TEMP");
// 		DeleteFile("C:\\SIGN");
// 		return ;
// 	}
// 	if (pImageNtHeaders->OptionalHeader.DataDirectory[4].Size!=0 || pImageNtHeaders->OptionalHeader.DataDirectory[4].VirtualAddress!=0 )
// 	{
// 		if(MessageBox(_TEXT("这个文件以经有了数字签名 要继续添加吗?"),_TEXT("提示"),MB_YESNO|MB_ICONQUESTION)!=IDYES)
// 		{
// 			UnmapViewOfFile(pFile);
// 	//		AddText("这个文件以经有了数字签名");
// 			m_Log.ReplaceSel( "<- 这个文件以经有了数字签名...\r\n");
// 			DeleteFile("C:\\QQxuanwu.cer");
// 			DeleteFile("C:\\360weishi.cer");
// 			DeleteFile("C:\\liulanqi.cer");
// 			DeleteFile("C:\\kugou.cer");
// 			DeleteFile("C:\\tengxun.cer");
// 			DeleteFile("C:\\xunlei.cer");
// 			DeleteFile("C:\\TEMP");
// 			DeleteFile("C:\\SIGN");
// 			return ;
// 		}
	}
	
//	AddText("开始写入偏移位置和大小");
//	pImageNtHeaders->OptionalHeader.DataDirectory[4].Size=dwSingSize;				//数字签名的大小 
//	pImageNtHeaders->OptionalHeader.DataDirectory[4].VirtualAddress=dwSingOffe;		//数字签名的位置
	
	TCHAR szNewFile[MAX_PATH]={0};
//	lstrcpy(szNewFile,m_path.LockBuffer());
//	lstrcat(szNewFile,"New.exe");
	
//	AddText("创建新的文件");
//	CreateSingedFile(szNewFile,pFile,dwSingOffe+dwSingSize);
//	AddText("添加数字签名成功");
// 	m_Log.ReplaceSel( "<- 添加数字签名成功...\r\n");
// 	UnmapViewOfFile(pFile);
// 	DeleteFile("C:\\QQxuanwu.cer");
// 	DeleteFile("C:\\360weishi.cer");
// 	DeleteFile("C:\\liulanqi.cer");
// 	DeleteFile("C:\\kugou.cer");
// 	DeleteFile("C:\\tengxun.cer");
// 	DeleteFile("C:\\xunlei.cer");
// 
// 
// 	DeleteFile("C:\\TEMP");
// 	DeleteFile("C:\\SIGN");
// }

