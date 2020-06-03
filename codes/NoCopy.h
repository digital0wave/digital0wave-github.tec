//Lab10
//Jin Won Seo

#ifndef NOCOPY_H
#define NOCOPY_H

class NoCopy
{
	NoCopy(const NoCopy&);//ctor
	const NoCopy& operator = (const NoCopy&);//assignment operator

protected:
	NoCopy(){}
   ~NoCopy(){}
};

#endif
