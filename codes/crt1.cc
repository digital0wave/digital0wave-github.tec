//crt1()
//Lab 1
//Jin Won Seo
#include "main.h"

typedef void(*pFunc)(void);

extern char bss_beg[], bss_end[],heap_beg[];

extern pFunc ctor_beg[], ctor_end[], dtor_beg[], dtor_end[];//for statically-allocated object


//initialize from bss_beg to bss_end with zero
int crt1()
{
	char* bp = &bss_beg[0];
	char* ep = &bss_end[0];
	char* hb = &heap_beg[0];//heap begin
	char* he = reinterpret_cast<char*>(0x7fff);//heap end
	
	pFunc* cb = &ctor_beg[0];//begin of ctor
	pFunc* ce = &ctor_end[0];//end of ctor
	pFunc* db = &dtor_beg[0];
	pFunc* de = &dtor_end[0];	
	
	while(bp <= ep)
	{
		*bp = 0;
		bp++;
	}

	while(hb <= he)
	{
		*hb = 0x55;
		hb++;
	}

	/*
	*execute ctor of statically allocated Task object before main()
	*/
	while(cb <= ce)
	{
		(*cb)();//execute the function pointed by Function pointer
		cb++;
	}
	
	asm("cli");// enable global interrupts

	main();

	asm("sei"); // disable globa interrupts

	/*
	*execute dtor of statically allocated Task object after main()
	*/
	while(db <= de)
	{
		(*db)();//execute the function pointed by Function pointer
		db++;
	}

	return 0;

}
