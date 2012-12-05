#include "GameTimer.h"
#include <windows.h>

GameTimer::GameTimer()
: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
  mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
__int64 countsPerSec;
QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
mSecondsPerCount = 1.0 / (double)countsPerSec;
}

// Returns the total time elapsed since reset() was called, NOT counting any
// time when the clock is stopped.
float GameTimer::getGameTime()const
{
if( mStopped )
{
return (float)((mStopTime - mBaseTime)*mSecondsPerCount);
}

// The distance mCurrTime - mBaseTime includes paused time,
// which we do not want to count. To correct this, we can subtract
// the paused time from mCurrTime:
else
{
return (float)(((mCurrTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
}
}

float GameTimer::getDeltaTime()const
{
return (float)mDeltaTime;
}

void GameTimer::reset()
{
__int64 currTime;
QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

mBaseTime = currTime;
mPrevTime = currTime;
mStopTime = 0;
mStopped = false;
}

void GameTimer::start()
{
__int64 startTime;
QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


// Accumulate the time elapsed between stop and start.


if( mStopped )
{
mPausedTime += (startTime - mStopTime);	

mPrevTime = startTime;
mStopTime = 0;
mStopped = false;
}
}

void GameTimer::stop()
{
if( !mStopped )
{
__int64 currTime;
QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

mStopTime = currTime;
mStopped = true;
}
}

void GameTimer::tick()
{
if( mStopped )
{
mDeltaTime = 0.0;
return;
}

__int64 currTime;
QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
mCurrTime = currTime;

// Time difference between this frame and the previous.
mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

// Prepare for next frame.
mPrevTime = mCurrTime;

if(mDeltaTime < 0.0)
{
mDeltaTime = 0.0;
}
}