#pragma once
#include "Win32Window.h"
#include "Timer.h"
#include <D3D10.h>
#include <D3DX10.h>

#include "GameObjectManager.h"

#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "DirectionLightComponent.h"
#include "MeshComponent.h"

#include "ModelLoader.h"

#include "Fpsclass.h"
#include "Cpuclass.h"
#include "Timerclass.h"
#include "GameTimer.h"


#include <vector>
#include <string>

using namespace std;
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);

const float INFINITY = FLT_MAX;
const float PI       = 3.14159265358979323f;
const float MATH_EPS = 0.0001f;
//std::wstring mFrameStats;

class CGameApplication
{
public:
	CGameApplication(void);
	~CGameApplication(void);
	bool init();
	void Shutdown();
	void run();
	ID3D10Device* GetDevice();
	virtual void drawScene(); 
private:
	bool initInput();
	bool initGame();
	bool initGraphics();
	bool initWindow();
	bool Frame();
	void render();
	void update();


	

private:
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
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_FPSTimer;
	
	
	GameTimer mTimer;
	std::wstring mFrameStats;
	
	
	//Get Game Object Manager
	CGameObjectManager *m_pGameObjectManager;

	CModelLoader modelloader;
};