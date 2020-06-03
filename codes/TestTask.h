//Lab7
//Jin Won Seo

#ifndef TESTTASK_H
#define TESTTASK_H

#include "Task.h"
#include "stddef.h"
#include "TestPin.h"

class TestTask:public Task
{
	TestPin tp;
	size_t times;
	int interval;//time between pulse bursting

	virtual void main()
	{
		while(true)
		{
			for(size_t i=0; i<times; i++)
			{
				tp.raise();
				tp.lower();
			}
			TimerTask::delay(interval);
			//yield();//yield its execution to another task
		}		
	}

	public:
	TestTask(size_t stackSize, uint16 priority, int bit, size_t t, int duration)
	:Task(stackSize,priority), tp(portdlc, ddrdlc, bit), times(t), interval(duration)
//:Task(stackSize), tp(portp, ddrp, bit), times(t), interval(duration)
	{
		fork();
	}
	virtual ~TestTask(){}
};
#endif
