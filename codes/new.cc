//ELEX8030 Lab2
//new.cc
//Jin won Seo
#include "new.h"
#include "malloc.h"

static void (*pHandler)(void);
//custome memory allocation function new[]

void* operator new[](size_t size)
{
	void* vpMallloc = malloc(size);
	if(vpMallloc != NULL)
	{
		return vpMallloc;
	}else { //fail to allocate,error
		pHandler();
		return NULL;
	}
}

void operator delete[](void* p)
{
	 free(p);
}

void* operator new(size_t size)
{

	void* vpMallloc = malloc(size);
	if(vpMallloc != NULL)
	{
		return vpMallloc;
	}else { //fail to allocate,error
		pHandler();
		return NULL;
	}
}

void operator delete(void* p)
{
	free(p);
//    delete p;
}
/*
void memory_handler()
{
   //fail to allocte memory
   //exit(1);
   set_new_handler(NULL);
}
*/
void set_new_handler(void (*pf)(void))
{
//   set_new_handler(pEHand);
    pHandler = pf;

}
