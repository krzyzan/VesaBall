// Timer.h: interface for the CTimer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//-----------------------------------------------------------------------------
// Name: CTimer()
// Desc: Performs timer opertations. Use the following commands:
//			Reset 			- to reset the timer
//			Start 			- to start the timer
//			Stop			- to stop (or pause) the timer
//			Advance			- to advance the timer by 0.1 seconds
//			GetAbsoluteTime - to get the absolute system time
//			GetAppTime		- to get the current time
//			GetElapsedTime	- to get the time that elapsed between GetEllapsedTime calls
//									
//-----------------------------------------------------------------------------

class CTimer
{
private:
	BOOL	 m_bUsingQPF;
	LONGLONG m_llQPFTicksPerSec;
	LONGLONG m_llStopTime;
	LONGLONG m_llLastElapsedTime;
	LONGLONG m_llBaseTime;

public:
	CTimer()
	{
		m_llQPFTicksPerSec	= 0;
		LARGE_INTEGER qwTicksPerSec;
		QueryPerformanceFrequency( &qwTicksPerSec );
		m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
	}

	FLOAT GetElapsedTime()
	{
		LARGE_INTEGER qwTime;
		if ( m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
		else QueryPerformanceCounter( &qwTime );
		double fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
		m_llLastElapsedTime = qwTime.QuadPart;
		return (FLOAT) fElapsedTime;
	}

	FLOAT GetAppTime()
	{
		LARGE_INTEGER qwTime;
		if ( m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
		else QueryPerformanceCounter( &qwTime );
		double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
		return (FLOAT) fAppTime;
	}

	void Reset()
	{
		LARGE_INTEGER qwTime;
		if ( m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
		else QueryPerformanceCounter( &qwTime );
		m_llBaseTime		= qwTime.QuadPart;
		m_llLastElapsedTime = qwTime.QuadPart;
	}

	void Start()
	{
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter( &qwTime );
		m_llBaseTime += qwTime.QuadPart - m_llStopTime;
		m_llStopTime = 0;
		m_llLastElapsedTime = qwTime.QuadPart;
	}

	void Stop()
	{
		LARGE_INTEGER qwTime;
		if ( m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
		else QueryPerformanceCounter( &qwTime );
		m_llStopTime = qwTime.QuadPart;
		m_llLastElapsedTime = qwTime.QuadPart;
	}

	void TimerAdvance()
	{
		LARGE_INTEGER qwTime;
		if ( m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
		else QueryPerformanceCounter( &qwTime );
		m_llStopTime += m_llQPFTicksPerSec/10;
	}

	FLOAT GetAbsoluteTime()
	{
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter( &qwTime );
		double fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
		return (FLOAT) fTime;
	}
};