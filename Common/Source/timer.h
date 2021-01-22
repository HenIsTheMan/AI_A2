#pragma once

#include <windows.h>

class StopWatch{
public:
	StopWatch();
	~StopWatch();

	double getElapsedTime(); //Get time in seconds since the last call to this function
	void startTimer();
	void waitUntil(long long); //No gd
private:
	LARGE_INTEGER frequency, prevTime, currTime;
	UINT wTimerRes;

	double LIToSecs(LARGE_INTEGER&);
};