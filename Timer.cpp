#include "StdAfx.h"
#include "Timer.h"

CTimer::CTimer()
{
	bTimerStopped = true;

	llStopTime = 0;
	llLastElapsedTime = 0;
	llBaseTime = 0;

	llTicksPerSec = SDL_GetPerformanceFrequency();
}

//--------------------------------------------------------------------------------------
void CTimer::Reset()
{
	// Get either the current time or the stop time
	Uint64 qwTime = (llStopTime != 0) ? llStopTime : SDL_GetPerformanceCounter();

	llBaseTime = qwTime;
	llLastElapsedTime = qwTime;
	llStopTime = 0;
	bTimerStopped = false;
}

//--------------------------------------------------------------------------------------
void CTimer::Start()
{
	// Get the current time
	Uint64 qwTime = SDL_GetPerformanceCounter();

	if (bTimerStopped)
		llBaseTime += qwTime - llStopTime;
	llStopTime = 0;
	llLastElapsedTime = qwTime;
	bTimerStopped = false;
}

//--------------------------------------------------------------------------------------
void CTimer::Stop()
{
	if (!bTimerStopped)
	{
		// Get either the current time or the stop time
		Uint64 qwTime = (llStopTime != 0) ? llStopTime : SDL_GetPerformanceCounter();

		llStopTime = qwTime;
		llLastElapsedTime = qwTime;
		bTimerStopped = true;
	}
}

//--------------------------------------------------------------------------------------
void CTimer::Advance()
{
	llStopTime += llTicksPerSec / 10;
}

//--------------------------------------------------------------------------------------
float CTimer::GetAbsoluteTime()
{
	// Get either the current time or the stop time
	Uint64 qwTime = (llStopTime != 0) ? llStopTime : SDL_GetPerformanceCounter();

	float fTime = (float)qwTime / (float)llTicksPerSec;

	return fTime;
}

//--------------------------------------------------------------------------------------
float CTimer::GetTime()
{
	// Get either the current time or the stop time
	Uint64 qwTime = (llStopTime != 0) ? llStopTime : SDL_GetPerformanceCounter();

	float fAppTime = (float)(qwTime - llBaseTime) / (float)llTicksPerSec;

	return fAppTime;
}

//--------------------------------------------------------------------------------------
float CTimer::GetElapsedTime()
{
	// Get either the current time or the stop time
	Uint64 qwTime = (llStopTime != 0) ? llStopTime : SDL_GetPerformanceCounter();

	float fElapsedTime = (float)(qwTime - llLastElapsedTime) / (float)llTicksPerSec;
	llLastElapsedTime = qwTime;

	return fElapsedTime;
}

//--------------------------------------------------------------------------------------
bool CTimer::IsStopped()
{
	return bTimerStopped;
}
