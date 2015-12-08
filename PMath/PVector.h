#ifndef __PVector_H__
#define __PVector_H__

#include <cstdio>
#include <cmath>
#include "PPoint.h"

class PVector3f 
{
public:
	//Public Variables
	float x,y,z;
	float magnitude;

	//Constructors
	PVector3f();
	PVector3f(float, float, float);
	PVector3f(PPoint3f, PPoint3f);

	//Public Functions
	float calcMagnitude();
	void normalize();
	void giveValue();

	//Math Methods!
	PVector3f operator +(const PVector3f&);
	PVector3f operator -(const PVector3f&);
	PVector3f operator -();
	PVector3f operator *(const PVector3f&);
	PVector3f operator *(const float);
	
};


#endif