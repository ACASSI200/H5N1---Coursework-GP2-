#include "AudioSourceComponent.h"
#include "AudioSystem.h"

CAudioSourceComponent::CAudioSourceComponent()
{
	m_pSound=NULL;
	m_bStream=false;
	m_pChannel=NULL;
	m_strName="AudioSourceComponent";
}

CAudioSourceComponent::~CAudioSourceComponent()
{
	//release the sound
	if (m_pSound)
	{
		m_pSound->release();
		m_pSound=NULL;
	}
}

void CAudioSourceComponent::init()
{
	//Create the sound
	m_pSound=CAudioSystem::getInstance().loadSound(m_strFilename,m_bStream);
	
}

void CAudioSourceComponent::play(int loopCount)
{
	//play the sound, we get a channel which can be used to play, stop
	//pause, control the volume of the sound
	m_pChannel=CAudioSystem::getInstance().playSound(m_pSound,loopCount);
}

void CAudioSourceComponent::stop()
{
	//Use the channel to stop the sound
	if (m_pChannel)
	{
		m_pChannel->stop();
	}
}