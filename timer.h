// Timer.h: interface for the CTimer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <windows.h>

class CTimer
{
public:
    CTimer();

    void Reset();				// resets the timer
    void Start();				// starts the timer
    void Stop();				// stop (or pause) the timer
    void Advance();				// advance the timer by 0.1 seconds
    float GetAbsoluteTime();	// get the absolute system time
    float GetTime();			// get the current time
    float GetElapsedTime();	// get the time that elapsed between GetElapsedTime() calls
    bool IsStopped();			// returns true if timer stopped

protected:
    bool bUsingQPF;
    bool bTimerStopped;
    LONGLONG llQPFTicksPerSec;

    LONGLONG llStopTime;
    LONGLONG llLastElapsedTime;
    LONGLONG llBaseTime;
};


