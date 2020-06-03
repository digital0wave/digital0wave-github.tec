//lab 10
//Jin Won Seo

#ifndef HC12SPI_H
#define HC12SPI_H

#include "Spi.h"
#include "stddef.h"
#include "NoCopy.h"
#include "chipio.h"

class HC12Spi : public Spi
{
private:
	volatile spiport& port; // chipio

	virtual void put(const void* buf, int bits)
	{
		const char* p = static_cast< const char* > (buf);
		for(int i = 0; i < bits/8; i++)
		{
			port.spdr = *p++;//spdr:spi data
			//wait until complete
			while(port.spsr & port.spif == 0);//spsr: spi status, spif:transfer complete
		}
	}

	virtual void get(void* buf, int bits)
	{
		char* p = static_cast<char*>(buf);
		for(int i = 0; i < bits/8; i++)
		{
			port.spdr = 0;
			while((port.spsr & port.spif) == 0);
			*p++ = port.spdr;
		}
	}

public:
	HC12Spi(volatile spiport &s) : port(s)
	{
		ddrs |= 0xE0; // set up port direction for ss, sck, mosi
		//spe:enable spi, mstr:master mode, cpha:clock phase = sample center
		port.spcr1 = port.spe | port.mstr | port.cpha; //spcr1:spi control 1
	}

	virtual ~HC12Spi()
	{
		port.spcr1 = port.cpha;//disable
	}
};

#endif


