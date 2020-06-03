#ifndef NEW_H
#define NEW_H

//ELEX8030 Lab2
//new.h
//Jin Won Seo

#include "stddef.h"

void* operator new[](size_t size);
void operator delete[](void*);

void* operator new(size_t size);
void operator delete(void*);

//void memory_handler();
void set_new_handler(void (*)(void));
#endif
