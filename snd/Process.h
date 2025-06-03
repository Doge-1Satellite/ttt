// Process.h: interface for the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESS_H__B8CBE642_39B5_11D2_8045_BC0310C10374__INCLUDED_)
#define AFX_PROCESS_H__B8CBE642_39B5_11D2_8045_BC0310C10374__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "soundin.h"

/*------------------------------------------------------------
     This class is the top class for all the sound process

        CProcess
	       XCorr
		      CFft
			    CFilter
			       CSound

	 Only the adresse of the function ComputeSamples() is passed
	 to the Sound Thread by the param "this"
     every class has his own vitual function and must
	 call his base class first.
-------------------------------------------------------------*/
class CProcess : public CSoundIn  
{
public:
	CProcess();
	virtual ~CProcess();
	virtual void ComputeSamples(SHORT *);

};

#endif // !defined(AFX_PROCESS_H__B8CBE642_39B5_11D2_8045_BC0310C10374__INCLUDED_)
