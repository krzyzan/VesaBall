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
    FLOAT GetAbsoluteTime();	// get the absolute system time
    FLOAT GetTime();			// get the current time
    FLOAT GetElapsedTime();	// get the time that elapsed between GetElapsedTime() calls
    BOOL IsStopped();			// returns true if timer stopped

protected:
    BOOL m_bUsingQPF;
    BOOL m_bTimerStopped;
    LONGLONG m_llQPFTicksPerSec;

    LONGLONG m_llStopTime;
    LONGLONG m_llLastElapsedTime;
    LONGLONG m_llBaseTime;
};


