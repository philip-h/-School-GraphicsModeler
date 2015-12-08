#include "PPoint.h"

//Constructors
PPoint3f::PPoint3f() 
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

PPoint3f::PPoint3f(float ax, float ay, float az)
{
	x = ax;
	y = ay;
	z = az;
}

void PPoint3f::giveValue()
{
	printf("(%f,%f,%f)\n", x,y,z);
}