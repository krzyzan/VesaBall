#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

//! Timer wysokiej rozdzielczoœci
/*!
	\warning U¿ywa \e QueryPerformanceCounter(), funkcja nie jest obs³ugiwana przez bardzo stare komputery!
*/

class CTimer
{
public:
    CTimer();

		//! Resetuje timer		
    void Reset();
		//! Startuje timer
    void Start();
		//! Zatrzymuje timer
    void Stop();				
		//! Przesuwa timer o 0.1 seekundy
    void Advance();			
		//! Pobiera absolutny czas sytemowy
    float GetAbsoluteTime();
		//! Pobiera czas
    float GetTime();
		//! Pobiera czas od ostatniego wywo³ania GetElapsedTime()
    float GetElapsedTime();
		//! Zwraca \b true jeœli timer jest zatrzymany
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
