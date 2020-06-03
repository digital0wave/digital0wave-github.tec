//Lab12
//Jin Won Seo
#ifndef APPCMDPROC_H
#define APPCMDPROC_H

#include "new.h"
#include "pwm.h"
#include "Task.h"
#include "Servo.h"
#include "PwmCmd.h"
#include "HelloCmd.h"
#include "DumpCmd.h"
#include "CmdProc.h"
#include "HC12Rti.h"
#include "LTC1404.h"
#include "ServoConfigCmd.h"

class AppCmdProc: public Task,public CmdProc
{
	HC12Rti& rti;
	LTC1404& ltc;
	PwmChannel& pwmTec;
	PwmChannel& pwmRes;
	Servo& servo;
	
	size_t sz;
	uint16 pri;
	streambuf* sb;
	
	
	virtual void main()
	{

		Cmd* menu[] = {
			new HelloCmd("hello"), 
			new DumpCmd("dump"),
			new HC12RtiCmd("rti", rti),
			new LTC1404Cmd("adc", ltc), 
			new PwmCmd("pwm", pwmTec, pwmRes),
			new ServoConfigCmd("servo", servo),
			new ServoListCmd("disp", sb, servo, sz, pri),
			0
		};
		CmdProc::present(menu);
		
		for(Cmd** p=&menu[0]; *p; p++)
			delete *p; //call deconstructor

	}

	public:
	AppCmdProc(streambuf* sbuf, HC12Rti& hc12rti, LTC1404& ltc1404 , PwmChannel& tec,  PwmChannel& res, Servo& sv, size_t stackSize, uint16 priorityCmd, uint16 priServoList )
	:Task(stackSize,priorityCmd), CmdProc(sbuf, "TEC >"), rti(hc12rti) , ltc(ltc1404), pwmTec(tec), pwmRes(res), servo(sv), sz(stackSize), pri(priServoList), sb(sbuf)
	{ 
		fork(); 
	}
	virtual ~AppCmdProc(){}
	
};

#endif
