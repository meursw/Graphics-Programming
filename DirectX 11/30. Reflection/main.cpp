#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;

	System = new SystemClass;
	
	if (System->Initialize()) {
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}