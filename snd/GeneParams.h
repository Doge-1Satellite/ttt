#if !defined(AFX_GENEPARAMS_H__27EF4E02_B2CD_11D2_8045_004005E3B23E__INCLUDED_)
#define AFX_GENEPARAMS_H__27EF4E02_B2CD_11D2_8045_004005E3B23E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GeneParams.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneParams dialog

class CGeneParams : public CDialog
{
// Construction
public:
	CGeneParams(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGeneParams)
	enum { IDD = IDD_GENE_PARAMS };
	UINT	m_GeneAmpl;
	UINT	m_GeneFreq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneParams)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGeneParams)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENEPARAMS_H__27EF4E02_B2CD_11D2_8045_004005E3B23E__INCLUDED_)
