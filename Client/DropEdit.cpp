﻿/////////////////////////////////////////////////////////////////////////////
//
//	CDropEdit
//	Copyright 1997 Chris Losinger
//  Download by http://www.NewXing.com
//	This code is freely distributable and modifiable, as long as credit
//	is given to where it's due. Watch, I'll demonstrate :
//
//	shortcut expansion code modified from :
//	CShortcut, 1996 Rob Warner
//
////
//
//	To use this in an app, you'll need to :
//
//	1) #include <afxole.h> in stdafx.h
//	
//	2) in your CWinApp-derived class *::InitInstance, you'll need to call
//		::CoInitialize(NULL);
//
//	3) in your CWinApp-derived class *::ExitInstance, you'll need to call
//	::CoUninitialize();
//
//	4) Place a normal edit control on your dialog. 
//	5) Check the "Accept Files" property.
//
//	6) In your dialog class, declare a member variable of type CDropEdit
//	(be sure to #include "CDropEdit.h")
//		ex. CDropEdit m_dropEdit;
//
//	7) In your dialog's OnInitDialog, call
//		m_dropEdit.SubclassDlgItem(IDC_YOUR_EDIT_ID, this);
//
//	8) if you want the edit control to handle directories, call
//		m_dropEdit.SetUseDir(TRUE);
//
//	9) if you want the edit control to handle files, call
//		m_dropEdit.SetUseDir(FALSE);
//
//	that's it!
//
//	This will behave exactly like a normal edit-control but with the 
//	ability to accept drag-n-dropped files (or directories).
//
//


#include "stdafx.h"
#include "resource.h"
#include "DropEdit.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDropEdit

CDropEdit::CDropEdit()
{
	m_bUseDir=FALSE;
}

CDropEdit::~CDropEdit()
{
}


BEGIN_MESSAGE_MAP(CDropEdit, CEdit)
	//{{AFX_MSG_MAP(CDropEdit)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropEdit message handlers

int CDropEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DragAcceptFiles(TRUE);
	
	return 0;
}

//
//	handle WM_DROPFILES
//

void CDropEdit::OnDropFiles(HDROP dropInfo)
{
	// Get the number of pathnames that have been dropped
	WORD wNumFilesDropped = DragQueryFile(dropInfo, -1, NULL, 0);

	CString firstFile="";

	// get all file names. but we'll only need the first one.
	for (WORD x = 0 ; x < wNumFilesDropped; x++) {

		// Get the number of bytes required by the file's full pathname
		WORD wPathnameSize = DragQueryFile(dropInfo, x, NULL, 0);

		// Allocate memory to contain full pathname & zero byte
		char * npszFile = (char *) LocalAlloc(LPTR, wPathnameSize += 1);

		// If not enough memory, skip this one
		if (npszFile == NULL) continue;

		// Copy the pathname into the buffer
		DragQueryFile(dropInfo, x, npszFile, wPathnameSize);

		// we only care about the first
		if (firstFile=="")
			firstFile=npszFile;

		// clean up
		LocalFree(npszFile);
	}

	// Free the memory block containing the dropped-file information
	DragFinish(dropInfo);

	// if this was a shortcut, we need to expand it to the target path
	CString expandedFile = ExpandShortcut(firstFile);

	// if that worked, we should have a real file name
	if (expandedFile!="") 
		firstFile=expandedFile;

	
	struct _stat buf;
	// get some info about that file
	int result = _stat( firstFile, &buf );
	if( result == 0 ) {

		// verify that we have a dir (if we want dirs)
		if ((buf.st_mode & _S_IFDIR) == _S_IFDIR) {
			if (m_bUseDir)
				SetWindowText(firstFile);

		// verify that we have a file (if we want files)
		} else if ((buf.st_mode & _S_IFREG) == _S_IFREG) {
			if (!m_bUseDir)
				SetWindowText(firstFile);
		}
	}
}

//////////////////////////////////////////////////////////////////
//	use IShellLink to expand the shortcut
//	returns the expanded file, or "" on error
//
//	original code was part of CShortcut 
//	1996 by Rob Warner
//	rhwarner@southeast.net
//	http://users.southeast.net/~rhwarner

CString CDropEdit::ExpandShortcut(CString &inFile)
{
	CString outFile = "";

    // Make sure we have a path
    ASSERT(inFile != _T(""));

    IShellLink* psl;
    HRESULT hres;
    LPTSTR lpsz = inFile.GetBuffer(MAX_PATH);

    // Create instance for shell link
    hres = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
        IID_IShellLink, (LPVOID*) &psl);
    if (SUCCEEDED(hres))
    {
        // Get a pointer to the persist file interface
        IPersistFile* ppf;
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*) &ppf);
        if (SUCCEEDED(hres))
        {
            // Make sure it's ANSI
            WORD wsz[MAX_PATH];
            ::MultiByteToWideChar(CP_ACP, 0, lpsz, -1, wsz, MAX_PATH);

            // Load shortcut
            hres = ppf->Load(wsz, STGM_READ);
            if (SUCCEEDED(hres)) {
				WIN32_FIND_DATA wfd;
				// find the path from that
				HRESULT hres = psl->GetPath(outFile.GetBuffer(MAX_PATH), 
								MAX_PATH,
								&wfd, 
								SLGP_UNCPRIORITY);

				outFile.ReleaseBuffer();
            }
            ppf->Release();
        }
        psl->Release();
    }

	inFile.ReleaseBuffer();

	// if this fails, outFile == ""
    return outFile;
}
