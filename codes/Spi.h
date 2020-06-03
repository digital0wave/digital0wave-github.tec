//lab10
//Jin Won Seo

#ifndef SPI_H
#define SPI_H

#include "NoCopy.h"
#include "Task.h"

class Spi : public NoCopy
{

	Mutex mutex; //serialize clients
	virtual void put(const void* buf, int bits) = 0;//pure VF
	virtual void get(void* buf, int bits) = 0;
	
protected:
	Spi()	{}
	virtual ~Spi()	{}	

public:			
		class Session : private NoCopy //handle object
		{
		private:
			Spi& spi;//spi port to use
			Mutex::Lock lock;
		
		public:
			Session(Spi &s) : spi(s), lock(spi.mutex)
			{
			}
			
			void put(const void* buf, int bits)
			{
				spi.put(buf, bits); //reflection
			}

			void get(void* buf, int bits)
			{
				spi.get(buf, bits);
			}
		};// end class Session
		friend class Session;//was Spi but it doesn't make sense to have friend class in same spi class
};//end Class spi
#endif
