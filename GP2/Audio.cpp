#include "Audio.h"

void Audio::initSound(){
	result = System_Create(&fmodSystem);
	
	result = fmodSystem->init(NUM_CHANNELS, FMOD_INIT_NORMAL, 0);
}

void Audio::updateSound(){
	result = fmodSystem->update();
}


void Audio::loadSound(){
	result = fmodSystem->createSound("sounds/enterGame.mp3", FMOD_DEFAULT, 0, &sound1);
	sound1->setMode(FMOD_LOOP_OFF);


	result = fmodSystem->createStream("sounds/soundtrack.mp3", FMOD_DEFAULT, 0, &soundtrack);
	soundtrack->setMode(FMOD_LOOP_NORMAL);

}

void Audio::playSoundtrack(){
	result = fmodSystem->playSound(soundtrack, FMOD_DEFAULT, false, &channel);
}

void Audio::EnterGame(){
	result = fmodSystem->playSound(sound1, FMOD_DEFAULT, false, &channel);
}
//
//void Audio::playSound2(){
//	result = fmodSystem->playSound(sound2, FMOD_DEFAULT, false, &channel);
//}

