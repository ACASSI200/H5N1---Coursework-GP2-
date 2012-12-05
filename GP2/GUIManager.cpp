#include "GUIManager.h"
#include "Input.h"
#include "Mouse.h"

CGUIManager::CGUIManager()
{
}

CGUIManager::~CGUIManager()
{

}

void CGUIManager::destroy()	
{
	m_pContext->RemoveReference();
	Rocket::Core::Shutdown();

	if (m_pD3D10Renderer)
	{
		delete m_pD3D10Renderer;
		m_pD3D10Renderer=NULL;
	}
	if (m_pSystemInterface)
	{
		delete m_pSystemInterface;
		m_pSystemInterface=NULL;
	}
}

void CGUIManager::init(ID3D10Device * pD3D10Device,int width,int height)
{
	// Install our DirectX render interface into Rocket.
	m_pD3D10Renderer=new RenderInterfaceDirectX10(pD3D10Device,width,height);
	Rocket::Core::SetRenderInterface(m_pD3D10Renderer);

	m_pSystemInterface=new CGUISystemInterface();
	Rocket::Core::SetSystemInterface(m_pSystemInterface);

	Rocket::Core::Initialise();

	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Roman.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Bold.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Italic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-BoldItalic.otf");
	m_pContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(width, height));
	if (m_pContext == NULL)
	{
		Rocket::Core::Shutdown();
		return;
	}
	Rocket::Controls::Initialise();
	Rocket::Debugger::Initialise(m_pContext);
	Rocket::Debugger::SetVisible(false);

	// Load and show the tutorial document.
	Rocket::Core::ElementDocument* document = m_pContext->LoadDocument("window.rml");
	if (document != NULL)
	{
		document->Show();
		document->RemoveReference();
	}
}

void CGUIManager::update()
{
	m_pContext->Update();
	updateMouse();
	updateKeyboard();
}

void CGUIManager::updateMouse()
{
	if (CInput::getInstance().getMouse()->getMouseDown(0))
	{
		m_pContext->ProcessMouseButtonDown(0,0);
	}
	else if (CInput::getInstance().getMouse()->getMouseDown(1))
	{
		m_pContext->ProcessMouseButtonDown(1,0);
	}
	else
	{
		m_pContext->ProcessMouseButtonDown(2,0);
	}

	if (CInput::getInstance().getMouse()->getMouseUp(0))
	{
		m_pContext->ProcessMouseButtonUp(0,0);
	}
	else if (CInput::getInstance().getMouse()->getMouseUp(1))
	{
		m_pContext->ProcessMouseButtonUp(1,0);
	}
	else
	{
		m_pContext->ProcessMouseButtonUp(2,0);
	}

	m_pContext->ProcessMouseMove(CInput::getInstance().getMouse()->getAbsoluteMouseX(),
		CInput::getInstance().getMouse()->getAbsoluteMouseY(),0);
}

void CGUIManager::updateKeyboard()
{
}

void CGUIManager::render()
{
	m_pContext->Render();
}