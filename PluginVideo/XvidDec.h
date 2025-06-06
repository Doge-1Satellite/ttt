#ifndef _XVID_DECODE_H
#define _XVID_DECODE_H
 
#include "xvid.h"
 
#define MY_TEST 
class CXvidDecHandler {
public:
	virtual void PostDecHandler(unsigned char * image, int used_bytes) = 0 ; 
};

class CXvidDec {

public:
	CXvidDec() ;
	~CXvidDec() ; 
	bool               Open();
	bool               Close();
	void               Decode(unsigned char* xvid, int xvid_len);
	static void		   XVID_GLOBAL_INIT();
	void			   AttachCaller(int width, int height, CXvidDecHandler * dec_caller) ; 

protected:
	int                dec_core(unsigned char *bitstream, unsigned char *image, int bs_size) ;

protected:
	CXvidDecHandler*  m_dec_caller ; 
	void*             m_dec_handle ;
	unsigned char *   m_image ;
	int				  m_width ; 
	int				  m_height ; 
};
#endif
