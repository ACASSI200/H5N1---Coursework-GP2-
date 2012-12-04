#pragma once

#include "IComponent.h"

#include <fmod.hpp>

#include <string>

using namespace FMOD;
using namespace std;

class CAudioSourceComponent:public CBaseComponent
{
public:
	CAudioSourceComponent();
	~CAudioSourceComponent();

	//will do the actual loading
	void init();

	//set the filename, this has to be done just after
	//we create the component
	void setFilename(const string& name)
	{
		m_strFilename=name;
	};

	//Set the sound to stream or not, done after we the create
	//the component
	void setStream(bool stream)
	{
		m_bStream=stream;
	};

	//play
	void play(int loopCount=0);
	//stop
	void stop();
private:
	string m_strFilename;
	//The sound we are playing
	Sound *m_pSound;
	//channel to control the sound
	Channel *m_pChannel;
	bool m_bStream;

};