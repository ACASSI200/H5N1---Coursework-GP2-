#pragma once
#include "Win32Window.h"
#include "Timer.h"
#include <D3D10.h>
#include <D3DX10.h>
#include <fmod.hpp>

#include "GameObjectManager.h"

#include "MaterialComponent.h"
#include "GeometryComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "LightComponent.h"

//Audio - Includes
#include "AudioListenerComponent.h"
#include "AudioSourceComponent.h"
#include "AudioSystem.h"
#include "GameTimer.h"

//Physics
#include "Physics.h"
#include "BodyComponent.h"
#include "BoxCollider.h"
#include "MeshCollider.h"
#include <vector>
#include <string>

using namespace std;

const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);

class CGameApplication:public hkpContactListener 
{
public:
	CGameApplication(void);
	~CGameApplication(void);
	bool init();
	void run();
	GameTimer mTimer;
	std::wstring mFrameStats;
	virtual void drawScene();
	std::wstring mMainWndCaption;

private:
	bool initInput();
	bool initGame();
	//Init Physics, called by our main init method
	bool initGraphics();
	bool initPhysics();
	//Init Audio, called by our main init method
	bool initAudio();
	bool initWindow();
	bool Frame();
	void render();
	void update();

	void loadGame();
	void loadMainMenu();
	void loadExitScreen();
	void updateGame();
	void updateMenu();
	void updateExit();
	bool loadMenuGraphics();
	
	void contactPointCallback (const hkpContactPointEvent &event); 

protected:
	//Graphics
	ID3D10Device * m_pD3D10Device;
	IDXGISwapChain * m_pSwapChain;
	ID3D10RenderTargetView * m_pRenderTargetView;
	ID3D10DepthStencilView * m_pDepthStencelView;
	ID3D10Texture2D *m_pDepthStencilTexture;
	ID3DX10Font* mFont;
	D3DXCOLOR mClearColor;

	CWin32Window * m_pWindow;

	CTimer m_Timer;

	
};