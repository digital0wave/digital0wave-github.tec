#ifndef TESTPIN_H
#define TESTPIN_H

#include "chipio.h"

class TestPin
{
	private:
		volatile char& data;
		volatile char& dir;
		volatile char mask;

	public:
		//constructor
		TestPin(volatile char& da, volatile char& di, char bit)
		:data(da), dir(di), mask(1<<bit)
		{
			//1:associated pin(bit) output, 0:input
			dir |=mask;	//associated pin(bit) output
		};//arguments(data, direction, bit for mask)
		
		//pulse high
		void raise(){
			data |= mask;	//on
		};
		//pulse low
		void lower(){
		data &= ~mask;
		};
};
#endif	//TESTPIN_H
