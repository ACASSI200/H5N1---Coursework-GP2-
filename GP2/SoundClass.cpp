//--------------------------
// Filename: SoundClass.cpp
//--------------------------

#include "soundclass.h"

SoundClass::SoundClass(){
	m_DirectSound = 0;
	m_PrimaryBuffer = 0;

	//Initialize the listener interface to null in the class constructor.
	m_Listener = 0;
	m_secondaryBuffer1 = 0;

	//Initialize the 3D secondary sound buffer interface to null in the class constructor.
	m_secondary3DBuffer1 = 0;
}

SoundClass::SoundClass(const SoundClass& other){

}

SoundClass::~SoundClass(){

}

bool SoundClass::bInitialize(HWND hwnd){
	bool result;

	//Initialize direct sound and the primary sound buffer.
	result = bInitializeDirectSound(hwnd);
	if(!result){
		return false;
	}

	//now when i load a 3D sound it needs both the secondary sound buffer and the secondery 3D
	//sound buffer interface as both are now used for controlling the sound buffer.

	//Load a wave audio file onto a scondary buffer.
	result = bLoadWaveFile("../Engine/data/sound02.wav", &m_secondaryBuffer1, &m_secondary3DBuffer1);
	if(!result){
		return false;
	}

	////Play the wave file now that it has been loaded.
	result = bPlayWaveFile();
	if(!result){
		return false;
	}

	return true;
}

void SoundClass::vShutdown(){
	//function now releases both the secondary sound buffer and the secondary 3D sound Buffer interface.
	
	//Release the secondary buffer.
	vShutdownWaveFile(&m_secondaryBuffer1, &m_secondary3DBuffer1);

	//Shutdown the direct sound API.
	vShutdownDirectSound();

	return;
}

bool SoundClass::bInitializeDirectSound(HWND hwnd){

	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	//Initialize the direct sound interface pointer for the default sound device.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if(FAILED(result)){
		return false;
	}

	//Primary sound buffer has been modified to use the DSBCAPS_CTRL3D in the description so that when the buffer is created it now has 3D capabilities.

	//Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_PrimaryBuffer, NULL);
	if(FAILED(result)){
		return false;
	}

	// The format of the primary buffer stays the same, only secondary buffers are required to be mono.
	//Direct sound will take care of mixing different format secondary buffer into the primary buffer automatically.

	//Setup the format of the primary sound buffer.
	//In this case it is a .WAV file recoreded at 44, 100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//Set the primary buffer to be the wave format specified.
	result = m_PrimaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result)){
		return false;
	}

	//Once the primary buffer is created we can obtain a listener interface from it.
	//This will allow us to position the listener in 3D space in relation to the other 3D positioned sounds.
	
	//Obtain a listener interface.
	result = m_PrimaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*) &m_Listener);
	if(FAILED(result)){
		return false;
	}

	//To start we set the initial position of the listener at the origin of the world. 
	//The dS3D_IMMEDIATE parameter tells DirectSound to set it right away and not to defer it for batch processing later.

	//Set the initial position of the listener to be in the middle of the scene.
	m_Listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	return true;
}

void SoundClass::vShutdownDirectSound(){

	//The new listener interface is released in the ShutdownDirectSound Function.

	//Release the Listener interface.
	if(m_Listener){
		m_Listener->Release();
		m_Listener = 0;
	}

	//Release the primary sound buffer pointer.
	if(m_PrimaryBuffer){
		m_PrimaryBuffer->Release();
		m_PrimaryBuffer = 0;
	}

	if(m_DirectSound){
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}
//The LoadWaveFile function also takes as input an IDirectSound3DBuffer8 interface pointer.
bool SoundClass::bLoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer){

	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	//Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0){
		return false;
	}

	//Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1){
		return false;
	}

	//Check that the chunk ID is the RIFF format.
	if((waveFileHeader.cChunkId[0]) != 'R' || (waveFileHeader.cChunkId[1] != 'I') ||
		(waveFileHeader.cChunkId[2]) != 'F' || (waveFileHeader.cChunkId[3] != 'F')){
			return false;
	}

	// Check that the file format is the WAVE format.
	if((waveFileHeader.cFormat[0] != 'W') || (waveFileHeader.cFormat[1] != 'A') ||
		(waveFileHeader.cFormat[2] != 'V') || (waveFileHeader.cFormat[3] != 'E')){
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if((waveFileHeader.cSubChunkId[0] != 'f') || (waveFileHeader.cSubChunkId[1] != 'm') ||
	   (waveFileHeader.cSubChunkId[2] != 't') || (waveFileHeader.cSubChunkId[3] != ' ')){
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if(waveFileHeader.sAudioFormat != WAVE_FORMAT_PCM){
		return false;
	}

	//3D sound files must be recorded as single channel (mono).

	//Check that the wave file was recorded in mono format.
	if(waveFileHeader.sNumChannels != 1){
		return false;
	}

	//check that the wave file was recorded at a sample rate of 44.1 KHz.
	if(waveFileHeader.lSampleRate != 44100){
		return false;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if(waveFileHeader.sBitsPerSample != 16)
	{
		return false;
	}

	// Check for the data chunk header.
	if((waveFileHeader.cDataChunkId[0] != 'd') || (waveFileHeader.cDataChunkId[1] != 'a') ||
	   (waveFileHeader.cDataChunkId[2] != 't') || (waveFileHeader.cDataChunkId[3] != 'a'))
	{
		return false;
	}

	//The secondary buffer format uses single channel now for 3D sounds instead of dual channel (stereo).

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//The buffer will require 3D capabilities. We use the DSBCAPS_CTRL3D bitwised with the other options for the dwFlags parameter.

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader.lDataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.lDataSize];
	if(!waveData)
	{
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.lDataSize, filePtr);
	if(count != waveFileHeader.lDataSize)
	{
		return false;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.lDataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result))
	{
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.lDataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if(FAILED(result))
	{
		return false;
	}
	
	// Release the wave data since it was copied into the secondary buffer.
	delete [] waveData;
	waveData = 0;

	// Get the 3D interface to the secondary sound buffer.
	result = (*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundClass::vShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer){
	// Release the 3D interface to the secondary sound buffer.
	if(*secondary3DBuffer)
	{
		(*secondary3DBuffer)->Release();
		*secondary3DBuffer = 0;
	}

	// Release the secondary sound buffer.
	if(*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

bool SoundClass::bPlayWaveFile(){

	HRESULT result;
	float positionX, positionY, positionZ;
//Setup the position where we want the 3D sound to be located. In this case it will be set over to the left.

	// Set the 3D position of where the sound should be located.
	positionX = -2.0f;
	positionY = 0.0f;
	positionZ = 0.0f;

	// Set position at the beginning of the sound buffer.
	result = m_secondaryBuffer1->SetCurrentPosition(0);
	if(FAILED(result))
	{
		return false;
	}

	// Set volume of the buffer to 100%.
	result = m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
	{
		return false;
	}
//Now use the position and set the 3D sound position using the 3D interface. The DS3D_IMMEDIATE flag sets the sound position immediately instead of deferring it for later batch processing.

	// Set the 3D position of the sound.
	m_secondary3DBuffer1->SetPosition(positionX, positionY, positionZ, DS3D_IMMEDIATE);

	// Play the contents of the secondary sound buffer.
	result = m_secondaryBuffer1->Play(0, 0, 0);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}



