//hc12sci.h
//lab4
//Jin Won Seo

#ifndef HC12SCIBUF_H
#define HC12SCIBUF_H

#include "chipio.h"
#include "SciBuf.h"
//#include "stddef.h"

class HC12SciBuf : public SciBuf
{

	volatile sciport& sci;
	virtual void send(char c);
	virtual char recv();
	virtual bool avail();
/*
	void hc12send(char c);
	char hc12recv();
	bool hb12avail();
*/

	public:
	HC12SciBuf(size_t rxLen, size_t txLen, volatile sciport& sp);	
	
	virtual ~HC12SciBuf();

};

#endif
