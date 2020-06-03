//Lab5
//Jin Won Seo
#ifndef HELLOCMD_H
#define HELLOCMD_H

#include "Cmd.h"

class HelloCmd:public Cmd
{
	virtual const char* parse(istream&, ostream& os);

	public:
	HelloCmd(const char* name):Cmd(name){}
};

const char* HelloCmd::parse(istream&, ostream& os)
{
	os << endl << "Hello, World!";
	return 0; //no error
}
#endif
