#include "stdafx.h"
#include "timer.h"

CTimer::CTimer()
{
    bUsingQPF         = false;
    bTimerStopped     = true;
    llQPFTicksPerSec  = 0;

    llStopTime        = 0;
    llLastElapsedTime = 0;
    llBaseTime        = 0;

    // Use QueryPerformanceFrequency() to get frequency of timer.  
    LARGE_INTEGER qwTicksPerSec;
    bUsingQPF = (BOOL) (QueryPerformanceFrequency( &qwTicksPerSec ) != 0);
    llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}


//--------------------------------------------------------------------------------------
void CTimer::Reset()
{
    if ( !bUsingQPF )
        return;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if ( llStopTime != 0 )
        qwTime.QuadPart = llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    llBaseTime        = qwTime.QuadPart;
    llLastElapsedTime = qwTime.QuadPart;
    llStopTime        = 0;
    bTimerStopped     = FALSE;
}


//--------------------------------------------------------------------------------------
void CTimer::Start()
{
    if ( !bUsingQPF )
        return;

    // Get the current time
    LARGE_INTEGER qwTime;
    QueryPerformanceCounter( &qwTime );

    if ( bTimerStopped )
        llBaseTime += qwTime.QuadPart - llStopTime;
    llStopTime = 0;
    llLastElapsedTime = qwTime.QuadPart;
    bTimerStopped = FALSE;
}


//--------------------------------------------------------------------------------------
void CTimer::Stop()
{
    if ( !bUsingQPF )
        return;

    if ( !bTimerStopped )
    {
        // Get either the current time or the stop time
        LARGE_INTEGER qwTime;
        if ( llStopTime != 0 )
            qwTime.QuadPart = llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        llStopTime = qwTime.QuadPart;
        llLastElapsedTime = qwTime.QuadPart;
        bTimerStopped = TRUE;
    }
}


//--------------------------------------------------------------------------------------
void CTimer::Advance()
{
    if ( !bUsingQPF )
        return;

    llStopTime += llQPFTicksPerSec/10;
}


//--------------------------------------------------------------------------------------
float CTimer::GetAbsoluteTime()
{
    if ( !bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if ( llStopTime != 0 )
        qwTime.QuadPart = llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    float fTime = qwTime.QuadPart / (float) llQPFTicksPerSec;

    return fTime;
}


//--------------------------------------------------------------------------------------
float CTimer::GetTime()
{
    if ( !bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if ( llStopTime != 0 )
        qwTime.QuadPart = llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    float fAppTime = (float) ( qwTime.QuadPart - llBaseTime ) / (float) llQPFTicksPerSec;

    return fAppTime;
}


//--------------------------------------------------------------------------------------
float CTimer::GetElapsedTime()
{
    if ( !bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if ( llStopTime != 0 )
        qwTime.QuadPart = llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    float fElapsedTime = (float) ( qwTime.QuadPart - llLastElapsedTime ) / (float) llQPFTicksPerSec;
    llLastElapsedTime = qwTime.QuadPart;

    return fElapsedTime;
}


//--------------------------------------------------------------------------------------
bool CTimer::IsStopped()
{
    return bTimerStopped;
}

