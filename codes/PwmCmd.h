//Lab11
//Jin Won Seo
#ifndef PWMCMD_H
#define PWMCMD_H

#include "Cmd.h"
#include "pwm.h"
#include "CmdProc.h"

class PwmCmd : public Cmd
{
	
	PwmChannel& pwmTec;
	PwmChannel& pwmRes;
	
	virtual const char* parse(istream&, ostream& os);
		
	public:
	PwmCmd(const char* name,PwmChannel& tec, PwmChannel& res):Cmd(name),pwmTec(tec),pwmRes(res) {}
};


const char* PwmCmd::parse(istream& is, ostream& os)
{
	char str[10];//string
	float duty = 0;
	
	is >> str;
	is >> duty;
	
//	os << endl << "d" << duty<< "str" << str;

	if(!str)
		os << endl << "No command specified: get or set ";
	
	if(strcmp(str, "get") == 0)
	{
		os << endl << "TEC Duty : " << pwmTec.getDuty() << " |" << " Res Duty : " << pwmRes.getDuty();	//get duty cycle of tec/res
	}else
	{
	
		if(!(duty))
		{
			os << endl << "No Duty Cycles Specified  or No matching command !";
			return 0;
		}
		
		if (strcmp(str, "tec") == 0)
		{
			pwmTec.setDuty(duty);		
			os << endl << "set duty TEC :" << duty;

		}else if (strcmp(str, "res") == 0)
		{
			os << endl << "set duty resistor :" << duty;
			pwmRes.setDuty(duty);
		}else
		{
			os << endl << "No command matching with get or set ";
		}		

	}
		
	return 0; //no error
}



#endif