#ifndef TOOL_ALL_IN_ONE_H
#define TOOL_ALL_IN_ONE_H

#include<assert.h>
#include"Print.h"

const double hitNear = 0.01;
const double hitFar = 500;

const int maxCastTime = 10;

inline int RandomIntger(int min, int max)
{
	assert(max - min >= 1);
	return rand() % (max - min + 1) + min;
}

#endif