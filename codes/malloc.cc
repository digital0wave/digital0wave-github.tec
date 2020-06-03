//ELEX8030 Lab2
//Jin Won Seo
//malloc.cc
//one way allocator only assigns available free space/memory
//it doesn't delete or free memory
#include "malloc.h"

extern char heap_beg[];

static char* cpFree;	
static char* cpHeapStart;

void* malloc(size_t size)
{
	const size_t margin = 512;//margin of stack space, was 20 
	//for the first time called
	if(cpHeapStart == NULL)
	{
		 cpHeapStart = &heap_beg[0];
		 cpFree = cpHeapStart;
	}
	
	char cDummy = 0;
	char* sp = &cDummy;
	size_t diff = sp - cpFree;

	//if memory space is available
	if((diff-margin) >= size)
	{
		//allocate memory
		cpHeapStart = cpFree;
		cpFree += size;	//move to size of array

	}else
	{
		//out of memory
		return NULL;
	}

	return cpHeapStart;

}
//one-way doesn't delete memory
void free(void* ipDel)
{
	//do nothing
	ipDel = ipDel;
	//	    delete[] (ipDel);    
}


