#pragma once

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include <windows.h>
#include "MyMath.h"

class StopWatch{
public:
	StopWatch();
	~StopWatch();

	double getElapsedTime(); //Get time in seconds since the last call to this function
	void startTimer();
private:
	LARGE_INTEGER frequency, prevTime, currTime;
	UINT wTimerRes;

	double LIToSecs(LARGE_INTEGER&);
};