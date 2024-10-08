#pragma once

#define WIN32_LEAN_AND_MEAN // pre processing directive. speeds up build process.

#include <windows.h> // will be included in main.cpp. Provides functions for creating & destroying windows.

#include "inputclass.h"
#include "applicationclass.h"

 // The definition of the class is fairly simple.
 // We see the Initialize, Shutdown, and Run functions that were called in WinMain defined here.

 // There are also some private functions that will be called inside those functions.

 // We have also included a MessageHandler function in the class
 // to handle the Windows system messages that will get sent to the application while it is running.

 // Finally, we have some private variables, m_Input and m_Application,
 // which will be pointers to the two objects that handle input and graphics rendering.

class SystemClass 
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	ApplicationClass* m_Application; // for handling DirectX graphics code.
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;