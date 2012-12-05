#include "GUISystemInterface.h"

CGUISystemInterface::CGUISystemInterface()
{
	m_Timer.start();
}

CGUISystemInterface::~CGUISystemInterface()
{
}
	
/// Gets the number of seconds elapsed since the start of the application.
float CGUISystemInterface::GetElapsedTime()
{
	return m_Timer.getElapsedTime();
}

/// Logs the specified message.
bool CGUISystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
	OutputDebugStringA(message.CString());
	return false;
}