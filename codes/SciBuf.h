//sci12buf.h
//Lab4
//Jin Won Seo

#ifndef SCIBUF_H
#define SCIBUF_H

#include "stddef.h"
#include "iostream.h"
#include "malloc.h"

class SciBuf:public streambuf
{

	char* rxBuf;
	char* rxEnd;
	char* txBuf;
	char* txEnd;// pointers to rxbuffer and txbuffer
//		size_t rxLen;
//		size_t txLen;

	virtual void send(char c)=0;//send 1 char out port
	virtual char recv()=0;//wait and recv 1 char from port
	virtual bool avail()=0;
	
	protected:
	virtual int sync();
	virtual int overflow(int);
	virtual int underflow();

	public:
	SciBuf(size_t , size_t);
	virtual ~SciBuf();

};

#endif
