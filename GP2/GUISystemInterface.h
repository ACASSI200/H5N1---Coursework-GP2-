#pragma once

#include <Rocket/Core/SystemInterface.h>
#include "Timer.h"

class CGUISystemInterface: public Rocket::Core::SystemInterface
{
public:
	CGUISystemInterface();
	virtual ~CGUISystemInterface();
	
	/// Gets the number of seconds elapsed since the start of the application.
	virtual float GetElapsedTime();

	/// Logs the specified message.
	virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);
private:
	CTimer m_Timer;

};