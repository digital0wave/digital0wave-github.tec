//Lab5
//Jin Won Seo
#ifndef CMDPROC_H
#define CMDPROC_H

#include "Cmd.h"
#include "ctype.h"
#include "strstream.h"
//#include "stddef.h"
//#include "iostream.h"

class CmdProc
{
	streambuf* sbuf;//keep the order as these members are initialized
	const char* prompt;

	void getLine(char* buf, size_t len);
	
	protected:
	void present(Cmd* pMenu[]);
	
	public:
	// called by AppCmdProc with streambuf* that is initialized main() and "tec >" 
	CmdProc(streambuf* sb, const char* p):sbuf(sb), prompt(p){}
};

void CmdProc::present(Cmd* pMenu[])
{

	ostream os(sbuf);	
	char buf[80];

	while(true)
	{
		// first time call displays "tec >" then later displays whatever commands come from inputstream
		os << endl << prompt << flush;//call streambuf sync()
		getLine(buf, sizeof(buf));// get one command line from streambuf

		//if 1st element of buffer filled with zero, then skip since it's end
		if(buf[0] !=0)
		{
			const char* msg="unknown command";	//default error message
			for(Cmd** p(pMenu); *p; p++)
			{
				istrstream iss(buf);
				//iss>>setw(sizeof(buf))>>buf;
				if((*p) -> match(iss))//this pointer
				{
					msg = (*p) -> parse(iss, os);
					break;
				}//end if
			}//end for

			//after complete loop and nothing matches then error
			if(msg)
				os << endl << "Error: " << msg;
		}//end if
	}//end while
}

void CmdProc::getLine(char* buf, size_t len)
{
  istream is(sbuf);
  ostream os(sbuf);

  char* bp = &buf[0];
  char* ep = &buf[len - 1];

  while (true)
  {
    int c = is.get();
    switch (c)
    {
      case '\r':
      case '\n':
        *bp = 0;
        return;

      case 'H' - 0x40:
        if (bp > buf)
        {
          os << "\b \b" << flush;
          --bp;
        }
        break;

      default:
        if (isprint(c) && bp < ep)
        {
          os << char(c) << flush;
          *bp++ = c;
        }
        break;
    }
  }
}

#endif
