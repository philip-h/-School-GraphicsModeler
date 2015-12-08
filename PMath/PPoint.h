#ifndef __PPOINT_H__
#define __PPOINT_H__

#include <stdio.h>

class PPoint3f {
public:
	//Public Variables
	float x,y,z;

	//Constructors
	PPoint3f();
	PPoint3f(float, float, float);

	void giveValue();
};
#endif
