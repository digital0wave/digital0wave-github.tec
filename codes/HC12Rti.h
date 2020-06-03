//lab 6
//Jin Won Seo

#ifndef HC12RTI_H
#define HC12RTI_H

#include "Isr.h"
#include "Cmd.h"
#include "chipio.h"
#include "TimerTask.h"
#include "hc12ictlr.h"
#include "globalmask.h"

class HC12Rti:public Isr, public TimerTask
{
	ulong32 ticks;// msec since startup
	virtual void handler();	

	public:
	HC12Rti(Ictlr& ictlr, size_t stackSize, uint16 priority)
	:Isr(ictlr, HC12Ictlr::isRti),TimerTask(stackSize,priority),ticks(0)
	{
		rtictl =rti | rtr1;//rti1:1.024 msec, rti:interrupt mask/flag
		fork();
	}
	
	//this is a free function, not a method
	friend ostream& operator << (ostream& os, const HC12Rti& hc12rti)
	{
		ulong32 ticks = 0;// tick count
		{
			GlobalMask mask; //globally masked
			ticks = hc12rti.ticks;
		}//confine the scope for mask locally
		return os << endl << ticks;
	}
};

void HC12Rti::handler()
{
		rtiflg = rti;//acknowledge interrupt, reset FF
		++ticks;//count msec
		tick();//notify TimerTask of passage of 1 msec
}

class HC12RtiCmd:public Cmd
{
	HC12Rti& hc12rti;
	virtual const char* parse(istream&, ostream& os);
	
	public:
	HC12RtiCmd(const char* name,HC12Rti& rti):Cmd(name),hc12rti(rti){}
};

const char* HC12RtiCmd::parse(istream&, ostream& os)
{
	os << hc12rti;
	return 0; //no error
}
#endif
