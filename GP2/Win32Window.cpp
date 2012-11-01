#include "Win32Window.h"
#include "Input.h"
#include "Keyboard.h"
#include "Mouse.h"

CWin32Window * g_pWindow=NULL;

LRESULT CALLBACK StaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//call the wndProc function held inside the instance of the CWin32Window class - BMD
	return g_pWindow->wndProc(hWnd,uMsg,wParam,lParam);
}


CWin32Window::CWin32Window(void)
{
	g_pWindow=this;
}

CWin32Window::~CWin32Window(void)
{

}

bool CWin32Window::init(const wstring &title,int width,int height,bool fullscreen)
{
	m_bIsFullScreen=fullscreen;
	//Retrieve the instance of the applicaiton, this is needed to
	//link up the application to the window 
	//http://msdn.microsoft.com/en-us/library/ms683199%28v=vs.85%29.aspx -BMD
	HINSTANCE currentInstance=GetModuleHandle(NULL);
	// Register the windows class, this is used for information about the window
	// such as title, icon, mouse cursor, style of window etc
	// http://msdn.microsoft.com/en-us/library/ms633577%28v=vs.85%29.aspx - BMD
	WNDCLASSEX wndClass;
	//the size of the wndclass structure - BMD
	wndClass.cbSize=sizeof(WNDCLASSEX);
	//the style of the window - BMD
	wndClass.style = CS_VREDRAW|CS_HREDRAW;
	//the function to handle windows messages - BMD
	wndClass.lpfnWndProc = StaticWndProc;
	//extra class info - BMD
	wndClass.cbClsExtra = 0;
	//extra window info - BMD
	wndClass.cbWndExtra = 0;
	//application instance - BMD
	wndClass.hInstance = currentInstance;
	//icon for the window
	//http://msdn.microsoft.com/en-us/library/ms648072%28v=vs.85%29.aspx - BMD
	wndClass.hIcon = LoadIcon(currentInstance,IDI_APPLICATION);
	//cursor of the window
	//http://msdn.microsoft.com/en-us/library/ms648391%28v=vs.85%29.aspx - BMD
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	//background colour
	//http://msdn.microsoft.com/en-us/library/dd144925%28v=vs.85%29.aspx - BMD
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//menu name - BMD
	wndClass.lpszMenuName = NULL;
	//window class name - BMD
	wndClass.lpszClassName =title.c_str();
	//the small icon
	//http://msdn.microsoft.com/en-us/library/ms648072%28v=vs.85%29.aspx - BMD
	wndClass.hIconSm=LoadIcon(NULL,IDI_APPLICATION);

	//register the class, all subsequent calls to CreateWindowEX will use the above class
	//http://msdn.microsoft.com/en-us/library/ms633587%28v=vs.85%29.aspx - BMD
	if (!RegisterClassEx(&wndClass))
		return false;

	//style of window, this will be changed if we are going full screen 
	//N.B. for a full list styles go to the following link 
	//http://msdn.microsoft.com/en-us/library/ms632600%28v=vs.85%29.aspx -BMD
	DWORD style=WS_OVERLAPPEDWINDOW;
	//if we are full screen change the style
	if (fullscreen)
	{
		//change style - BMD
		style=WS_POPUP;
		//hide mouse cursor
		//http://msdn.microsoft.com/en-us/library/ms648396%28v=vs.85%29.aspx -BMD
		ShowCursor(FALSE);
	}
	
	//create the window, returns a valid HWND(handle to window)
	// http://msdn.microsoft.com/en-us/library/ms632680%28v=vs.85%29.aspx - BMD
	m_hWND= CreateWindowEx( NULL, //extra styles
							wndClass.lpszClassName, //The window class name
							title.c_str(),//The title of the window	
							style,//style of the window
                            0,//x position 
							0, //y position
							width, //width 
							height, //height
							GetDesktopWindow(), // a handle to the parent window
							NULL, //handle to a menu
							currentInstance, //application instance
							0); //window creation data -BMD

	//if the handle to the window is null, then something went wrong -BMD
	if (!m_hWND)
		return false;

	//Show Window
	//http://msdn.microsoft.com/en-us/library/ms633548%28v=vs.85%29.aspx - BMD
	ShowWindow(m_hWND, SW_SHOWDEFAULT);
	//Update window, triggers a pain
	//http://msdn.microsoft.com/en-us/library/dd145167%28v=vs.85%29.aspx - BMD
	UpdateWindow(m_hWND);

	// if all goes well, set running to true -BMD
	m_bIsRunning=true;

    m_Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    m_Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    m_Rid[0].dwFlags = RIDEV_INPUTSINK;   
    m_Rid[0].hwndTarget = m_hWND;
    RegisterRawInputDevices(m_Rid, 1, sizeof(m_Rid[0]));

	return true;
}

bool CWin32Window::checkForWindowMessages()
{
	//The windows message -BMD
	MSG  msg;
	//Check to see if there is a message in the windows
	//message queue
	// http://msdn.microsoft.com/en-us/library/ms644943%28v=vs.85%29.aspx - BMD
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)!=0)
	{
		//translate message - http://msdn.microsoft.com/en-us/library/ms644955%28v=vs.85%29.aspx -BMD
		TranslateMessage( &msg );
		//dispatch to the registed window message pump function - http://msdn.microsoft.com/en-us/library/ms644934%28v=vs.85%29.aspx -BMD
        DispatchMessage( &msg );
        return true;
	}	
	else
		return false;
}


//wndProc
//The windows message pump
//hWnd - The window that is recieving messages(HWND)
//uMsg - The message type(unisgned int)
//NB. Messages - http://msdn.microsoft.com/en-us/library/ms633573%28v=vs.85%29.aspx
//wParam - The wide parameter of the message(WPARAM)
//lParam - The low parameter of the message(LPARAM)
//returns the result of the message(LRESULT) -BMD
LRESULT CALLBACK CWin32Window::wndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//switch on message type - BMD
	switch (uMsg)
	{
		//Destory message
		//The window has been closed - BMD
		case WM_DESTROY:
			{
				//set running to false - BMD
				m_bIsRunning=false;
				//post a quit message to the windows message pump - BMD
				PostQuitMessage(0);	
				break;
			}
		case WM_KEYDOWN:
			{
				CInput::getInstance().getKeyboard()->setKeyDown((int)wParam);
				break;
			}
		case WM_KEYUP:
			{
				CInput::getInstance().getKeyboard()->setKeyUp((int)wParam);
				break;
			}
			case WM_INPUT: 
			{
				UINT dwSize = 40;
				static BYTE lpb[40];
    
				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, 
								lpb, &dwSize, sizeof(RAWINPUTHEADER));
    
				RAWINPUT* raw = (RAWINPUT*)lpb;
    
				if (raw->header.dwType == RIM_TYPEMOUSE) 
				{
					int xPosRelative = raw->data.mouse.lLastX;
					int yPosRelative = raw->data.mouse.lLastY;
					int mouseOneButtonDown=raw->data.mouse.ulButtons&RI_MOUSE_BUTTON_1_DOWN;
					int mouseTwoButtonDown=raw->data.mouse.ulButtons&RI_MOUSE_BUTTON_2_DOWN;
					int mouseThreeButtonDown=raw->data.mouse.ulButtons&RI_MOUSE_BUTTON_3_DOWN;

					CInput::getInstance().getMouse()->setMouseButtonsDown((bool)mouseOneButtonDown,
						(bool)mouseTwoButtonDown,(bool)mouseThreeButtonDown);

					CInput::getInstance().getMouse()->setMouseMove((float)xPosRelative,(float)yPosRelative);
				} 
				break;
			}

			//if no message has been handled by us, just let windows handle it
			// http://msdn.microsoft.com/en-us/library/ms633572%28v=vs.85%29.aspx - BMD
			default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
