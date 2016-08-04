#ifndef TIMER_H
#define TIMER_H

#include "Windows.h"
#include "mmsystem.h"

class Timer
{
public:
	Timer(){
		bActive = false;
		Reset();
	}
	~Timer(){}

	void Start()
	{
		bActive = true;
		Update();
		
	}
	void Stop()
	{
		bActive = false;
	}


	void Reset(){
		timeDelta = 0.0f;
		last_time = 0.0f;
		current_time = 0.0f;
		total_time = 0.0f;
	}

	float Update()
	{
		if( !bActive )
			return 0.0f;

		float deltaTime;
#ifndef WIN32
#define WIN32
#endif
#ifdef WIN32
	    static __int64 gTime,gLastTime;
		__int64 freq;
		QueryPerformanceCounter((LARGE_INTEGER *)&gTime);  // Get current count
		QueryPerformanceFrequency((LARGE_INTEGER *)&freq); // Get processor freq
		timeDelta = (double)(gTime - gLastTime)/(double)freq;
		gLastTime = gTime;
		last_time = (double)gTime;
		current_time = (double)gLastTime;
		last_time = current_time;
		total_time += timeDelta;
#else
		struct timeval tv;
		static struct timeval lasttv = { 0 , 0 };
		if (lasttv.tv_usec == 0 && lasttv.tv_sec == 0)
			gettimeofday(&lasttv, NULL);
		gettimeofday(&tv, NULL);
		timeDelta = (tv.tv_usec - lasttv.tv_usec)/1000000.f
			+ (tv.tv_sec - lasttv.tv_sec);
		lasttv = tv;
#endif
		return timeDelta;
	}
	float GetTimeInMilliseconds();
	float GetTimeInSeconds(){
		return current_time;
	}
	float GetTimeDelta(){
		return timeDelta;
	}

	float GetRunningTime(){
		return total_time;
	}

	bool bActive;
	float timeDelta;
	float last_time;
	float current_time;
	float total_time;
};
#endif