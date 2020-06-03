//lab 10
//Jin Won Seo

#ifndef FIFO_H
#define FIFO_H

#include "task.h"
#include "stddef.h"

template<typename T> class FiFo
{
	private:
		T *bufP;
		T *endP;
		T *putP;
		T *getP;

		Mutex mutex;
		Semaphore spaces;//count empty space
		Semaphore items;//count items in buffer

	public:
		FiFo(uint16 len) : bufP(new T[len]), endP(bufP + len), putP(bufP), getP(bufP), spaces(len), items(0)
		{
		}

		~FiFo()
		{
			delete[] bufP;
		}

		void put(const T &t)
		{
			Mutex::Lock lock(mutex);

			spaces.wait();
			*putP++ = t;
			putP = putP < endP ? putP : bufP;
			items.signal();
		}		

		T get()
		{
			Mutex::Lock lock(mutex);

			items.wait();
			T t = *getP++;
			getP = getP < endP ? getP : bufP;
			spaces.signal();
			return t;
		}
};

#endif


