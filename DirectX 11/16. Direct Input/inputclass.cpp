#include "inputclass.h"
#include <map>
InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

InputClass::InputClass(const InputClass& o) {}

InputClass::~InputClass() {}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT hr;
	
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface. Once you have a Direct Input object you can initialize other input devices.
	hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr)) return false;

	// Initialize the direct input interface for the keyboard.
	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(hr)) return false;

	// Set the data format. In this case since it is a keyboard so we can use the predefined data format.
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return false;

	// Set the cooperative level of the keyboard to not share with other programs
	hr = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr)) return false;

	// Acquire the keyboard
	hr = m_keyboard->Acquire();
	if (FAILED(hr)) return false;

	// Initialize the direct input interface for the mouse.
	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	hr = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}

	// Acquire the mouse.
	hr = m_mouse->Acquire();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}



// The Shutdown function releases the two devices and the interface to Direct Input. Notice that the devices are always un-acquired first and then released.
void InputClass::Shutdown()
{

	if (m_keyboard) {
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_mouse) {
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_directInput) {
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputClass::Frame()
{
	if (!ReadKeyboard())
		return false;
	
	if (!ReadMouse())
		return false;

	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT hr;

	// Read keyboard device
	hr = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			m_keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT hr;

	// Read mouse device
	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			m_keyboard->Acquire();
		else
			return false;
	}

	return true;
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.


	return;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise AND on the keyboard state to check if the escape key is currently being pressed.
	return (m_keyboardState[DIK_ESCAPE] & 0x80);
}

bool InputClass::IsTabPressed()
{
	return(m_keyboardState[DIK_TAB] & 0x80);
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

bool InputClass::IsMousePressed()
{
	// Check the left mouse button state.
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsKeyPressed(char key)
{
	switch (key)
	{
	case 'W':
		return (m_keyboardState[DIK_W] & 0x80);
	case 'A':
		return (m_keyboardState[DIK_A] & 0x80);
	case 'S':
		return (m_keyboardState[DIK_S] & 0x80);
	case 'D':
		return (m_keyboardState[DIK_D] & 0x80);
	case 'E':
		return (m_keyboardState[DIK_E] & 0x80);
	case 'Q':
		return (m_keyboardState[DIK_Q] & 0x80);
	}

	return false;
}

void InputClass::GetAllPressedKeys(std::string& str)
{
	for (auto value : keyMap)
	{
		if (m_keyboardState[value.first] & 0x80)
			str += value.second;
	}
}