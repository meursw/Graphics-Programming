#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "inputclass.h"
#include "applicationclass.h"

class SystemClass {
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
	void InitalizeWindows(int&, int&);
	void ShutdownWindows();

private:
	// An LPCWSTR is a 32-bit pointer to a constant string of 16-bit Unicode characters, which MAY be null-terminated.
	LPCWSTR m_applicationName;
	// A handle to identify your application for others WINAPI calls
	HINSTANCE m_hinstance;
	// Handle to a window
	HWND m_hwnd;

	InputClass* m_Input;
	ApplicationClass* m_Application;


};

// A callback function, that processes messages sent to a window.
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif