#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

//! High-resolution timer
/*!
	\warning Uses \e QueryPerformanceCounter(); this function is not supported on very old computers!
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
    bool bUsingQPF;
    bool bTimerStopped;
    LONGLONG llQPFTicksPerSec;

    LONGLONG llStopTime;
    LONGLONG llLastElapsedTime;
    LONGLONG llBaseTime;
};

#endif
