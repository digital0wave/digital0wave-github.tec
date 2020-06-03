// $Id: globalmask.h,v 1.1 2004/10/04 19:35:44 querbach Exp $

// globalmask.h			   Copyright (C) 2002, Real-Time Systems Inc.
//-------------------------------------------- All Rights Reserved ----------
//
//	Global Interrupt Control
//
//	Creating an object of the GlobalMask class masks interrupts globally
//	as long as it exists.  When the object is destroyed, the previous
//	state of the global interrupt mask is restored.
//
//---------------------------------------------------------------------------

#ifndef GLOBALMASK_H
#define GLOBALMASK_H


class GlobalMask
{
  int prevState;	// interrupt mask state before creation of object

public:
  GlobalMask()		// mask interrupts
  {
    asm(" 
	tfr	ccr,b		; get current state of mask
	std	%0		; save it
	sei			; mask interrupts globally
	" : "=m" (prevState) : : "d"
    ); 
  }
  
  ~GlobalMask()		// restore interrupt mask
  {
    asm("
    	ldd	%0		; get previous state of mask
    	tfr	b,ccr		; restore it
    	" : : "m" (prevState) : "d"
    );
  }
};


#endif // GLOBALMASK_H

