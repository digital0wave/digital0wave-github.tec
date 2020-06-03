//Lab5
//Jin Won Seo
#ifndef CMD_H
#define CMD_H

#include "stddef.h"
#include "string.h"
#include "iomanip.h"
#include "iostream.h"

class Cmd
{
	const char* name;
	
	virtual const char* parse(istream&, ostream&)=0; //pure virtual func
	bool match(istream& is);
	
	friend class CmdProc; //CmdProc has access to private members in Cmd
	public:
	Cmd(const char* nm):name(nm){}
	virtual ~Cmd(){};
	
	
};

bool Cmd::match(istream& is)
{
	char buf[20];	//temp buffer
	return (is >> setw(sizeof(buf)) >> buf) && (strcmp(buf,name)==0);
}


#endif
