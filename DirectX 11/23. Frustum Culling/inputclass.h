#pragma once
#define DIRECTINPUT_VERSION 0x0800

// Linking
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <map>
#include <string>

static std::map<int, std::string> keyMap{
{DIK_ESCAPE, "[ESC]"}, {DIK_1, "1"}, {DIK_2, "2"}, {DIK_3, "3"}, {DIK_4, "4"}, {DIK_5, "5"},
{DIK_6, "6"}, {DIK_7, "7"}, {DIK_8, "8"}, {DIK_9, "9"}, {DIK_0, "0"}, {DIK_MINUS, "-"}, {DIK_EQUALS, "="},
{DIK_BACK, "[BACK]"}, {DIK_TAB, "[TAB]"}, {DIK_Q, "Q"}, {DIK_W, "W"}, {DIK_E, "E"}, {DIK_R, "R"}, {DIK_T, "T"},
{DIK_Y, "Y"}, {DIK_U, "U"}, {DIK_I, "I"}, {DIK_O, "O"}, {DIK_P, "P"}, {DIK_LBRACKET, "["}, {DIK_RBRACKET, "]"},
{DIK_RETURN, "[ENTER]"}, {DIK_LCONTROL, "[LCONTROL]"}, {DIK_A, "A"}, {DIK_S, "S"}, {DIK_D, "D"}, {DIK_F, "F"}, {DIK_G, "G"},
{DIK_H, "H"}, {DIK_J, "J"}, {DIK_K, "K"}, {DIK_L, "L"}, {DIK_SEMICOLON, ";"}, {DIK_APOSTROPHE, "\'"}, {DIK_GRAVE, "`"},
{DIK_LSHIFT, "[LSHIFT]"}, {DIK_BACKSLASH, "\\"}, {DIK_Z, "Z"}, {DIK_X, "X"}, {DIK_C, "C"}, {DIK_V, "V"}, {DIK_B, "B"}, 
{DIK_N, "N"}, {DIK_M, "M"}, {DIK_COMMA, ","}, {DIK_PERIOD, "."}, {DIK_SLASH, "/"}, {DIK_RSHIFT, "[RSHIFT]"}, {DIK_MULTIPLY, "*"},
{DIK_SPACE, "[SPACE]"}, {DIK_CAPITAL, "[CAPS]"}, {DIK_NUMLOCK, "[LOCK]"}, {DIK_F1, "[F1]"}, {DIK_F2, "[F2]"}, {DIK_F3, "[F3]"}, {DIK_F4, "[F4]"}, {DIK_F5, "[F5]"},
{DIK_F6, "[F6]"}, {DIK_F7, "[F7]"}, {DIK_F8, "[F8]"}, {DIK_F9, "[F9]"}, {DIK_F10, "[F10]"}, {DIK_NUMLOCK, "[NUMLOCK]"}, {DIK_SCROLL, "[SCROLL]"},
{DIK_NUMPAD7, "7"}, {DIK_NUMPAD8, "8"}, {DIK_NUMPAD9, "9"}, {DIK_SUBTRACT, "-"}, {DIK_NUMPAD4, "4"}, {DIK_NUMPAD5, "5"}, {DIK_NUMPAD6, "6"},
{DIK_ADD, "+"}, {DIK_NUMPAD1, "1"}, {DIK_NUMPAD2, "2"}, {DIK_NUMPAD3, "3"}, {DIK_NUMPAD0, "0"}, {DIK_DECIMAL, "."}, {DIK_F11, "[F11]"},
{DIK_F12, "[F12]"}, {DIK_F13, "[F13]"}, {DIK_F14, "[F14]"}, {DIK_F15, "[F15]"}, {DIK_NUMPADEQUALS, "="}, {DIK_NUMPADENTER, "[ENTER]"}, {DIK_RCONTROL, "[RCTRL]"},
{DIK_VOLUMEDOWN, "[VOLDOWN]"}, {DIK_VOLUMEUP, "[VOLUP]"}, {DIK_NUMPADCOMMA, ","}, {DIK_DIVIDE, "/"}, {DIK_PAUSE, "[PAUSE]"}, {DIK_HOME, "[HOME]"}, {DIK_UP, "[UP]"},
{DIK_LEFT, "[LEFT]"}, {DIK_RIGHT, "[RIGHT]"}, {DIK_DOWN, "[DOWN]"}, {DIK_DELETE, "[DEL]"},
};

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int&, int&);
	bool IsMousePressed();

	bool IsEscapePressed();
	bool IsTabPressed();
	bool IsKeyPressed(char);

	void GetAllPressedKeys(std::string&);
	
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	// Interfaces to Direct Input and keyboard & mouse devices
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	// Used to record current state of keyboard and mouse
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight, m_mouseX, m_mouseY;
};
