//lab6
//Jin Won Seo
#ifndef ISR_H
#define ISR_H

#include "stddef.h"

class Isr
{
	//compiler expects a class declaration
	class Ictlr& ictlr;//controller that is attached to
	uint16 index;//index in my controller's table
	friend class Ictlr;
	virtual void handler()=0;//pure virtual
	
	protected:
	Isr(Ictlr&, uint16 index);
	virtual ~Isr();
};

class Ictlr
{
	uint16 length;//# of entries in table
	Isr** isrTable;

	protected:
	void runHandler(uint16 index)
	{
		if(isrTable[index])
			isrTable[index]->handler();
		else
			while(1);//do something about "wild" interrupt
	}

	public:
	Ictlr(uint16 len);
	virtual ~Ictlr();
	void attach(Isr*);//add Interrupt Service Routine to table
	void detach(Isr*);//remove ISR from table

};

#endif
