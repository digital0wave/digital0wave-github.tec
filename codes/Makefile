# $Id: Makefile,v 1.5 2019/01/26 21:28:54 jseo Exp $

# Makefile for ELEX8030 term project


# Project description

PROG	= tec.o
LIBS		= iostream.a 

OBJS    = $(addsuffix .o, $(basename $(wildcard *.cc) $(wildcard *.s)))
CFLAGS  = -O2 -nostdinc -fno-exceptions -fno-rtti -fpcc-struct-return -fvtable-thunks
CFLAGS	+= -W -Wall -Wtraditional -Wundef -Wbad-function-cast -Wcast-qual -Wwrite-strings
CFLAGS	+= -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations
CFLAGS	+= -Wold-style-cast -Woverloaded-virtual -Werror -g -Wa,-ahlnd=$*.lst -D__HC912B32__
LFLAGS	 = -Wl,--defsym,_stack_top=0x7FFF -Wl,-Map,$*.map 
LFLAGS 	+= -Wl,-T,$*.ld

# Tools

CC	=  hc12-coff-gcc


# Targets

all : $(PROG)

$(PROG) : $(OBJS) $(LIBS) 
	$(CC) $(LFLAGS) -o $@ $^
%.o : %.cc
	$(CC) $(CFLAGS) -c $<

%.o : %.s
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(PROG) $(OBJS) *.lst *.map

