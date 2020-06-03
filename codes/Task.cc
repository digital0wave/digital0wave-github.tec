//Lab7
//Task.cc
//Jin Won Seo

#include "Task.h"

Task initTask(NULL, ~0);//task for code running from reset
Task* Task::current(& initTask);

//TaskList readyList;//list of tasks that is ready to run
PriorityTaskList _readyList;
TaskList& readyList(_readyList);

register void** spReg asm("s");//give the name spReg to H/W stack pointer Reg's

void Task::changePriority(uint16 pri)
{
	GlobalMask mask;
	current->priority = pri;
	yield();
}
/*
* Context switch
* 1st, save return addr of task( from running Task A' stack)
* 2nd, as return, pop return addr from another task(new Task B's stack) 
*/
void Task::dispatch()
{
	current->sp =spReg;//save old sp stored in H/W stack point used in HC12
	current = this; //make this task as current task
	spReg = current->sp;//load this task to H/W sp
	//pop return addr
}
/*
TaskList::~TaskList()
{
}
*/
//put caller on a TaskList
//swtich to first task on readyList
void TaskList::enqueue(int msec)
{
//	GlobalMask mask;
	Node node(this, msec);//assign this to node list
	readyList.first->task->dispatch();
}
//move node to readyList
void TaskList::dequeue()
{
//	GlobalMask mask;
	Node node(&readyList);//added in lab 9
	readyList.insert(remove(first));//remove first node from a wait list then insert it in readylist
	readyList.first->task->dispatch();//lab 9
}
void Task::yield()
{
	readyList.enqueue();//consent ready to test
}

void Task::main(){}//task default main, do nothing

void Task::start()
{
	yield();//re-establish priority invariant
	asm("cli");// enable global interrupts
	current->main();
	asm("sei"); // disable globa interrupts

	TaskList forever;// for the first time start, create list and put it into list. no one uses this list
	forever.enqueue();
}


Task::Task(size_t stackSize, uint16 pri)
:stack(new (void*)[stackSize/sizeof(void*)]), sp(&stack[stackSize/sizeof(void*)]), priority(pri)
{
	if(stackSize)//special case for initial task
	{
		*--sp = 0;//decrement first and store. for gdb
		*--sp = 0;//for gdb
		*--sp = &start;
	}
}

Task::~Task()
{
	//stack removes task from list
	//FIXME
	delete[] stack;
}

/* 
*Fork system call use for creates a new process, 
*which is called child process, which runs concurrently with process (which process called system call fork) 
*and this process is called by parent process(In our case, InitTask). 
*/
void Task::fork()
{
//	GlobalMask mask;
	//stop creator
	TaskList::Node node(&readyList);
	//start new task being created
	dispatch();
}

//insert a TaskList::Node at the end of list
void TaskList::insert(Node* node)
{
	if(node)//if node is null do nothing
	{
		//if list is empty
		if(!first)
		{
			first = node;//assign node to the first
		}else//otherwise search for final node in the list
		{
			Node* temp = first;
			//once get out of loop, temp is the last one
			while(temp->next != NULL)
				temp = temp->next;
			
			node->next = temp->next;//assign NULL to the node's next
			temp->next = node;
		}
		node->list = this;//every node points back to TaskList(this)	
	}//end if	
}

TaskList::Node* TaskList::remove(TaskList::Node* node)
{

	Node* temp = first;

	if(node == first)
	{
		first = node->next;//assign NULL to first 
	}
	else
	{
		
		while(temp->next != NULL)
		{
			
			if(temp->next == node)
			{
				temp->next = node->next;//assign node's next to temp's next
				break;	
			}
			temp = temp->next;
		}
		if(temp->next == NULL)//if node is not found
			return NULL;		
	}	
	node->next = NULL;	
	node->list = NULL;//node doesn't belong to any TaskList
	
	return node;//or node not sure
}


bool ModTimeTaskList::past(uint16 now)
{
	return first && (int(now - first->time) >=0);
	//return first && ((now - first->time) >=0);
}

/*
* insert node according to time using modulo
* earliest time first
*/
void ModTimeTaskList::insert(TaskList::Node* node)
{
	//if list is empty
	if(node)
	{
		if(!first)
		{
			first = node;//assign node to the first
		}else//otherwise search for final node in the list
		{
			Node* current = first;
			//check 1st node pointed by first 
			if ((current->time - node->time >= 0))
			{ 
			  node->next = current; 
			  first = node; 
			}else
			{
				while (current->next!=NULL && (( current->next->time - node->time ) < 0))
				{ 
					current = current->next; 
				} 
				//once get out of loop, node's time < current's time

				node->next = current->next; 
				current->next = node; 
			}
		}
		node->list = this;//every node points back to TaskList(this)
	}

}

void PriorityTaskList::insert(TaskList::Node* node)
{
	if(node)
	{
		if(!first)
		{
			first = node;//assign node to the first
		}else//otherwise search for final node in the list
		{
			Node* current = first;
			//check 1st node pointed by first 
			if (current->task->getPriority() >= node->task->getPriority())//could be replaced by lowerThan()
			{ 
			  node->next = current; 
			  first = node; 
			}else
			{
				while (current->next != NULL && !current->next->task->lowerThan(node->task))
				{ 
					current = current->next; 
				} 
				//once get out of loop, the priority of the task pointed by node < priority of the task pointed by current

				node->next = current->next; 
				current->next = node; 
			}
		}
		node->list = this;//every node points back to TaskList(this)
	}

}

void Mutex::acquire()
{
	SchedLock lock;//scheduling lock so can't be preempted
	
	while(holder)
		enqueue();
	holder = Task::getCurrent();
}//SchedLock goes out of scope

void Mutex::release()
{
	SchedLock lock;
	holder = NULL;
	dequeue();
}//schedlock goes out of scope. calls deconstructor of schedlock

void Semaphore:: wait()
{
	SchedLock lock;
	while(value <= 0)
		enqueue();
	value--;
}

void Semaphore::signal()
{
	SchedLock lock;
	value++;
	dequeue();
}


