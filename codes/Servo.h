//lab12
//Jin Won Seo
#ifndef SERVO_H
#define SERVO_H

#include "stddef.h"
#include "string.h"
#include "iomanip.h"
#include "iostream.h"

#include "pwm.h"
#include "Task.h"
#include "LTC1404.h"
#include "TimerTask.h"

class Servo : public Task 
{
public:
	struct Config
	{
		float setpoint;
		float pGain;
		float iGain;
		float dGain;
		float iLimit;
		
		Config(float sp, float pg, float ig, float dg, float il) : setpoint(sp), pGain(pg), iGain(ig), dGain(dg), iLimit(il) {}	
		
	};//end Config

	struct Status
	{
		float feedback;//Temperature
		float error;//current error 
		float prevError;//for derivative
		float iError;// I accumulated error 
		float dError;// derror/dt *cycle
		float pTerm; // pGain * error 
		float iTerm;
		float dTerm;
		float response; //TEC driver
		
		Status() : feedback(0), error(0), prevError(0), iError(0), dError(0), pTerm(0), iTerm(0), dTerm(0), response(0) {}
	};
	//ctor
	Servo(LTC1404& ltc, PwmChannel& pChan, size_t stackSize, uint16 priority, float sp, float pg, float ig, float dg, float il) 
	: Task(stackSize,priority), adc(ltc), pwm(pChan), config(sp, pg, ig, dg, il)
	{
		fork(); 
	}
	//get copy of config obj for display
	operator Config() const
	{
		Mutex::Lock lock(mutex);
		return config;
	}

	//set new config
	Config& operator = ( const Config& c)
	{
		Mutex::Lock lock(mutex);
		return (config = c);
	}
	
		//get copy of status obj for display
	operator Status() const
	{
		Mutex::Lock lock(mutex);
		return status;
	}

	//set new status
	Status& operator = ( const Status& st)
	{
		Mutex::Lock lock(mutex);
		return (status = st);
	}
	
private:
	LTC1404& adc;
	PwmChannel& pwm;
	mutable Mutex mutex;
	Config config;
	Status status;
	
	void main()
	{
		while(true)
		{
			TimerTask::delay(1000);
			Mutex::Lock lock(mutex);
			//get feedback
			float filt = 0;
			filt = (adc.read())*100;//read temperature
			status.feedback = filt;
			//calculate error, derror/dt, integral dt
			status.prevError = status.error;// store current error to previous error
			status.error = filt - config.setpoint;//subtract filt temp by setpoint 
			status.dError = status.error - status.prevError; //differences b/w pre and current error
			status.iError += status.error;//accumulated error 
			
			status.pTerm = status.error * config.pGain;// error * pGain
			
			//config.iGain = (config.iGain < config.iLimit) ? config.iGain : config.iLimit;//regulate integral gain. less than limit
			status.iTerm = status.iError * config.iGain;// accumulated error * integral gain(iGain)
			
			status.dTerm = status.dError * config.dGain;//error difference * derivative gain (dGain)
			
			status.response =  status.pTerm + status.iTerm + status.dTerm;
			//sum terms, drive tec
										
			pwm.setDuty(status.response);
		}//end while
	}//end main
};//end Servo

//display config data
inline ostream& operator << (ostream& os, const Servo::Config& c)
{
	return os << "setpoint=" << c.setpoint <<
	" " << "pGain=" << c.pGain <<
	" " << "iGain=" << c.iGain <<
	" " << "dGain=" << c.dGain <<
	" " << "iLimit=" << c.iLimit << endl;
}

//display status data
inline ostream& operator << (ostream& os, const Servo::Status& s)
{
	return os << "feedback=" << s.feedback <<
	" " << "error=" << s.error <<
	" " << "prevError=" << s.prevError <<
	" " << "iError=" << s.iError <<
	" " << "dError=" << s.dError <<
	" " << "pTerm=" << s.pTerm <<	
	" " << "iTerm=" << s.iTerm <<
	" " << "dTerm=" << s.dTerm <<	
	" " << "response=" << s.response << '\r';	
}
#endif
