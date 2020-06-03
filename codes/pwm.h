// $Id: pwm.h,v 1.4 2004/11/01 20:19:26 querbach Exp $

// pwm.h			   Copyright (C) 2003, Real-Time Systems Inc.
//-------------------------------------------- All Rights Reserved ----------
//
//	PWM Driver
//
//---------------------------------------------------------------------------

#ifndef PWM_H
#define PWM_H

#include "NoCopy.h"
#include "Isr.h"
#include "outputcompare.h"


class PwmDriver : public Isr, private NoCopy
{
  class PwmChannel* first;	// list of channels
  const unsigned period;	// PWM period

  OutputCompare ocTick;		// twice cycle rate
  OutputCompare ocOff;		// turn off all outputs at start of cycle

  void attach(class PwmChannel* chan);
  virtual void handler();

  friend class PwmChannel;

public:
  PwmDriver(Ictlr& ic, unsigned period);
};


class PwmChannel : private NoCopy
{
  PwmDriver& driver;		// associated driver
  PwmChannel* next;		// next channel in list

  unsigned maxOnTime;		// maximum allowable period
  float dutyCycle;		// duty cycle as fraction
  int direction;		// direction of current flow
  unsigned onTime;		// PWM output on-time
  unsigned newOnTime;		// on-time for future cycles
  
  OutputCompare ocDir;		// direction line
  OutputCompare ocPwm;		// pwm line
  OutputCompare ocBrk;		// brake line

  void handler(int phase, unsigned nextCycle);

  friend class PwmDriver;
    
public:
  PwmChannel(PwmDriver& driver, int chanNum);

  void setDuty(float duty);
  float getDuty() const;
  
};




#endif // PWM_H

