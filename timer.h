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
	BOOL	 bUsingQPF;
	LONGLONG llQPFTicksPerSec;
	LONGLONG llStopTime;
	LONGLONG llLastElapsedTime;
	LONGLONG llBaseTime;

public:
	CTimer()
	{
		llQPFTicksPerSec	= 0;
		LARGE_INTEGER qwTicksPerSec;
		QueryPerformanceFrequency( &qwTicksPerSec );
		llQPFTicksPerSec = qwTicksPerSec.QuadPart;
	}

	FLOAT GetElapsedTime()
	{
		LARGE_INTEGER qwTime;
		if ( llStopTime != 0) qwTime.QuadPart = llStopTime;
		else QueryPerformanceCounter( &qwTime );
		double fElapsedTime = (double) ( qwTime.QuadPart - llLastElapsedTime ) / (double) llQPFTicksPerSec;
		llLastElapsedTime = qwTime.QuadPart;
		return (FLOAT) fElapsedTime;
	}

	FLOAT GetAppTime()
	{
		LARGE_INTEGER qwTime;
		if ( llStopTime != 0) qwTime.QuadPart = llStopTime;
		else QueryPerformanceCounter( &qwTime );
		double fAppTime = (double) ( qwTime.QuadPart - llBaseTime ) / (double) llQPFTicksPerSec;
		return (FLOAT) fAppTime;
	}

	void Reset()
	{
		LARGE_INTEGER qwTime;
		if ( llStopTime != 0) qwTime.QuadPart = llStopTime;
		else QueryPerformanceCounter( &qwTime );
		llBaseTime		= qwTime.QuadPart;
		llLastElapsedTime = qwTime.QuadPart;
	}

	void Start()
	{
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter( &qwTime );
		llBaseTime += qwTime.QuadPart - llStopTime;
		llStopTime = 0;
		llLastElapsedTime = qwTime.QuadPart;
	}

	void Stop()
	{
		LARGE_INTEGER qwTime;
		if ( llStopTime != 0) qwTime.QuadPart = llStopTime;
		else QueryPerformanceCounter( &qwTime );
		llStopTime = qwTime.QuadPart;
		llLastElapsedTime = qwTime.QuadPart;
	}

	void TimerAdvance()
	{
		LARGE_INTEGER qwTime;
		if ( llStopTime != 0) qwTime.QuadPart = llStopTime;
		else QueryPerformanceCounter( &qwTime );
		llStopTime += llQPFTicksPerSec/10;
	}

	FLOAT GetAbsoluteTime()
	{
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter( &qwTime );
		double fTime = qwTime.QuadPart / (double) llQPFTicksPerSec;
		return (FLOAT) fTime;
	}
};