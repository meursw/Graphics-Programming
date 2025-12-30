#include "systemclass.h"
#include <iostream>
#include <fstream>
SystemClass::SystemClass() {
	m_Input = 0;
	m_Application = 0;
}

SystemClass::SystemClass(const SystemClass&) {}

SystemClass::~SystemClass() {}

bool SystemClass::Initialize() {
	int screenWidth{ 0 }, screenHeight{ 0 };

	InitalizeWindows(screenWidth, screenHeight);

	m_Input = new InputClass;
	m_Input->Initialize();

	m_Application = new ApplicationClass;

	// pass the window handle to the application (variable of system class)
	return m_Application->Initialize(screenWidth, screenHeight, m_hwnd, m_Input);

}

void SystemClass::Shutdown() {
	if (m_Application) {
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	if (m_Input) {
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();

	return;
}

//The Run function is where our application will loop and do all the application processing until we decide to quit.
//The application processing is done in the Frame function which is called each loop.
//This is an important concept to understand as now the rest of our application must be written with this in mind.
//The pseudo code looks like the following :
//
//while not done
//check for windows system messages
//process system messages
//process application loop
//check if user wanted to quit during the frame processing

void SystemClass::Run() {
	MSG msg; // Windows system message
	bool done, frameResult;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done) {
		// Handle windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			frameResult = Frame();
			if (!frameResult) done = true;
		}
	}

	return;
}

bool SystemClass::Frame() {
	if (m_Input->isKeyDown(VK_ESCAPE)) {
		return false;
	}
	bool result;

	// Get center of client rect
	RECT windowRect;
	GetClientRect(m_hwnd, &windowRect);
	int centerX = (windowRect.left + windowRect.right) / 2;
	int centerY = (windowRect.bottom + windowRect.top) / 2;

	// Get cursor/mouse position and convert its coordinates to screen client coordinates
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_hwnd, &cursorPos);

	float deltaX = static_cast<float>(cursorPos.x - centerX);
	float deltaY = static_cast<float>(cursorPos.y - centerY);

	if (fabs(deltaX) > 0.01f || fabs(deltaY) > 0.01f) {
		m_Application->m_Camera->UpdateRotation(XMFLOAT2(deltaX, deltaY));

		POINT screenCenter;
		screenCenter.x = centerX;
		screenCenter.y = centerY;
		ClientToScreen(m_hwnd, &screenCenter);
		SetCursorPos(screenCenter.x, screenCenter.y);
	}

	result = m_Application->Frame();

	return result;
}

// The MessageHandler function is where we direct the windows system messages into.

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {

	case WM_KEYDOWN: {
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	case WM_KEYUP: {
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	default: {
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	}
}

void SystemClass::InitalizeWindows(int& screenWidth, int& screenHeight) {
	// struct that contains information for window creation
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;

	int posX, posY;

	ApplicationHandle = this;

	// Get the instance of this application/executable.
	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"Engine";

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;

	// Register window class
	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN) {
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;

	}
	else {
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create window with the screen settings and get handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	// Put cursor in the center
	RECT windowRect;
	GetClientRect(m_hwnd, &windowRect);
	int centerX = (windowRect.left + windowRect.right) / 2;
	int centerY = (windowRect.bottom + windowRect.top) / 2;

	POINT screenCenter{ centerX, centerY };
	ClientToScreen(m_hwnd, &screenCenter);
	SetCursorPos(screenCenter.x, screenCenter.y);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}