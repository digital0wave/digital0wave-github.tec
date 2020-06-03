//lab6
//Jin Won Seo

#include "Isr.h"
#include "chipio.h"

//Isr constructor
Isr::Isr(Ictlr& ic, uint16 i)
:ictlr(ic), index(i)
{
	ictlr.attach(this);
//	ddrdlc |= (1 << 5);		// DEBUG
}

//Isr de-constructor
Isr::~Isr()
{
	ictlr.detach(this);
}

//Ictlr constructor
Ictlr::Ictlr(uint16 len)
:length(len), isrTable(new Isr*[length])
{
	for(size_t i =0; i<length; i++)
	{
		isrTable[i] = 0;
	}
//	ddrdlc |= (1 << 5);		// DEBUG
}

//Ictlr de-constructor
Ictlr::~Ictlr()
{
	delete[] isrTable;
}
//add Isr to the table
void Ictlr::attach(Isr* isr)
{
	if(isr->index < length)
		isrTable[isr->index] = isr;
	//else is not necessary
}
//remove Isr from the table
void Ictlr::detach(Isr* isr)
{
	if(isr->index < length)
		isrTable[isr->index]=0;
}
