//Are we on the windows platform?
#ifdef WIN32
//if so include the header for windows applications
#include <Windows.h>
#endif

#include "GameApplication.h"

//Check to see if we are on windows
#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine,
					int nCmdShow)
#else
//we are on some other platform
int main(int argc, char **argv)
#endif
{
	CGameApplication *pApp=new CGameApplication();
	if (!pApp->init())
	{
		if (pApp)
		{
			delete pApp;
			pApp=NULL;
			return 1;
		}
	}
	pApp->run();
	if (pApp)
	{
		delete pApp;
		pApp=NULL;
	}
	return 0;
}

