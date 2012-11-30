#pragma once
#include "Win32Window.h"
#include "Timer.h"
#include <D3D10.h>
#include <D3DX10.h>
#include<D3DX10core.h>
#include "GameObjectManager.h"


#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "GeometryComponent.h"
#include "CameraComponent.h"

#include "DirectionLightComponent.h"
#include "MeshComponent.h"

#include "Physics.h"
#include "BodyComponent.h"
#include "BoxCollider.h"

#include <vector>

using namespace std;

enum GameState
{
	MAINMENU=0,
	GAME,
	EXIT,
};

class CGameApplication
{
public:
	CGameApplication(void);
	~CGameApplication(void);
	bool init();
	void run();
	
private:

	void loadGame();
	void loadMainMenu();
	void loadExitScreen();

	void updateGame();
	void updateMenu();
	void updateExit();

	bool initInput();
	bool initGame();
	bool initPhysics();
	bool initGraphics();
	bool initWindow();
	bool loadMenuGraphics();

	void render();
	void update();
	
private:
	//Graphics
	ID3D10Device * m_pD3D10Device;
	IDXGISwapChain * m_pSwapChain;
	ID3D10RenderTargetView * m_pRenderTargetView;
	ID3D10DepthStencilView * m_pDepthStencelView;
	ID3D10Texture2D *m_pDepthStencilTexture;
	CWin32Window * m_pWindow;
	CTimer m_Timer;
	//Get Game Object Manager
	CGameObjectManager *m_pGameObjectManager;
	GameState m_GameState;
};