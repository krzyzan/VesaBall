#include "stdafx.h"
#include "timer.h"

CTimer::CTimer()
{
    m_bUsingQPF         = false;
    m_bTimerStopped     = true;
    m_llQPFTicksPerSec  = 0;

    m_llStopTime        = 0;
    m_llLastElapsedTime = 0;
    m_llBaseTime        = 0;

    // Use QueryPerformanceFrequency() to get frequency of timer.  
    LARGE_INTEGER qwTicksPerSec;
    m_bUsingQPF = (bool) (QueryPerformanceFrequency( &qwTicksPerSec ) != 0);
    m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}


//--------------------------------------------------------------------------------------
void CTimer::Reset()
{
    if( !m_bUsingQPF )
        return;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    m_llBaseTime        = qwTime.QuadPart;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_llStopTime        = 0;
    m_bTimerStopped     = FALSE;
}


//--------------------------------------------------------------------------------------
void CTimer::Start()
{
    if( !m_bUsingQPF )
        return;

    // Get the current time
    LARGE_INTEGER qwTime;
    QueryPerformanceCounter( &qwTime );

    if( m_bTimerStopped )
        m_llBaseTime += qwTime.QuadPart - m_llStopTime;
    m_llStopTime = 0;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_bTimerStopped = FALSE;
}


//--------------------------------------------------------------------------------------
void CTimer::Stop()
{
    if( !m_bUsingQPF )
        return;

    if( !m_bTimerStopped )
    {
        // Get either the current time or the stop time
        LARGE_INTEGER qwTime;
        if( m_llStopTime != 0 )
            qwTime.QuadPart = m_llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        m_llStopTime = qwTime.QuadPart;
        m_llLastElapsedTime = qwTime.QuadPart;
        m_bTimerStopped = TRUE;
    }
}


//--------------------------------------------------------------------------------------
void CTimer::Advance()
{
    if( !m_bUsingQPF )
        return;

    m_llStopTime += m_llQPFTicksPerSec/10;
}


//--------------------------------------------------------------------------------------
FLOAT CTimer::GetAbsoluteTime()
{
    if( !m_bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    FLOAT fTime = qwTime.QuadPart / (FLOAT) m_llQPFTicksPerSec;

    return fTime;
}


//--------------------------------------------------------------------------------------
FLOAT CTimer::GetTime()
{
    if( !m_bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    FLOAT fAppTime = (FLOAT) ( qwTime.QuadPart - m_llBaseTime ) / (FLOAT) m_llQPFTicksPerSec;

    return fAppTime;
}


//--------------------------------------------------------------------------------------
FLOAT CTimer::GetElapsedTime()
{
    if( !m_bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    FLOAT fElapsedTime = (FLOAT) ( qwTime.QuadPart - m_llLastElapsedTime ) / (FLOAT) m_llQPFTicksPerSec;
    m_llLastElapsedTime = qwTime.QuadPart;

    return fElapsedTime;
}


//--------------------------------------------------------------------------------------
bool CTimer::IsStopped()
{
    return m_bTimerStopped;
}

