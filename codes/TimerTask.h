//Lab8
//Jin Won Seo

#ifndef TIMERTASK_H
#define TIMERTASK_H

#include "stddef.h"
#include "Task.h"

class TimerTask: public Task
{
	ModTimeTaskList clientList;//client tasks here wait for activating by this task
	TaskList tickList;//this task waits here for 1msec interrupt

	uint16 now;//current time in msec
	static TimerTask* the;
	virtual void main();

protected:
	TimerTask(size_t stackSize, uint16 priority);
	virtual ~TimerTask();
	void tick();//called by derived class, handler to notice passing of 1 msec
public:
	static void delay(uint16 msec);//delay for msec
};

TimerTask* TimerTask::the;//define singleton ptr

TimerTask::TimerTask(size_t stackSize, uint16 priority):Task(stackSize, priority), now(0)
{
	the = this;//fork in derived class
}

TimerTask::~TimerTask() {the=NULL;}

void TimerTask::delay(uint16 msec)
{
	the->clientList.enqueue(the->now + msec);
}

void TimerTask::main()
{
	while(true)
	{
		tickList.enqueue();
		now++;
		while(clientList.past(now))
			clientList.dequeue();
	}	
}

void TimerTask::tick()
{
	tickList.dequeue();
}

#endif
