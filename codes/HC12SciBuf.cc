//hc12sci.cc
//lab4
//Jin Won Seo

#include "Task.h"
#include "HC12SciBuf.h"

HC12SciBuf::HC12SciBuf(size_t rxLen, size_t txLen, volatile sciport& sp)
:SciBuf(rxLen, txLen),sci(sp)
{	
	//sci control2
	sci.sccr2 |=(sci.te |sci.re);//enable reciever and transmitter
}

HC12SciBuf::~HC12SciBuf()
{
	sci.sccr2 &= ~(sci.te | sci.re);//disable receiver and transmitter
}

bool HC12SciBuf::avail()
{
	//sci status 1
	return sci.scsr1 & sci.rdrf;//receive data register full
}

char HC12SciBuf::recv()
{
	while(!avail()) { Task::yield(); }
	return sci.scdrl;//data reg low
}

void HC12SciBuf::send(char c)
{
	while(!(sci.scsr1 & sci.tdre)) { Task::yield(); }//transmit data register empty
	sci.scdrl=c;//put char into data reg low
}

