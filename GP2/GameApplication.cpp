#include "GameApplication.h"
#include "GameObject.h"

#include "ModelLoader.h"
#include "Input.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <string>
#include <sstream>

CGameApplication::CGameApplication(void)
{
	m_pWindow=NULL;
	m_pD3D10Device=NULL;
	m_pRenderTargetView=NULL;
	m_pSwapChain=NULL;
	m_pDepthStencelView=NULL;
	m_pDepthStencilTexture=NULL;
	playerHealth = 100;
	
	mFrameStats = L" ";
	mClearColor     = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	m_camState = DEBUG;
}

CGameApplication::~CGameApplication(void)
{
	if (m_pD3D10Device)
		m_pD3D10Device->ClearState();

	CGameObjectManager::getInstance().clear();

	CPhysics::getInstance().destroy();
	
	if (m_pRenderTargetView)
		m_pRenderTargetView->Release();
	if (m_pDepthStencelView)
		m_pDepthStencelView->Release();
	if (m_pDepthStencilTexture)
		m_pDepthStencilTexture->Release();
	if (m_pSwapChain)
		m_pSwapChain->Release();
	if (m_pD3D10Device)
		m_pD3D10Device->Release();
	if (m_pWindow)
	{
		delete m_pWindow;
		m_pWindow=NULL;
	}

	
}

bool CGameApplication::init()
{
	if (!initWindow())
		return false;
	if (!initGraphics())
		return false;
	if (!initInput())
		return false;
	if (!initAudio())
		return false;
	if (!initPhysics())
		return false;
	if (!initGame())
		return false;
	return true;
}
void CGameApplication::createBox(float x,float y,float z)
{
	//Create Game Object
	CGameObject *EnemyObject=new CGameObject();
	EnemyObject->getTransform()->setPosition(x,y,z);
	//Set the name
	EnemyObject->setName("TestCube");
	
	//create material
	CMaterialComponent *EnemyMaterial=new CMaterialComponent();
	EnemyMaterial->SetRenderingDevice(m_pD3D10Device);
	EnemyMaterial->setEffectFilename("Texture.fx");

	//Create geometry
	CModelLoader modelloader;
	//CGeometryComponent *pGeometry=modelloader.loadModelFromFile(m_pD3D10Device,"humanoid.fbx");
	CGeometryComponent *EnemyGeom=modelloader.createCube(m_pD3D10Device,1.0f,1.0f,1.0f);
	
	//create a box collider, this could be any collider
	CBoxCollider *EnemBox=new CBoxCollider();
	//set the size of the box
	EnemBox->setExtents(1.0f,1.0f,1.0f);
	//add collider
	EnemyObject->addComponent(EnemBox);

	//create body
	CBodyComponent *pBody=new CBodyComponent();
	EnemyObject->addComponent(pBody);

	EnemyGeom->SetRenderingDevice(m_pD3D10Device);
	//Add component
	EnemyObject->addComponent(EnemyMaterial);
	EnemyObject->addComponent(EnemyGeom);
	//add the game object
	CGameObjectManager::getInstance().addGameObject(EnemyObject);
}



void CGameApplication::contactPointCallback (const hkpContactPointEvent &event)
{
	//Called when a collision occurs
	hkpRigidBody *pBody1=event.getBody(0);
	hkpRigidBody *pBody2=event.getBody(1);


	CGameObject *pGameObject1=(CGameObject*)pBody1->getUserData();
	CGameObject *pGameObject2=(CGameObject*)pBody2->getUserData();
	//CGameObject *pGameObject3=(CGameObject*)pBody3->getUserData();
	//Do something with the game objects

	//playerHealth --;
}

bool CGameApplication::initGame()
{

	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 24;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    wcscpy(fontDesc.FaceName, L"Times New Roman");

	D3DX10CreateFontIndirect(m_pD3D10Device, &fontDesc, &mFont);

    // Set primitive topology, how are we going to interpet the vertices in the vertex buffer - BMD
    //http://msdn.microsoft.com/en-us/library/bb173590%28v=VS.85%29.aspx - BMD
    m_pD3D10Device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );	

	//===============================================================
	//							SCENERY OBJECT
	//===============================================================
	//Create Game Object
	CGameObject *pTestGameObject=new CGameObject();
	//Set the name
	pTestGameObject->setName("Test");
	
	//create material
	CMaterialComponent *pMaterial=new CMaterialComponent();
	pMaterial->SetRenderingDevice(m_pD3D10Device);
	pMaterial->setEffectFilename("Texture.fx");
	pMaterial->loadDiffuseTexture("rockwall.png");
	

	//Audio - Create our Audio Component
	CAudioSourceComponent *pAudio=new CAudioSourceComponent();
	//Audio - If its a wav file, you should not stream
	pAudio->setFilename("gunshot.wav");
	//Audio - stream set to false
	pAudio->setStream(false);
	//Audio - Add it to the Game Object
	pTestGameObject->addComponent(pAudio);

	//Create geometry
	CModelLoader modelloader;
	//CGeometryComponent *pGeometry=modelloader.loadModelFromFile(m_pD3D10Device,"CityScene.fbx");
	pTestGameObject->getTransform()->setPosition(0.0f,0.0f,0.0f);
	//pTestGameObject->getTransform()->rotate(90.0f,0.0f,0.0f);
	CGeometryComponent *pGeometry=modelloader.createCube(m_pD3D10Device,200.0f,2.0f,200.0f);
	CBoxCollider *pBox=new CBoxCollider();
	CMeshCollider *pMesh = new CMeshCollider();
	//float verts = pGeometry->getNumberOfVertices();

	pBox->setExtents(200.0f,1.0f,200.0f);

//	pBox->physicsShape()->isConvex();
	pTestGameObject->addComponent(pBox);

	//create body make it fixed so no gravity effects it
	CBodyComponent *pBody=new CBodyComponent();
	pBody->setFixed(true);
	pTestGameObject->addComponent(pBody);

	pGeometry->SetRenderingDevice(m_pD3D10Device);
	//Add component
	pTestGameObject->addComponent(pMaterial);
	pTestGameObject->addComponent(pGeometry);
	//add the game object
	CGameObjectManager::getInstance().addGameObject(pTestGameObject);
	//===============================================================
	
	//===============================================================
	//							PLAYER OBJECT
	//===============================================================
	CGameObject *pPlayer=new CGameObject();
	//Set the name
	pPlayer->setName("Player");
	//create material
	CMaterialComponent *pPlayerMaterial=new CMaterialComponent();
	pPlayerMaterial->SetRenderingDevice(m_pD3D10Device);
	
	pPlayerMaterial->setEffectFilename("Texture.fx");
	pPlayerMaterial->loadDiffuseTexture("face.png");
	//Audio - Create our Audio Component
	CAudioSourceComponent *pPlayerAudio=new CAudioSourceComponent();
	//Audio - If its a wav file, you should not stream
	pPlayerAudio->setFilename("gunshot.wav");
	//Audio - stream set to false
	pPlayerAudio->setStream(false);
	//Audio - Add it to the Game Object
	pPlayer->addComponent(pPlayerAudio);

	//Create geometry
	CModelLoader Playermodelloader;
	CGeometryComponent *pPlayerGeometry=modelloader.loadModelFromFile(m_pD3D10Device,"armoredrecon.fbx");
	//CGeometryComponent *pPlayerGeometry=Playermodelloader.createCube(m_pD3D10Device,1.0f,1.0f,1.0f);
	pPlayer->getTransform()->setPosition(10.0f,20.0f,0.0f);

	//create box
	CBoxCollider *pPlayerBox=new CBoxCollider();

	pPlayerBox->setExtents(1.0f,1.0f,1.0f);
	pPlayer->addComponent(pPlayerBox);

	//create body make it fixed so no gravity effects it
	CBodyComponent *pPlayerBody=new CBodyComponent();
	pPlayerBody->setFixed(false);
	
	pPlayer->addComponent(pPlayerBody);

	pPlayerGeometry->SetRenderingDevice(m_pD3D10Device);
	//Add component
	pPlayer->addComponent(pPlayerMaterial);
	pPlayer->addComponent(pPlayerGeometry);
	//add the game object
	CGameObjectManager::getInstance().addGameObject(pPlayer);
	//=========================================================

	//=========================================================
	//					ENEMY
	//=========================================================

	float startY=10.0f;
	for (int i=0;i<10;i++)
	{
		//call create bi=ox
		createBox(0.0f,(10.0f*i)+startY,0.0f);
	}
	//=========================================================

	CGameObjectManager::getInstance().init();

	//Audio - play music audio source
	//pMusic->play();
	
	m_Timer.start();

	return true;
}

void CGameApplication::run()
{
	mTimer.reset();
	while(m_pWindow->running())
	{
		if (! m_pWindow->checkForWindowMessages())
		{
			update();
			render();
			mTimer.tick();
		}
	}
}

void CGameApplication::DebugCam()
{

			D3D10_VIEWPORT vp;
			UINT numViewports=1;
			m_pD3D10Device->RSGetViewports(&numViewports,&vp);

			CAudioSourceComponent *pMusic=new CAudioSourceComponent();
			CGameObject *pCameraGameObject=new CGameObject();
			pCameraGameObject->getTransform()->setPosition(0.0f,0.0f,-50.0f);
			pCameraGameObject->setName("Camera");
			CCameraComponent *pCamera=new CCameraComponent();
			pCamera->setUp(0.0f,1.0f,0.0f);
			pCamera->setLookAt(0.0f,0.0f,0.0f);
			pCamera->setFOV(D3DX_PI*0.25f);
			pCamera->setAspectRatio((float)(vp.Width/vp.Height));
			pCamera->setFarClip(1000.0f);
			pCamera->setNearClip(0.1f);
			pCameraGameObject->addComponent(pCamera);
			//pCameraGameObject->getTransform()->setPosition(0.0f,2.0f,-50.0f);
			CGameObject *pLightGameObject=new CGameObject();
			//Audio - Create another audio component for music
			//Audio -If it is an mp3 or ogg then set stream to true
			pMusic->setFilename("Music.ogg");
			//Audio - stream to true
			pMusic->setStream(true);
			//Audio - Add to camera, don't call play until init has been called
			pCameraGameObject->addComponent(pMusic);
			//Audio - Attach a listener to the camera
			CAudioListenerComponent *pListener=new CAudioListenerComponent();
			pCameraGameObject->addComponent(pListener);	
			CGameObjectManager::getInstance().addGameObject(pCameraGameObject);



}

void CGameApplication::PlayerCam()
{
			D3D10_VIEWPORT vp;
			UINT numViewports=1;
			m_pD3D10Device->RSGetViewports(&numViewports,&vp);
			float playerX = CGameObjectManager::getInstance().findGameObject("Player")->getTransform()->getPosition().x;
			float playerY =  CGameObjectManager::getInstance().findGameObject("Player")->getTransform()->getPosition().y;
			float playerZ = CGameObjectManager::getInstance().findGameObject("Player")->getTransform()->getPosition().z;
			CGameObject *pPlayerCam = new CGameObject();
			pPlayerCam->getTransform()->setPosition(playerX+5.0f,playerY+5.0f,playerZ);
			pPlayerCam->setName("PlayerCam");
			CCameraComponent *pPlayerCamComp = new CCameraComponent();
			
			pPlayerCamComp->setUp(0.0f,1.0f,0.0f);
			//pPlayerCamComp->setLookAt(0.0f,0.0f,0.0f);
			pPlayerCamComp->setFOV(D3DX_PI*0.25f);
			pPlayerCamComp->setAspectRatio((float)(vp.Width/vp.Height));
			pPlayerCamComp->setFarClip(500.0f);
			pPlayerCamComp->setNearClip(0.1f);
			CAudioListenerComponent *pPlayerListner=new CAudioListenerComponent();
			pPlayerCam->addComponent(pPlayerCamComp);
			pPlayerCam->addComponent(pPlayerListner);
			CGameObjectManager::getInstance().addGameObject(pPlayerCam);

}

void CGameApplication::render()
{
	m_pD3D10Device->OMSetDepthStencilState(0, 0);
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pD3D10Device->OMSetBlendState(0, blendFactor, 0xffffffff);
    //m_pD3D10Device->IASetInputLayout(mVertexLayout);
    m_pD3D10Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Just clear the backbuffer, colours start at 0.0 to 1.0
	// Red, Green , Blue, Alpha - BMD
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; 
	//Clear the Render Target
	//http://msdn.microsoft.com/en-us/library/bb173539%28v=vs.85%29.aspx - BMD
    m_pD3D10Device->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
	m_pD3D10Device->ClearDepthStencilView(m_pDepthStencelView,D3D10_CLEAR_DEPTH,1.0f,0);
	//We need to iterate through all the Game Objects in the managers
	for(vector<CGameObject*>::iterator iter=CGameObjectManager::getInstance().getBegining();
		iter!=CGameObjectManager::getInstance().getEnd();iter++)
	{
		//grab the transform
		CTransformComponent *pTransform=(*iter)->getTransform();
		//and the geometry
		CGeometryComponent *pGeometry=static_cast<CGeometryComponent*>((*iter)->getComponent("GeometryComponent"));
		//and the material
		CMaterialComponent *pMaterial=static_cast<CMaterialComponent*>((*iter)->getComponent("MaterialComponent"));

		//if we have a valid geometry
		if (pGeometry)
		{
			//bind the buffer
			pGeometry->bindBuffers();
		}
		//do we have a matrial
		if (pMaterial)
		{
			//set the matrices
			pMaterial->setProjectionMatrix((float*)CGameObjectManager::getInstance().getMainCamera()->getProjection());
			pMaterial->setViewMatrix((float*)CGameObjectManager::getInstance().getMainCamera()->getView());
			pMaterial->setWorldMatrix((float*)pTransform->getWorld());
			pMaterial->setTextures();
			//bind the vertex layout
			pMaterial->bindVertexLayout();
			//loop for the passes in the material
			for (UINT i=0;i<pMaterial->getNumberOfPasses();i++)
			{
				//Apply the current pass
				pMaterial->applyPass(i);
				//we have a geometry
				if (pGeometry)
				{
					//draw from the geometry
					m_pD3D10Device->DrawIndexed(pGeometry->getNumberOfIndices(),0,0);
				}
			}
		}

	}
	RECT FPS = {10, 10, 0, 0};
	mFont->DrawText(0, mFrameStats.c_str(), -1, &FPS, DT_NOCLIP, GREEN);
	//Swaps the buffers in the chain, the back buffer to the front(screen)
	//http://msdn.microsoft.com/en-us/library/bb174576%28v=vs.85%29.aspx - BMD
    m_pSwapChain->Present( 0, 0 );
}

void CGameApplication::update()
{
	switch( m_camState)
	{
	case DEBUG:
		{
			DebugCam();
			break;
		}
	case PLAYER:
		{
			PlayerCam();
			break;
		}
	}

	m_Timer.update();
	CPhysics::getInstance().update(m_Timer.getElapsedTime());
	//Audio - Update the audio system, this must be called to update streams and listener position
	CAudioSystem::getInstance().update();

	CCameraComponent *pCamera=CGameObjectManager::getInstance().getMainCamera();
	if (pCamera)
	{
		//Fine on track pads but not on mices
		float mouseDeltaX=CInput::getInstance().getMouse()->getRelativeMouseX();
		float mouseDeltaY=CInput::getInstance().getMouse()->getRelativeMouseY();

		pCamera->yaw(mouseDeltaX*m_Timer.getElapsedTime());
		pCamera->pitch(mouseDeltaY*m_Timer.getElapsedTime());
	}
	
	if (CInput::getInstance().getKeyboard()->isKeyDown((int)'W'))
	{
		//play sound
		CTransformComponent * pTransform=CGameObjectManager::getInstance().findGameObject("Player")->getTransform();
		pTransform->translate(0.0f,0.0f,0.5f);

	}
	else if (CInput::getInstance().getKeyboard()->isKeyDown((int)'A'))
	{
		//play sound
		CTransformComponent * pTransform=CGameObjectManager::getInstance().findGameObject("Player")->getTransform();
		pTransform->translate(-0.5f,0.0f,0.0f);
		
	}
		else if (CInput::getInstance().getKeyboard()->isKeyDown((int)'S'))
	{
		//play sound
		CTransformComponent * pTransform=CGameObjectManager::getInstance().findGameObject("Player")->getTransform();
		pTransform->translate(0.0f,0.0f,-0.5f);
		
	}
		else if (CInput::getInstance().getKeyboard()->isKeyDown((int)'D'))
	{
		//play sound
		CTransformComponent * pTransform=CGameObjectManager::getInstance().findGameObject("Player")->getTransform();
		pTransform->translate(0.5f,0.0f,0.0f);
		
	}
	else if (CInput::getInstance().getKeyboard()->isKeyDown((int)'Q'))
	{
		//play sound
		CTransformComponent * pTransform=CGameObjectManager::getInstance().findGameObject("Player")->getTransform();
		pTransform->translate(0.0f,0.5f,0.0f);
		
	}
		else if (CInput::getInstance().getKeyboard()->isKeyDown((int)'E'))
	{
		//play sound
		CTransformComponent * pTransform=CGameObjectManager::getInstance().findGameObject("Player")->getTransform();
		pTransform->translate(0.0f,-0.5f,0.0f);
	}
	else if (CInput::getInstance().getKeyboard()->isKeyDown((int)'P'))
	{
		m_camState = PLAYER;
	}
	else if (CInput::getInstance().getKeyboard()->isKeyDown((int)'O'))
	{
		m_camState = DEBUG;
	}

	//Audio -  If the left mouse button has been pressed
	if (CInput::getInstance().getMouse()->getMouseDown(0))
	{
		//Audio - grab the audio component
		CAudioSourceComponent * pAudio=(CAudioSourceComponent *)CGameObjectManager::getInstance().findGameObject("Test")->getComponent("AudioSourceComponent");
		//Audio - call play
		pAudio->play();
	}
	
	float playerPosX = CGameObjectManager::getInstance().findGameObject("Player")->getTransform()->getPosition().x;
	float playerPosY = CGameObjectManager::getInstance().findGameObject("Player")->getTransform()->getPosition().y;
	float playerPosZ = CGameObjectManager::getInstance().findGameObject("Player")->getTransform()->getPosition().z;

	CTransformComponent * pEnemTransform=CGameObjectManager::getInstance().findGameObject("TestCube")->getTransform();
	//pEnemTransform->setPosition(playerPosX,playerPosY,playerPosZ);

	static int frameCnt = 0;
	static float t_base = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (mTimer.getGameTime() - t_base) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;   
		outs.precision(6);
		outs << L"fps: " << fps << L"\n" 
			 << "Milliseconds: Per Frame: " << mspf;
		mFrameStats = outs.str();
		
		// Reset for next average.
		frameCnt = 0;
		t_base  += 1.0f;
	}
	
	CGameObjectManager::getInstance().update(m_Timer.getElapsedTime());
}

bool CGameApplication::initPhysics()
{
	CPhysics::getInstance().init();
	//Add the Game Application
	CPhysics::getInstance().getPhysicsWorld()->addContactListener(this);
	return true;
}

void CGameApplication::drawScene()
{
	m_pD3D10Device->ClearRenderTargetView(m_pRenderTargetView,mClearColor);
	m_pD3D10Device->ClearDepthStencilView(m_pDepthStencelView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
}
bool CGameApplication::initInput()
{
	CInput::getInstance().init();
	return true;
}
bool CGameApplication::initAudio()
{
	CAudioSystem::getInstance().init();
	return true;
	//return;

}

//initGraphics - initialise the graphics subsystem - BMD
bool CGameApplication::initGraphics()
{
	//Retrieve the size of the window, this is need to match the
	//back buffer to screen size - BMD
	RECT windowRect;
	//http://msdn.microsoft.com/en-us/library/ms633503%28v=vs.85%29.aspx -BMD
	GetClientRect(m_pWindow->getHandleToWindow(),&windowRect);

	//Calculate the width and height of the window - BMD
	UINT width=windowRect.right-windowRect.left;
	UINT height=windowRect.bottom-windowRect.top;

	//Device creation flags, used to control our the D3D10 device is created
	UINT createDeviceFlags=0;
	//If we are in a debug build then set the device creation flag to debug device
#ifdef _DEBUG
	createDeviceFlags|=D3D10_CREATE_DEVICE_DEBUG;
#endif

	//Swap Chain description - used in the creation of the swap chain
	//http://msdn.microsoft.com/en-us/library/bb173075%28v=vs.85%29.aspx - BMD

	//Initialise the swap chain description by setting all its values to zero - BMD
	DXGI_SWAP_CHAIN_DESC sd;
	//http://msdn.microsoft.com/en-us/library/aa366920%28v=vs.85%29.aspx - BMD
    ZeroMemory( &sd, sizeof( sd ) );
	//What kind of surface is contained in the swap chain, in this case something we draw too
	//http://msdn.microsoft.com/en-us/library/bb173078%28v=vs.85%29.aspx - BMD
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//Number of buffers, if we are not full screen this will be one as the desktop
	//acts as a front buffer. If we are in full screen this will be one - BMD
	if (m_pWindow->isFullScreen())
		sd.BufferCount = 2;
	else 
		sd.BufferCount=1;
	//The handle of the window which this swap chain is linked to, this must not be NULL - BMD
	sd.OutputWindow = m_pWindow->getHandleToWindow();
	//Are we in windowed mode, arggh opposite of full screen
	sd.Windowed = (BOOL)(!m_pWindow->isFullScreen());
	//Multisampling(antialsing) parameters for the swap chain - this has performance considerations - see remarks in docs
	//http://msdn.microsoft.com/en-us/library/bb173072%28v=vs.85%29.aspx - BMD
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
	//The description of the swap chain buffer
	//http://msdn.microsoft.com/en-us/library/bb173064%28v=vs.85%29.aspx - BMD
	//width & height of the buffer - this matches the size of the window - BMD
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
	//The data format of the buffer in the swap chain, 8bits used for Red, green, blue & alpha - unsigned int(UNIFORM) - BMD
	//http://msdn.microsoft.com/en-us/library/bb173059%28v=vs.85%29.aspx
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Refresh rate of the buffer in the swap chain - BMD
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
	
	//NB. You should get use to seeing patterns like this when programming with D3D10 
	//where we use a description object which is then used in the creation of a D3D10 resource 
	//like swap chains. Also in a real application we would check to see if some of the above
	//options are support by the graphics hardware. -BMD

	//Create D3D10 Device and swap chain 
	//http://msdn.microsoft.com/en-us/library/bb205087%28v=vs.85%29.aspx - BMD
	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, //Pointer to IDXGIAdpater, this is a display adapater on the machine this can be NULL - BMD
		D3D10_DRIVER_TYPE_HARDWARE,//Type of Driver we have, it can be a hardware device, refrence(slow) or Software(not supported yet) - BMD
		NULL, //Handle to a module that implements a software rasterizer - BMD
		createDeviceFlags,//The device creation flags we used earlier on - BMD
		D3D10_SDK_VERSION,//The version of the SDK we are using this should D3D10 - BMD
		&sd,//The memory address of the swap chain description - BMD
		&m_pSwapChain, //The memory address of the swap chain pointer, if all goes well this will be intialised after this function call - BMD
		&m_pD3D10Device)))//the memory address of the D3D10 Device, if all goes well this will be initialised after this function call - BMD
		return false;

	//NB. There are two ways of creating the device, the above way which initialises the device and swap chain at the sametime
	// or we can create a swap chain and a device seperatly and then associate a swap chain with a device. - BMD

	//Create a render target, this is a Texture which will hold our backbuffer, this will
	//enable us to link the rendertarget with buffer held in the swap chain - BMD
	ID3D10Texture2D *pBackBuffer;
	//Get a buffer from the swap chain 
	//http://msdn.microsoft.com/en-us/library/bb174570%28v=vs.85%29.aspx - BMD
	if (FAILED(m_pSwapChain->GetBuffer(0, //buffer index, 0 will get the back buffer
		__uuidof(ID3D10Texture2D),//The unique identifier of the type of pointer we want in
								  //this case a I3D10 Texture2D
		(void**)&pBackBuffer)))//A pointer to a memory address, this is cast to a void ** because this function
							   //can return back different types dependent on the 2nd param
		return false;

	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width=width;
	descDepth.Height=height;
	descDepth.MipLevels=1;
	descDepth.ArraySize=1;
	descDepth.Format=DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count=1;
	descDepth.SampleDesc.Quality=0;
	descDepth.Usage=D3D10_USAGE_DEFAULT;
	descDepth.BindFlags=D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags=0;
	descDepth.MiscFlags=0;

	if (FAILED(m_pD3D10Device->CreateTexture2D(&descDepth,NULL,&m_pDepthStencilTexture)))
		return false;

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format=descDepth.Format;
	descDSV.ViewDimension=D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice=0;

	if (FAILED(m_pD3D10Device->CreateDepthStencilView(m_pDepthStencilTexture,&descDSV,&m_pDepthStencelView)))
		return false;


	//Create the Render Target View, a view is the way we access D3D10 resources
	//http://msdn.microsoft.com/en-us/library/bb173556%28v=vs.85%29.aspx - BMD
	if (FAILED(m_pD3D10Device->CreateRenderTargetView( pBackBuffer, //The resource we are creating the view for - BMD
		NULL, //The description of the view, in this case NULL - BMD
		&m_pRenderTargetView ))) // the memory address of a pointer to D3D10 Render Target - BMD
	{
		
		pBackBuffer->Release();
		return  false;
	}
	//The above Get Buffer call will allocate some memory, we now need to release it. - BMD
    pBackBuffer->Release();

	//Binds one or more render targets and depth buffer to the Output merger stage - BMD
	//http://msdn.microsoft.com/en-us/library/bb173597%28v=vs.85%29.aspx - BMD
	m_pD3D10Device->OMSetRenderTargets(1, //Number  of views - BMD
		&m_pRenderTargetView, //pointer to an array of D3D10 Render Target Views - BMD
		m_pDepthStencelView); //point to Depth Stencil buffer - BMD

    // Setup the viewport 
	//http://msdn.microsoft.com/en-us/library/bb172500%28v=vs.85%29.aspx - BMD
    D3D10_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
	//Sets the Viewport 
	//http://msdn.microsoft.com/en-us/library/bb173613%28v=vs.85%29.aspx - BMD
    m_pD3D10Device->RSSetViewports( 1 //Number of viewports to bind
		, &vp );//an array of viewports

	return true;
}

bool CGameApplication::initWindow()
{
	m_pWindow=new CWin32Window();
	if (!m_pWindow->init(TEXT("Lab 1 - AwesomeGame"),800,640,false))
		return false;
	return true;
}