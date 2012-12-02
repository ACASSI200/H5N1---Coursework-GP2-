#include "AudioSystem.h"

#include <string.h>

CAudioSystem::CAudioSystem()
{
	//set to NULL(0)
	m_pAudioSystem=0;
}

CAudioSystem::~CAudioSystem()
{
	//Release the audio system
	if (m_pAudioSystem)
	{
		m_pAudioSystem->release();
		m_pAudioSystem=0;
	}
}

void CAudioSystem::init()
{
	//This setup is directly taken from the Getting Started with FMOD for Windows
	//Page 8 titled - Recommended startup sequence (IMPORTANT!)
	//temp values
	FMOD_RESULT result;
	//version of FMD
	unsigned int version;
	//number of audio drivers
	int numdrivers;
	//speaker mode
	FMOD_SPEAKERMODE speakermode;
	//The capabilities of the audio device
	FMOD_CAPS caps;
	//holds the name of the driver
	char name[256];

	//Create audio system
	result = FMOD::System_Create(&m_pAudioSystem);
	
	//check version
	result = m_pAudioSystem->getVersion(&version);
	if (version < FMOD_VERSION)
	{
		//Wrong version of FMOD
		return;
	}
	//Get number of drivers
	result = m_pAudioSystem->getNumDrivers(&numdrivers);
	if (numdrivers == 0)
	{
		result = m_pAudioSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}
	else
	{
		//Get driver caps and speaker mode, this will get the setup of the speakers
		//from the OS
		result = m_pAudioSystem->getDriverCaps(0, &caps, 0, &speakermode);
		result = m_pAudioSystem->setSpeakerMode(speakermode);
		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			/*
			The user has the 'Acceleration' slider set to off! This is really bad
			for latency! You might want to warn the user about this.
			*/
			result = m_pAudioSystem->setDSPBufferSize(1024, 10);
		}
		//get the driver name
		result = m_pAudioSystem->getDriverInfo(0, name, 256, 0);
		if (strstr(name, "SigmaTel"))
		{
			/*
			Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			PCM floating point output seems to solve it.
			*/
			result = m_pAudioSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
		}
		//finally init FMOD
		result = m_pAudioSystem->init(100, FMOD_INIT_NORMAL, 0);
		if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
		{
			/*
			Ok, the speaker mode selected isn't supported by this soundcard. Switch it
			back to stereo...
			*/
			result = m_pAudioSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
			/*
			... and re-init.
			*/
			result = m_pAudioSystem->init(100, FMOD_INIT_NORMAL, 0);
		}
	}
}

Sound * CAudioSystem::loadSound(const string& filename, bool stream)
{
	//load or stream sound
	FMOD_RESULT result;
	Sound *pSound;
	if (stream)
		result = m_pAudioSystem->createSound(filename.c_str(), FMOD_DEFAULT, 0, &pSound);
	else
		result = m_pAudioSystem->createStream(filename.c_str(), FMOD_DEFAULT, 0, &pSound);

	return pSound;
}

//Number of times to loop before stopping. 0 = oneshot. 1 = loop once then stop. -1 = loop forever. Default = -1 
Channel * CAudioSystem::playSound(Sound * pSound,int loopCount)
{
	//play sound, return a channel so we can control the sound
	//while its playing
	Channel *pChannel;
	m_pAudioSystem->playSound(FMOD_CHANNEL_FREE,pSound,false,&pChannel);
	pChannel->setLoopCount(loopCount);

	return pChannel;
}

void CAudioSystem::updateListener(FMOD_VECTOR &position, FMOD_VECTOR &vel, FMOD_VECTOR &forward, FMOD_VECTOR &up)
{
	//update the listener
	m_pAudioSystem->set3DListenerAttributes(0,&position,&vel,&forward,&up);
}

void CAudioSystem::update()
{
	//update the audio system, this will have to be called every frame
	m_pAudioSystem->update();
}

