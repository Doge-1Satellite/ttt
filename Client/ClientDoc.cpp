﻿// ClientDoc.cpp : implementation of the CClientDoc class
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientDoc

IMPLEMENT_DYNCREATE(CClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CClientDoc, CDocument)
	//{{AFX_MSG_MAP(CClientDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDoc construction/destruction

CClientDoc::CClientDoc()
{
	// TODO: add one-time construction code here

}

CClientDoc::~CClientDoc()
{
}

//DEL BOOL CClientDoc::OnNewDocument()
//DEL {
//DEL 	if (!CDocument::OnNewDocument())
//DEL 		return FALSE;
//DEL 
//DEL 	// TODO: add reinitialization code here
//DEL 	// (SDI documents will reuse this document)
//DEL 	return TRUE;
//DEL }



/////////////////////////////////////////////////////////////////////////////
// CClientDoc serialization

void CClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CClientDoc diagnostics

#ifdef _DEBUG
void CClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CClientDoc commands
