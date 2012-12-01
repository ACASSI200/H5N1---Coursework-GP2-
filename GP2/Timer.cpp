#include "Timer.h"

CTimer::CTimer()
{
	m_LastTime=0;
	m_CurrentTime=0;
	m_bUseHighPrecision=false;
	m_fElapsedTime=0.0f;
	m_fTotalTime=0.0f;
}



CTimer::~CTimer()
{

}

void CTimer::start()
{
	if (QueryPerformanceCounter((LARGE_INTEGER*)&m_LastTime))
	{
		m_bUseHighPrecision=true;
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	}
	else
	{
		//use get time
	}
}

void CTimer::reset()
{
	m_fElapsedTime=0.0f;
	m_fTotalTime=0.0f;
}

void CTimer::update()
{
	if (m_bUseHighPrecision)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTime);
		m_fElapsedTime=(float)((m_CurrentTime-m_LastTime))*1.0f/(float)m_Frequency;
		m_LastTime=m_CurrentTime;
		m_fTotalTime+=m_fElapsedTime;
	}
	else
	{

	}
}


static const int NUM_FPS_SAMPLES = 30;
float fpsSamples[NUM_FPS_SAMPLES];
int currentSample = 0;
int count = 0;
float fps = 0;

float CTimer::FPS(float timer){
	
	count++;
	if(count % 180 == 0){
		fpsSamples[currentSample % NUM_FPS_SAMPLES] = 1.0f / m_fElapsedTime;
		
		for (int i = 0; i < NUM_FPS_SAMPLES; i++)
			fps += fpsSamples[i];
		fps /= NUM_FPS_SAMPLES;

		return fps;
	}
    
	return fps;
}

float CTimer::getElapsedTime()
{
	return m_fElapsedTime;
}

float CTimer::getTotalTime()
{
	return m_fTotalTime;
}