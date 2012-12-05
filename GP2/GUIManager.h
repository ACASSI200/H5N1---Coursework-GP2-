#pragma once

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include "RenderInterfaceDirectX10.h"
#include "GUISystemInterface.h"

#include <string>
#include <vector>

using namespace std;

class CGUIManager
{
public:
	~CGUIManager();

	static CGUIManager& getInstance()
	{
		static CGUIManager instance;
		return instance;
	};

	Rocket::Core::ElementDocument* loadGUI(const string& name);



	void init(ID3D10Device * pD3D10Device,int width,int height);
	void destroy();

	void update();
	void render();
private:

	void updateMouse();
	void updateKeyboard();

	CGUIManager();


	CGUIManager(CGUIManager const&){};
	void operator=(CGUIManager const&){}; 
private:
	Rocket::Core::Context* m_pContext;
	RenderInterfaceDirectX10* m_pD3D10Renderer;
	CGUISystemInterface * m_pSystemInterface;

	vector<Rocket::Core::ElementDocument*> m_GUIScreens;
};