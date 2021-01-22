#include "timer.h"

StopWatch::StopWatch():
	currTime({0}),
	prevTime({0})
{
	QueryPerformanceFrequency(&frequency);
	constexpr auto TARGET_RESOLUTION = 1; //1 ms target resolution;
	TIMECAPS tc;
	if(timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR){
		//Error, app can't continue.
	}
	wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(wTimerRes);
}

StopWatch::~StopWatch(){
	timeEndPeriod(wTimerRes);
}

double StopWatch::LIToSecs(LARGE_INTEGER &L){
	return((double)L.QuadPart / (double)frequency.QuadPart);
}

void StopWatch::startTimer(){
	QueryPerformanceCounter(&prevTime);
}

double StopWatch::getElapsedTime(){
	LARGE_INTEGER time;
	QueryPerformanceCounter(&currTime);
	time.QuadPart = currTime.QuadPart - prevTime.QuadPart;
	prevTime = currTime;
	return LIToSecs(time);
}

void StopWatch::waitUntil(long long time){ //No gd
	LARGE_INTEGER nowTime;
	LONGLONG timeElapsed;
	while(1){
		QueryPerformanceCounter(&nowTime);
		timeElapsed = ((nowTime.QuadPart - prevTime.QuadPart) * 1000 / frequency.QuadPart);
		if(timeElapsed > time){
			return;
		} else{
			Sleep(time - timeElapsed);
		}
	}
}