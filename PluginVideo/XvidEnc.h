#pragma once
#ifndef _XVID_ENCODE_H
#define _XVID_ENCODE_H
#include "xvid.h"

#define MY_TEST
#define MAX_ENC_BUF   1024*100

class CXvidEncHandler {
public:
	virtual void PostEncHandler(unsigned char * xvid, int key, int xvid_len) = 0 ; 
};

class CXvidEnc {
protected:
	// original encode routine in xvid lib
	int			enc_core(unsigned char *image, unsigned char *bitstream, int *key);

public:
	CXvidEnc() ; 
	bool            Open();
	bool            Close();
	static void     XVID_GLOBAL_INIT();
	void            Encode(unsigned char * image);
	void			AttachCaller(int width, int height, CXvidEncHandler * enc_caller) ; 
protected:
	CXvidEncHandler* m_enc_caller ; 
	void *           m_enc_handle;
	unsigned char    m_bitstream[MAX_ENC_BUF];
	bool             m_closed; 
	int			     m_key ;
	int				 m_width ; 
	int				 m_height ; 
};
#endif