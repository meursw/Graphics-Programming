#pragma once

#include "d3dclass.h"
#include "inputclass.h"
#include "cameraclass.h"
#include "timerclass.h"

#include "modelclass.h"
#include "rendertextureclass.h"
#include "textureshaderclass.h"
#include "reflectionshaderclass.h"


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
    bool Frame(InputClass* Input, HWND hwnd);

private:
    bool Render();

    bool RenderReflectionToTexture();
    
    void CenterCursor(HWND);
    bool RenderUI(int = 0);
    
private:
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    TimerClass* m_Timer;

    ModelClass* m_CubeModel;
    ModelClass* m_FloorModel;
    RenderTextureClass* m_RenderTexture;
    TextureShaderClass* m_TextureShader;
    ReflectionShaderClass* m_ReflectionShader;

	int m_prevMouseX, m_prevMouseY;
	bool m_mouseLocked = true;
};