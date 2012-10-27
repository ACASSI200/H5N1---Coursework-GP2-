//--------------------------
// Filename: SoundClass.h
//--------------------------

#ifndef _SOUNDCLASS_H_
#define _ SOUNDCLASS_H_

//--------------------------
// Linking
//--------------------------
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//--------------------------
// Includes
//--------------------------
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundClass{
private:
	struct WaveHeaderType{
		char cChunkId[4];
		unsigned long lChuckSize;
		char cFormat[4];
		char cSubChunkId[4];
		unsigned long lSubChunkSize;
		unsigned short sAudioFormat;
		unsigned short sNumChannels;
		unsigned long lSampleRate;
		unsigned long lBytesPerSecond;
		unsigned short sBlockAlign;
		unsigned short sBitsPerSample;
		char cDataChunkId[4];
		unsigned long lDataSize;
	};

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool bInitialize(HWND);
	void vShutdown();

private: 
	bool bInitializeDirectSound(HWND);
	void vShutdownDirectSound();

	bool bLoadWaveFile(char*, IDirectSoundBuffer8**, IDirectSound3DBuffer8**);
	void vShutdownWaveFile(IDirectSoundBuffer8**, IDirectSound3DBuffer8**);

	bool bPlayWaveFile();

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_PrimaryBuffer;

	//new Listner interface for 3D sound.
	IDirectSound3DListener8* m_Listener;
	IDirectSoundBuffer8* m_secondaryBuffer1;

	//adding a 3D buffer interface for 3D sound manipulation.
	IDirectSound3DBuffer8* m_secondary3DBuffer1;
};

#endif