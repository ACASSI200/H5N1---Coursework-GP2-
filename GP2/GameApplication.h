#pragma once
#include "Win32Window.h"
#include "Timer.h"
#include <D3D10.h>
#include <D3DX10.h>

#include "GameObjectManager.h"

#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "GeometryComponent.h"
#include "CameraComponent.h"
#include "DirectionLightComponent.h"
#include "MeshComponent.h"
#include "Audio.h"


#include "ModelLoader.h"
#include "GUIManager.h"
#include "GameTimer.h"


#include <vector>
#include <Common\Base\hkBase.h>
#include <cstring>
#include <sstream>

//Physics
#include "Physics.h"
#include "BodyComponent.h"
#include "BoxCollider.h"


using namespace std;

const D3DXCOLOR WHITE(0.5f, 1.0f, 0.5f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 0.5f, 0.0f, 1.0f);

//Game States
enum GameState
{
	MAINMENU,
	GAMEPLAYING,
	PAUSE,
	EXIT
};



class CGameApplication:public hkpContactListener 
{
public:
	CGameApplication(void);
	~CGameApplication(void);
	bool initAudio();
	bool init();
	void run();

	string convertFloat(float number)
	{
		std::ostringstream buff;
		buff<<number;
		return buff.str();
	}

		GameTimer mTimer;
		std::wstring mFrameStats;

private:
	bool initInput();
	bool initGame();
	bool initGraphics();
	bool initPhysics();
	bool initGUI();
	bool initWindow();	
	void render();
	void update();
	
	void contactPointCallback (const hkpContactPointEvent &event);



private:
	//Graphics
	ID3D10Device * m_pD3D10Device;
	IDXGISwapChain * m_pSwapChain;
	ID3D10RenderTargetView * m_pRenderTargetView;
	ID3D10DepthStencilView * m_pDepthStencelView;
	ID3D10Texture2D *m_pDepthStencilTexture;
	CCameraComponent *pCamera;
	ID3DX10Font* mFont;
	ID3D10InputLayout* mVertexLayout;
	D3DXCOLOR mClearColor;

	CWin32Window * m_pWindow;

	CTimer m_Timer;

	Audio *audio;
	
	//Get Game Object Manager
	CGameObjectManager *m_pGameObjectManager;

	CModelLoader modelloader;

	Rocket::Core::ElementDocument *m_pMainMenu;
	Rocket::Core::ElementDocument *m_pGameGUI;
	Rocket::Core::ElementDocument *m_pPauseGUI;
	
	GameState m_GameState;
	

};