#include "Joypad.h"
#include <math.h>

CJoypad::CJoypad(int index)
{
	m_iIndex=index;
	ZeroMemory( &m_JoypadState, sizeof(XINPUT_STATE) );
}

CJoypad::~CJoypad()
{
}

void CJoypad::update()
{
	ZeroMemory( &m_JoypadState, sizeof(XINPUT_STATE) );
    DWORD dwResult= XInputGetState( m_iIndex, &m_JoypadState );
	if( dwResult == ERROR_SUCCESS )
	{ 
		//controller connected
		m_fLeftThumbstickX=m_JoypadState.Gamepad.sThumbLX;
		m_fLeftThumbstickY=m_JoypadState.Gamepad.sThumbLY;

		m_fLeftMagnitude=sqrt(m_fLeftThumbstickX*m_fLeftThumbstickX + m_fLeftThumbstickY*m_fLeftThumbstickY);
		m_fLeftThumbstickXNormalized=m_fLeftThumbstickX/m_fLeftMagnitude;
		m_fLeftThumbstickYNormalized=m_fLeftThumbstickY/m_fLeftMagnitude;
		if (m_fLeftMagnitude>XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			if (m_fLeftMagnitude>32767)
				m_fLeftMagnitude=32767;
			m_fLeftMagnitude-=XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			m_fLeftNormalizedMagnitude=m_fLeftMagnitude/(32767-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		}
		else
		{
			m_fLeftMagnitude=0.0f;
			m_fLeftNormalizedMagnitude=0.0f;
		}

		m_fRightThumbstickX=m_JoypadState.Gamepad.sThumbRX;
		m_fRightThumbstickY=m_JoypadState.Gamepad.sThumbRY;

		m_fRightMagnitude=sqrt(m_fRightThumbstickX*m_fRightThumbstickX + m_fRightThumbstickY*m_fRightThumbstickY);
		m_fRightThumbstickXNormalized=m_fRightThumbstickX/m_fRightMagnitude;
		m_fRightThumbstickYNormalized=m_fRightThumbstickY/m_fRightMagnitude;
		if (m_fRightMagnitude>XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			if (m_fRightMagnitude>32767)
				m_fRightMagnitude=32767;
			m_fRightMagnitude-=XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
			m_fRightNormalizedMagnitude=m_fRightMagnitude/(32767-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}
		else
		{
			m_fRightMagnitude=0.0f;
			m_fRightNormalizedMagnitude=0.0f;
		}

		//triggers
		if (m_JoypadState.Gamepad.bRightTrigger>XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			m_fRightTrigger=((float)m_JoypadState.Gamepad.bRightTrigger/256.0f);
		}
		else
			m_fRightTrigger=0.0f;
		if (m_JoypadState.Gamepad.bRightTrigger>XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			m_fLeftTrigger=((float)m_JoypadState.Gamepad.bLeftTrigger/256.0f);
		}
		else
			m_fLeftTrigger=0.0f;
	}else{
		// Controller is not connected 
	}
}