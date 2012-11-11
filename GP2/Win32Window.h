#pragma once

#include <string>
#include <Windows.h>

using namespace std;

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

class CWin32Window
{
public:
	CWin32Window(void);
	~CWin32Window(void);

	bool init(const wstring &title,int width,int height,bool fullscreen);
	
	bool running()
	{
		return m_bIsRunning;
	};

	bool isFullScreen()
	{
		return m_bIsFullScreen;
	};

	LRESULT CALLBACK wndProc( HWND hWnd, UINT uMsg, 
		WPARAM wParam, LPARAM lParam );
	
	bool checkForWindowMessages();

	HWND getHandleToWindow()
	{
		return m_hWND;
	};
private:
	bool m_bIsRunning;
	bool m_bIsFullScreen;
	HWND m_hWND;
	RAWINPUTDEVICE m_Rid[1];

};
