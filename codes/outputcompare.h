// $Id: outputcompare.h,v 1.4 2004/11/01 20:30:25 querbach Exp $

// outputcompare.h		   Copyright (C) 2004, Real-Time Systems Inc.
//------------------------------------------- All Rights Reserved -----------
//
//	68HC12 Output Compare Timer Driver
//
//---------------------------------------------------------------------------

#ifndef OUTPUTCOMPARE_H
#define OUTPUTCOMPARE_H

#include "chipio.h"


class OutputCompare
{
  volatile unsigned int& tcr;	// compare register
  unsigned mask;		// interrupt mask
  unsigned level;		// output level mask
  unsigned mode;		// output mode mask

public:
  OutputCompare(int num, bool enable = false)
  : tcr((&tc0)[num]),
    mask(1 << num),
    level(ol << 2 * num),
    mode(om << 2 * num)
  {
    tscr |= (ten | tffca);	// enable timer, clear flags fast

    tcr = tcnt - 1;		// set safe value
    tios |= mask;		// configure as compare timer

    if (enable)
    {
      tflg1 = mask;		// clear request flip-flop
      tmsk1 |= mask;		// enable interrupt
    }
  }

  ~OutputCompare()
  {
    tmsk1 &= ~mask;
    tios &= ~mask;

    tctl1 &= ~(mode | level);
    oc7m &= ~mask;
    oc7d &= ~mask;
  }

  // Get time of last compare.
  
  unsigned at() const { return tcr; }
  
  // Set time of next compare.
    
  void at(unsigned time) { tcr = time; }
  
  // Raise output line at give time.
        
  void raiseAt(unsigned time)
  {
    tctl1 |= mode;
    tctl1 |= level;
    at(time);
  }  
  
  // Drop output line at given time.
      
  void dropAt(unsigned time)
  {
    tctl1 |= mode;
    tctl1 &= ~level;
    at(time);
  }  

  // After compare:  is output line high?
  
  bool high() { return (tctl1 & level) != 0; }
  
  
  // An event on oc7 can override the output state.
  
  enum Ovr
  {
    disable,		// disable 0c7 override
    drop,		// override with drop on oc7
    raise		// override with raise on oc7
  };

  // Override output state on oc7 compare.
    
  void override(Ovr ovr)
  {
    switch (ovr)
    {
      case disable:
        oc7m &= ~mask;
        break;
        
      case drop:
        oc7d &= ~mask;
        oc7m |= mask;
        break;
        
      case raise:
        oc7d |= mask;
        oc7m |= mask;
        break;
    }
  }
};


#endif // OUTPUTCOMPARE_H

