// Buffer.h: interface for the CBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFER_H__829F6693_AC4D_11D2_8C37_00600877E420__INCLUDED_)
#define AFX_BUFFER_H__829F6693_AC4D_11D2_8C37_00600877E420__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBuffer  
{
// Attributes
protected:
	PBYTE	m_pBase;
	PBYTE	m_pPtr;
	UINT	m_nSize;

// Methods
protected:
	UINT ReAllocateBuffer(UINT nRequestedSize);
	UINT DeAllocateBuffer(UINT nRequestedSize);
	UINT GetMemSize();

public:
	CBuffer();
	virtual ~CBuffer();

	void  ClearBuffer();
	void  ReallyClearBuffer();
	UINT  Delete(UINT nSize);
	UINT  Read(PBYTE pData, UINT nSize);
	BOOL  Write(PBYTE pData, UINT nSize);
	BOOL  Write(CString& strData);
	BOOL  Insert(PBYTE pData, UINT nSize);
	BOOL  Insert(CString& strData);
	PBYTE GetBuffer(UINT nPos = 0);
	UINT  GetBufferLen();
	int   Scan(PBYTE pScan, UINT nPos);
	void  Copy(CBuffer& buffer);	
	void  FileWrite(const CString& strFileName);

};

#endif // !defined(AFX_BUFFER_H__829F6693_AC4D_11D2_8C37_00600877E420__INCLUDED_)
