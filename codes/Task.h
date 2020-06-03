//Lab7
//Task class and TaskList class
//Jin Won Seo

#ifndef TASK_H
#define TASK_H

#include "new.h"
#include "NoCopy.h"
#include "stddef.h"
#include "globalmask.h"
//#include "ModTimeTaskList.h"

class Task
{
	void** stack;
	void** sp;//stack pointer when not running
	uint16 priority;//lab 9
	
	//runing task
	static Task* current;
	
	void dispatch();//switch to this pointer task
	
	virtual void main();//actual core stuff to do
	
	static void start();//call main() to start task
	friend class TaskList;

	protected:
	void fork();//do first dispatch of task pointed by this pointer

	public:
	Task(size_t stackSize, uint16 priority);
	virtual ~Task();
	//get current task
	static Task* const getCurrent()
	{
		return current;
	}
	static void yield(); //give up CPU to another task

	bool lowerThan(const Task* task) const
	{
		//return true if this task is lower than argument task
		return (priority > task->priority);
	}

	uint16 getPriority() const
	{
		return priority;
	}

	static void changePriority(uint16 priority);

};

/*
* singlely lined list
*/
class TaskList : public NoCopy
{

protected:
	struct Node
	{
		TaskList* list;
		Node* next;
		Task* const task;
		int time;		
		GlobalMask mask;

		//put caller on list l
		Node(TaskList* l, int msec=0):list(l), next(NULL), task(Task::getCurrent()), time(msec)
		{
			list->insert(this);//insert this new node on the list,l
			
		}

		~Node()//destructor
		{
			list->remove(this);
		}
	};//end struct Node

	Node* first;//first Node in list

private:
	virtual void insert(Node*);
	Node* remove(Node*);
friend Node;

public:

	TaskList():first(NULL){}
	virtual ~TaskList(){}//if class has VF then it has to have virutal dtor

	void enqueue(int msec=0);//put caller on this list
	void dequeue();//move node to readyList
};//end TaskList class

class ModTimeTaskList : public TaskList
{
	virtual void insert(Node*);
	
public:
	ModTimeTaskList(){}
	virtual ~ModTimeTaskList(){}

	bool past(uint16 now);//return true if we are past first task's wake up time
};//end of ModTimeTaskList

class PriorityTaskList : public TaskList
{
	//ordered by task priority
	virtual void insert(Node*);
public:
	PriorityTaskList(){}
	virtual ~PriorityTaskList(){}
};

class SchedLock
{
	uint16 oldPriority;
public:
	SchedLock(uint16 newPriority = 0) : oldPriority(Task::getCurrent()->getPriority())
	{
		Task::changePriority(newPriority);
	}
	
	~SchedLock()
	{
		Task::changePriority(oldPriority);
	}
};

//added in lab 10
class Mutex : public TaskList
{
	//waitlist one/mutex
	//pointer to holding task, 0 if none
	const Task* holder; 
	void acquire();
	void release();

public:
	Mutex() : holder(NULL){}
	
	class Lock : public NoCopy
	{
		Mutex& mutex;
	public:
		Lock(Mutex& m): mutex(m)
		{
			mutex.acquire();//obtain mutex
		}
		
		~Lock()
		{
			mutex.release();
		}
	};//end of class Lock
	friend class Lock;
};//end of class Mutex

class Semaphore : public TaskList
{
	int value;// counter 
public:
	Semaphore(int v=0): value(v) {}
	void wait();//conventionallly p()
	void signal();//conventionally v()
};//end of class Semaphore

#endif
