#pragma once

#include <Windows.h>
#include <XInput.h>

class CJoypad
{
public:
	CJoypad(int index);
	~CJoypad();

	void update();

	float getLeftThumbStickX()
	{
		return m_fLeftThumbstickX;
	};

	float getLeftThumbStickY()
	{
		return m_fLeftThumbstickY;
	};

	float getRightThumbStickX()
	{
		return m_fRightThumbstickX;
	};

	float getRightThumbStickY()
	{
		return m_fRightThumbstickY;
	};
private:
	XINPUT_STATE m_JoypadState;
	int m_iIndex;

	float m_fLeftThumbstickX;
	float m_fLeftThumbstickY;
	float m_fLeftThumbstickXNormalized;
	float m_fLeftThumbstickYNormalized;
	float m_fLeftMagnitude;
	float m_fLeftNormalizedMagnitude;

	float m_fRightThumbstickX;
	float m_fRightThumbstickY;
	float m_fRightThumbstickXNormalized;
	float m_fRightThumbstickYNormalized;
	float m_fRightMagnitude;
	float m_fRightNormalizedMagnitude;

	float m_fRightTrigger;
	float m_fLeftTrigger;
};