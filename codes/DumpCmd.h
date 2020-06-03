//Lab5
//Jin Won Seo
#ifndef DUMPCMD_H
#define DUMPCMD_H

#include "Cmd.h"
#include "ctype.h"

class DumpCmd:public Cmd
{
	
	virtual const char* parse(istream& is, ostream& os);

	public:
	DumpCmd(const char* name):Cmd(name){}
};

const char* DumpCmd::parse(istream& is, ostream& os)
{
	size_t begAddr, endAddr;

	int oldFormat = os.flags();//store old format
	int oldFillChar = os.fill();//store old char fill	

   os << hex;
	is >> hex;
   os.fill('0');

	if(!(is >> begAddr))
		return "need begin address in hex";
	
	if(!(is >> endAddr))
		endAddr = begAddr + 0x40;// offset of end addr


//	os << setfill('0') << setw(2) << hex;//fill 0 in 2 spaces of hex
	/*
	* Display starting address with 0
	* ex) If starting addr is 2ffff then display addr at 2fff0
	*/
	size_t length = 16;

	for(size_t addr = begAddr & 0xfff0; addr < endAddr; addr += length)
	{
//		size_t i=0;

		os << endl;//new line every loop
		os << setw(4) << addr << "  ";//print address

		for(size_t i=0 ; i < length ; i++)
		{
			char *c = reinterpret_cast<char *>(addr + i);
			os << setw(2) << size_t(*c) << " ";
			if((i+1) % 4 == 0 && i != length-1)//print '-' every 4 bits
				os << "- ";		
		}

		os << "| ";//print '|' between ascii and hex
		
		//print ascii
		for(size_t j=0 ; j < length; j++)
		{
			char *cp = reinterpret_cast<char *>(addr + j);
			if(isprint(*cp))//print printable char
				os<< *cp;
			else
				os << ".";		
			
		}		
	}

	os.fill(oldFillChar);//restore old char fill
   os.flags(oldFormat);//restore old format

	return 0;
}
#endif
