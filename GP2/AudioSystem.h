#include <fmod.hpp>

#include <string>

using namespace FMOD;
using namespace std;

class CAudioSystem
{
public:
	~CAudioSystem();
	static CAudioSystem& getInstance()
	{
		static CAudioSystem instance;
		return instance;
	};
	//Init audio system
	void init();
	//update
	void update();
	//load sound, should it be a streaming sound(music)
	Sound * loadSound(const string& filename, bool stream);
	//play sound, return the channel which can control the system
	Channel *playSound(Sound * pSound,int loopCount=0);
	//Update the listener
	void updateListener(FMOD_VECTOR &position, FMOD_VECTOR &vel, FMOD_VECTOR &forward, FMOD_VECTOR &up);
private:
	CAudioSystem();

	CAudioSystem(CAudioSystem const&){};
	void operator=(CAudioSystem const&){}; 
private:
	//Audio System, needed to play
	FMOD::System * m_pAudioSystem;
};