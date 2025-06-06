// GeneParams.cpp : implementation file
//

#include "stdafx.h"
#include "snd.h"
#include "GeneParams.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneParams dialog


CGeneParams::CGeneParams(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneParams::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeneParams)
	m_GeneAmpl = 0;
	m_GeneFreq = 0;
	//}}AFX_DATA_INIT
}


void CGeneParams::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneParams)
	DDX_Text(pDX, IDC_GENE_AMPL, m_GeneAmpl);
	DDV_MinMaxUInt(pDX, m_GeneAmpl, 0, 32000);
	DDX_Text(pDX, IDC_GENE_FREQ, m_GeneFreq);
	DDV_MinMaxUInt(pDX, m_GeneFreq, 0, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneParams, CDialog)
	//{{AFX_MSG_MAP(CGeneParams)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneParams message handlers
