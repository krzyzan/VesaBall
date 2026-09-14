#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

//! High-resolution timer
/*!
	Backed by SDL_GetPerformanceCounter(), which is always available (unlike
	the Windows QueryPerformanceCounter() this class originally wrapped,
	which some very old computers didn't support).
*/

class CTimer
{
public:
	CTimer();

	//! Resets the timer
	void Reset();
	//! Starts the timer
	void Start();
	//! Stops the timer
	void Stop();
	//! Advances the timer by 0.1 seconds
	void Advance();
	//! Gets the absolute system time
	float GetAbsoluteTime();
	//! Gets the time
	float GetTime();
	//! Gets the time since the last call to GetElapsedTime()
	float GetElapsedTime();
	//! Returns \b true if the timer is stopped
	bool IsStopped();

protected:
	bool bTimerStopped;
	Uint64 llTicksPerSec;

	Uint64 llStopTime;
	Uint64 llLastElapsedTime;
	Uint64 llBaseTime;
};

#endif
