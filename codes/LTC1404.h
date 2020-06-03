//lab10
//Jin Won Seo

#ifndef LTC1404_H
#define LTC1404_H

#include "Spi.h"
#include "Cmd.h"
#include "NoCopy.h"
#include "Task.h"
#include "chipio.h"
#include "stddef.h"
#include "TestPin.h"
#include "string.h"
#include "iomanip.h"
#include "iostream.h"
#include "TimerTask.h"
#include "globalmask.h"

class LTC1404 : public NoCopy, public Task
{
	//hardware
	Spi& spi;
	static const int selMask = 1 << 7; //chip select in port s
	static const int offset = 2; //offset of data in output, A/D converter output format(first 2 bits unsed)
	static const int validBits = (~0 << (16 - 12)); //valid bits after moving by offset, 0xfff0
	static const int jamBits = (~validBits + 1) >> 1;//Least Significant bits -1  in output
	static const int filtConst = 128;
	static const float vref = 2.048; //reference volt to a/d converter
	mutable Mutex mutex;  //modifiable for const vars
	int raw;//latest raw data measurement fro a/d
	int filt;//filtered measurement

	class Session : public Spi::Session
	{
	public:
		Session(Spi &spi) : Spi::Session(spi)
		{
			ports |= selMask;//chipio variable
		}
		~Session()
		{
			ports &= ~selMask;//unmask
		}
		int measure();
	};//end of class Session

	friend ostream& operator << (ostream& os, const LTC1404& ltc1404)
	{
		int raw = 0;
		int filt =0;
		{
			GlobalMask mask;
			raw = ltc1404.raw;
			filt = ltc1404.filt;
		}
		return os << endl << "raw = " << raw << " |" << " filt = " << filt << " |" << " Temp = " << (ltc1404.read())*100 <<"C";
	}


protected:
	virtual void main();//for filitering

public:
	LTC1404(size_t stackSize, uint16 priority, Spi& s);
	virtual ~LTC1404();
	float read() const;
};

int LTC1404::Session::measure()
{
	int buf;
	
	get(&buf, sizeof(buf) * 8);
	buf = buf << offset;//kill irrelvant high bits
	buf &= validBits;//surpress low bits
	buf |= jamBits;

	return buf;
}

LTC1404::LTC1404(size_t stackSize, uint16 priority, Spi& s) : Task(stackSize, priority), spi(s), raw(0), filt(0)
{
	{
		GlobalMask mask;
		ddrs |= selMask;//make CS on o/p
	}

	filt = Session(spi).measure();  //scope in one line
	filt = Session(spi).measure();  //scope in one line
//	filt = 11;
	fork();	
}

LTC1404::~LTC1404()
{
	//join() or stop the task
	GlobalMask mask;
	ddrs &= ~selMask;
}

void LTC1404::main()
{
	while(true)
	{
	TestPin tp(portdlc, ddrdlc, 2);//for test
	tp.raise();//for test
		TimerTask::delay(10);
		Mutex::Lock lock(mutex);
		raw = Session(spi).measure();
		filt = filt + (raw - filt) / filtConst;//IIR digital filter
		//os << "LTC1404:main() filt =" << filt << endl;
	tp.lower();//for test
	}
}


float LTC1404::read() const
{
	Mutex::Lock lock(mutex);
	return (filt / 32768.0) * vref;  //convert to float point, .0 internally considered as float(promote)
}

class LTC1404Cmd : public Cmd
{
	LTC1404& ltc1404;
	virtual const char* parse(istream&, ostream& os);
	
	public:
	LTC1404Cmd(const char* name,LTC1404& ltc):Cmd(name),ltc1404(ltc){}
};

const char* LTC1404Cmd::parse(istream&, ostream& os)
{
	os << ltc1404;
	return 0; //no error
}


#endif


