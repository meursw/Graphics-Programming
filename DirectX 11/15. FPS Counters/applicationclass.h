#pragma once

#include "d3dclass.h"
#include "inputclass.h"
#include "timerclass.h"

#include "modelclass.h"
#include "bitmapclass.h"
#include "spriteclass.h"

#include "cameraclass.h"
#include "lightclass.h"

#include "colorshaderclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"

#include "fontshaderclass.h"
#include "fontclass.h"
#include "textclass.h"
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

	bool Initialize(int, int, HWND, InputClass*);
	void Shutdown();
	bool Frame();

private:
	bool Render();
	bool UpdateFps();

public:
	D3DClass* m_Direct3D;
	InputClass* m_Input;
	
	ModelClass* m_Model;
	CameraClass* m_Camera;
	
	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader;
	
	LightShaderClass* m_LightShader;
	LightClass* m_Lights;
	int m_numLights;

	BitmapClass* m_Bitmap;
	SpriteClass* m_Sprite;

	TimerClass* m_Timer;

	FontShaderClass* m_FontShader;
	FontClass* m_Font;
	TextClass* m_FpsString;

	FpsClass* m_Fps;
	int m_previousFps;

};