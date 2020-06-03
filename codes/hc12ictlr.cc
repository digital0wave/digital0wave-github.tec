// $Id: hc12ictlr.cc,v 1.2 2004/10/15 18:44:08 querbach Exp $

// hc12ictlr.cc			   Copyright (C) 2002, Real-Time Systems Inc.
//------------------------------------------- All Rights Reserved -----------
//
//	68HC12 Interrupt Controller
//
//---------------------------------------------------------------------------

#include "chipio.h"		// DEBUG
#include "hc12ictlr.h"


HC12Ictlr* HC12Ictlr::theHC12Ictlr;


HC12Ictlr::HC12Ictlr()
: Ictlr(isNumSources)
{
  theHC12Ictlr = this;
  ddrdlc |= (1 << 5);		// DEBUG
}


HC12Ictlr::~HC12Ictlr()
{
  theHC12Ictlr = 0;
}


// GCC won't allow asm() statements except inside functions, so we define a
// dummy function to allow wrapping the first-level interrupt handlers in
// assembly language.  Note from the declaration in the header file that
// this "function" is never called; it's just a bag we can put the assembly
// code in.

void HC12Ictlr::handlerTable()
{

#define	makeVsr(source)							\
  asm("		public	vsr%c0"	: : "i" (HC12Ictlr::source)	);	\
  asm("\nvsr%c0"		: : "i" (HC12Ictlr::source)	);	\
  portdlc |= (1 << 5);			/* DEBUG */			\
  theHC12Ictlr->runHandler(source);					\
  portdlc &= ~(1 << 5);			/* DEBUG */			\
  asm("rti");

  makeVsr(isBdl);
  makeVsr(isAtd);
  makeVsr(isSc0);
  makeVsr(isSpi);
  makeVsr(isPai);
  makeVsr(isPao);
  makeVsr(isTof);
  makeVsr(isTc7);
  makeVsr(isTc6);
  makeVsr(isTc5);
  makeVsr(isTc4);
  makeVsr(isTc3);
  makeVsr(isTc2);
  makeVsr(isTc1);
  makeVsr(isTc0);
  makeVsr(isRti);
  makeVsr(isIrq);
};

