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
	GetDlgItem(IDC_EDIT1)->SetWindowText("���ļ��ϵ�������ǩ��....");

}


HBRUSH CSignature::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if ((pWnd->GetDlgCtrlID() == IDC_Log) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(0, 255, 0);
		pDC->SetTextColor(clr);   //���ð�ɫ���ı�
		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);     //���ú�ɫ�ı���
		return CreateSolidBrush(clr);  //��ΪԼ�������ر���ɫ��Ӧ��ˢ�Ӿ��
	}
	else
	{
		return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


void CSignature::OnDropFiles( HDROP hDropInfo )
{
	UINT cFiles = ::DragQueryFile( hDropInfo, 0xffffffff, 0, 0);    //ȡ���ļ���
	CString szText;
	char szFile[MAX_PATH];
	
	for( UINT count = 0; count < cFiles; count++ )
	{
		if(cFiles>0)
		{
			::DragQueryFile(hDropInfo, count, szFile, sizeof(szFile));
			szText += szFile;       //����·��+�ļ�������szFile
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
	// ����һ��ӳ���ļ�.
	if (!(hMapping = CreateFileMapping(hFile, 0, PAGE_READWRITE, 0, 0, 0)))
	{
		OutputDebugStringA("CreateMapping Failed.");
		CloseHandle(hFile);
		return NULL;
	}
	
	// ���ļ�ͷӳ����� pPE
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
// 	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_QQxuanwu),"SIGN");//������Դ
// 	if(hr==NULL) return;
// 	dwSize=SizeofResource(NULL,hr);
// 	HGLOBAL hg=LoadResource(NULL,hr);//������Դ
// 	if(hg==NULL) return;
// 	LPSTR lp=(LPSTR)LockResource(hg);//������Դ
// 	if(lp==NULL) return;
// 	
// 	HANDLE hFile;
// 	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//�����ļ�
// 	if(hFile == NULL)	return;
// 	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
// 	CloseHandle(hFile);	
// }


void CSignature::Onxuanwu() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_QQxuanwu),"SIGN");//������Դ
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//������Դ
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//������Դ
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//�����ļ�
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);	
}

void CSignature::Onweishi() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_360weishi),"SIGN");//������Դ
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//������Դ
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//������Դ
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//�����ļ�
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);	
}

void CSignature::Onkugou() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_kugou),"SIGN");//������Դ
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//������Դ
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//������Դ
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//�����ļ�
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);	
}

void CSignature::Onliulanqi() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_liulanqi),"SIGN");//������Դ
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//������Դ
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//������Դ
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//�����ļ�
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);	
}

void CSignature::Onxunlei() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_xunlei),"SIGN");//������Դ
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//������Դ
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//������Դ
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//�����ļ�
	if(hFile == NULL)	return;
	WriteFile(hFile,(LPCVOID)lp,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);
}

void CSignature::Ontengxun() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSize,dwWritten;
	HRSRC hr=FindResource(0,MAKEINTRESOURCE(IDR_tengxun),"SIGN");//������Դ
	if(hr==NULL) return;
	dwSize=SizeofResource(NULL,hr);
	HGLOBAL hg=LoadResource(NULL,hr);//������Դ
	if(hg==NULL) return;
	LPSTR lp=(LPSTR)LockResource(hg);//������Դ
	if(lp==NULL) return;
	
	HANDLE hFile;
	hFile = CreateFile("C:\\SIGN",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);//�����ļ�
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
	
/*	AddText("��ʼ����ļ�����Ч��");*/
// 	m_Log.ReplaceSel( "<- ��ʼ����ļ�����Ч��...\r\n");
//     if (pImageDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
//     {
// 		UnmapViewOfFile(pFile);
// //		AddText("��ѡ����ļ�����Ч��");
// 		m_Log.ReplaceSel( "<- ��ѡ����ļ�����Ч��...\r\n");
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
// 		m_Log.ReplaceSel( "<- ��ѡ����ļ�����Ч��...\r\n");
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
// 		if(MessageBox(_TEXT("����ļ��Ծ���������ǩ�� Ҫ���������?"),_TEXT("��ʾ"),MB_YESNO|MB_ICONQUESTION)!=IDYES)
// 		{
// 			UnmapViewOfFile(pFile);
// 	//		AddText("����ļ��Ծ���������ǩ��");
// 			m_Log.ReplaceSel( "<- ����ļ��Ծ���������ǩ��...\r\n");
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
	
//	AddText("��ʼд��ƫ��λ�úʹ�С");
//	pImageNtHeaders->OptionalHeader.DataDirectory[4].Size=dwSingSize;				//����ǩ���Ĵ�С 
//	pImageNtHeaders->OptionalHeader.DataDirectory[4].VirtualAddress=dwSingOffe;		//����ǩ����λ��
	
	TCHAR szNewFile[MAX_PATH]={0};
//	lstrcpy(szNewFile,m_path.LockBuffer());
//	lstrcat(szNewFile,"New.exe");
	
//	AddText("�����µ��ļ�");
//	CreateSingedFile(szNewFile,pFile,dwSingOffe+dwSingSize);
//	AddText("�������ǩ���ɹ�");
// 	m_Log.ReplaceSel( "<- �������ǩ���ɹ�...\r\n");
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

