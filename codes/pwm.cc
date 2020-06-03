// $Id: pwm.cc,v 1.3 2004/10/25 20:26:58 querbach Exp $

// pwm.cc			   Copyright (C) 2003, Real-Time Systems Inc.
//-------------------------------------------- All Rights Reserved ----------
//
//	PWM Driver
//
//---------------------------------------------------------------------------

#include "globalmask.h"
#include "chipio.h"
#include "hc12ictlr.h"
#include "pwm.h"
#include "TestPin.h"


// The LMD18200 data sheet says that we must enforce a minimum delay between
// transitions on the PWM, DIR, and BRAKE lines

static const int transDelay = 8;	// minimum transition delay, tcnt's


//---------------------------------------------------------------------------
//
//	Pwm Driver Class
//

PwmDriver::PwmDriver(Ictlr& ic, unsigned per)
: Isr(ic, HC12Ictlr::isTc3),
  first(0),
  period(per),
  ocTick(3, true),
  ocOff(7)
{
  ocTick.at(tcnt + period / 2);  
}


void PwmDriver::attach(PwmChannel* chan)
{
  GlobalMask mask;

  chan->next = first;
  first = chan;
}

  
void PwmDriver::handler()
{
//	TestPin tp(portdlc, ddrdlc, 2);//for test
//	tp.raise();//for test
	
	bool phase = ocTick.high();
	if(phase == 1)
	{
		ocTick.dropAt(ocTick.at() + period/2);//if phase=1 (high) then drop the pwm 
	}else
	{
		ocOff.at(ocTick.at() + period);// ocOff turns on every T
		ocTick.raiseAt(ocTick.at() + period/2);//if phase=0 (low) then raise the pwm 
	}

	for(PwmChannel* chan = first; chan ; chan = chan->next)
		chan->handler(phase, ocOff.at());
	
//	tp.lower();//for test
}


//---------------------------------------------------------------------------
//
//	Pwm Channel Class
//

PwmChannel::PwmChannel(PwmDriver& dr, int num)
: driver(dr),
  next(0),  

  maxOnTime(driver.period - 4 * transDelay),
  dutyCycle(0),
  onTime(0),
  newOnTime(0),

  ocDir(num * 4 + 0),//OC0,4
  ocPwm(num * 4 + 1),//OC1,5
  ocBrk(num * 4 + 2)//OC2,6
{
  GlobalMask mask;

  // set up oc7 action for PWM output

  ocPwm.override(ocPwm.drop);

  // attach to channel list

  driver.attach(this);
}


// Set duty cycle.

void PwmChannel::setDuty(float duty)
{
  duty = duty >  1.0 ?  1.0 
       : duty < -1.0 ? -1.0
       : duty;
  dutyCycle = duty;		// for user interface

  // Calculate direction and on-time.
  
  int dir = duty >= 0 ? 1 : -1;
  unsigned on = unsigned(driver.period * duty * dir);//t_on/T= duty, so t_on=duty*T
  on = on < maxOnTime ? on : maxOnTime;

  // Store direction and on-time synchronously for pickup by handler().
  
  GlobalMask mask;

  direction = dir;
  newOnTime = on;
}
  

// Get actual duty cycle being used.

float PwmChannel::getDuty() const
{
  return dutyCycle;
}


// Set direction line and time that PWM output rises.
//phase: 1(high) 0(low), nextCycle: last offTime of OC7
void PwmChannel::handler(int phase, unsigned nextCycle)
{
	unsigned period = driver.period;
	if(phase == 1)
	{
		onTime = newOnTime;//sync ontime
		//direction of current of H-bridge		
		if(direction >= 0)
			ocDir.raiseAt(nextCycle + transDelay);//raise if phase and direction is 1
		else
			ocDir.dropAt(nextCycle + transDelay);//drop if phase =1 but direction is not 1
		if(onTime >= period/2)
			ocPwm.raiseAt(nextCycle + period - onTime);
	}else
	{
		ocBrk.raiseAt(nextCycle + period);
		if(onTime < period/2)
			ocPwm.raiseAt(nextCycle - onTime);		
	}

}


