#pragma once

#include <windows.h>

class CTimer
{
public:
	CTimer();
	~CTimer();

	void start();
	void update();
	void reset();
	float getElapsedTime();
	float getTotalTime();
private:
	__int64 m_LastTime;
	__int64 m_CurrentTime;
	__int64 m_Frequency;
	float m_fElapsedTime;
	float m_fTotalTime;
	bool m_bUseHighPrecision;
};