#include <windows.h>
#include "Timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency);
	FPS = 100;
	StartTime = EndTime = 0;
}


Timer::Timer(unsigned int Fps)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency);
	if(Fps > 0)
		FPS = Fps;
	else
		FPS = 100;
	StartTime = EndTime = 0;
}


BOOL Timer::CheckSupport()
{
	return QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency);
}


void Timer::SetStartTime()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&StartTime);
}


void Timer::SetStartTime(__int64 Time)
{
	StartTime = Time;
}


void Timer::SetEndTime()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&EndTime);
}


void Timer::SetFPS(unsigned int Fps)
{
	if(Fps > 0)
		FPS = Fps;
}


void Timer::SetMaxFPS()
{
	FPS = 4294967295;
}


__int64 Timer::GetStartTime()
{
	return StartTime;
}


__int64 Timer::GetEndTime()
{
	return EndTime;
}


__int64 Timer::GetDifference()
{
	return EndTime-StartTime;
}


unsigned int Timer::GetFPS()
{
	return FPS;
}


__int64 Timer::CalculateFPSChunk()
{
	return Frequency/FPS;
}


double Timer::CalculateFPS()
{
	if(GetDifference() > 0)
		return (double)Frequency/GetDifference();

	return 0;
}