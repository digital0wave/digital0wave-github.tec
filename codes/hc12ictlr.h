// $Id: hc12ictlr.h,v 1.1 2004/10/04 19:35:44 querbach Exp $

// hc12ictlr.h			   Copyright (C) 2002, Real-Time Systems Inc.
//------------------------------------------- All Rights Reserved -----------
//
//	68HC12 Interrupt Controller
//
//---------------------------------------------------------------------------

#ifndef HC12ICTLR_H
#define HC12ICTLR_H

#include "Isr.h"


class HC12Ictlr : public Ictlr
{
  static HC12Ictlr* theHC12Ictlr;
  static void handlerTable() __attribute__ ((unused));//never called as a function, gcc specific

public:
  HC12Ictlr();
  ~HC12Ictlr();

  enum Sources
  {
    isBdl,
    isAtd,
    isSc0,
    isSpi,
    isPai,
    isPao,
    isTof,
    isTc7,
    isTc6,
    isTc5,
    isTc4,
    isTc3,
    isTc2,
    isTc1,
    isTc0,
    isRti,
    isIrq,
    
    isNumSources
  };    
};


#endif // HC12ICTLR_H

