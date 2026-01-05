#pragma once

#include "d3dclass.h"
#include "inputclass.h"
#include "timerclass.h"
#include "cameraclass.h"
#include "lightclass.h"

#include "modelclass.h"
#include "bitmapclass.h"
#include "spriteclass.h"
#include "textclass.h"

#include "colorshaderclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "fontshaderclass.h"

#include "fontclass.h"
#include "fpsclass.h"


// GLOBALS
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class ApplicationClass {
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(InputClass*, HWND);

private:
	bool Render();
	bool UpdateFps();
	bool UpdateInput(InputClass*);

	void CenterCursor(HWND);

public:
	D3DClass* m_Direct3D;
	
	ModelClass* m_Model;
	BitmapClass* m_Bitmap;
	SpriteClass* m_Sprite;
	TextClass* m_FpsString;
	TextClass* m_InputString;
	
	FontShaderClass* m_FontShader;
	ColorShaderClass* m_ColorShader;
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	
	LightClass* m_Lights;
	int m_numLights;

	TimerClass* m_Timer;
	CameraClass* m_Camera;
	FontClass* m_Font;

	FpsClass* m_Fps;
	int m_previousFps;

	int m_prevMouseX, m_prevMouseY;

	bool m_mouseLocked = true;
};