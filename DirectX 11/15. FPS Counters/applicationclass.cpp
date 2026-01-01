#include "applicationclass.h"


ApplicationClass::ApplicationClass() { 
	m_Direct3D = 0; 
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Lights = 0;
	m_Timer = 0;
	m_Bitmap = 0;
	m_Timer = 0;
	m_FontShader = 0;
	m_Font = 0;
	m_Fps = 0;
	m_FpsString = 0;
	
}
ApplicationClass::ApplicationClass(const ApplicationClass& o) {}
ApplicationClass::~ApplicationClass() {}

void MoveCamera(InputClass* m_Input, CameraClass* m_Camera, float frameTime) {
	if (m_Input->isKeyDown('W'))
		m_Camera->Render(FORWARD, frameTime);
	if (m_Input->isKeyDown('A'))
		m_Camera->Render(LEFT, frameTime);
	if (m_Input->isKeyDown('S'))
		m_Camera->Render(BACKWARD, frameTime);
	if (m_Input->isKeyDown('D'))
		m_Camera->Render(RIGHT, frameTime);
	if (m_Input->isKeyDown('E'))
		m_Camera->Render(UP, frameTime);
	if (m_Input->isKeyDown('Q'))
		m_Camera->Render(DOWN, frameTime);
}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, InputClass* input) {
	m_Input = input;

	char modelFilename[128];
	char textureFilename[128];
	char bitmapFilename[128];
	char spriteFilename[128];
	char fpsString[128];

	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"ERROR", MB_OK);
		return false;
	}

	// Create camera object
	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->SetRotation(0.0, 0.0, 0.0);
	m_Camera->SetCameraSpeed(50.0f);

	// Create model class
	m_Model = new ModelClass;

	// Set the file name of the texture.
	strcpy_s(textureFilename, "../DX11/Assets/Textures/drybones.png");
	
	// Set the file name of the model.
	strcpy_s(modelFilename, "../DX11/Assets/Models/drybones.txt");
	
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Set the file name of the bitmap file.
	strcpy_s(bitmapFilename, "../DX11/Assets/UI/harry_portrait.png");

	// Create and initialize the bitmap object.
	m_Bitmap = new BitmapClass;

	if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, bitmapFilename, 10,300, 6.0f, 6.0f))
		return false;

	strcpy_s(spriteFilename, "../DX11/Assets/Sprites/Tenna Dance/tenna_dance.txt");
	m_Sprite = new SpriteClass;
	if (!m_Sprite->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, spriteFilename, 50, 50))
		return false;
	
	// Create and initialize the texture shader object.
	m_TextureShader = new TextureShaderClass;

	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light shader object
	m_LightShader = new LightShaderClass;
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the font object.
	m_Font = new FontClass;
	if (!m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0))
		return false;

	// Set the strings we want to display.
	// Create and initialize the fps object.
	m_Fps = new FpsClass();

	m_Fps->Initialize();

	// Set the initial fps and fps string.
	m_previousFps = -1;
	strcpy_s(fpsString, "Fps: 0");

	// Create and initialize the text object for the fps string.
	m_FpsString = new TextClass;

	if (!m_FpsString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, fpsString, 10, 10, 0.0f, 1.0f, 0.0f))
		return false;
	
	m_LightShader = new LightShaderClass;
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_Lights = new LightClass;
	m_Lights->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Lights->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights->SetDirection(1.0f, 0.0f, 1.0f);
	m_Lights->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights->SetSpecularPower(256.0f);

	m_Timer = new TimerClass;
	if (!m_Timer->Initialize()) return false;

	return true;
}

void ApplicationClass::Shutdown() {
	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the light objects.
	if (m_Lights)
	{
		delete m_Lights;
		m_Lights = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the sprite object.
	if (m_Sprite)
	{
		m_Sprite->Shutdown();
		delete m_Sprite;
		m_Sprite = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the text object for the fps string.
	if (m_FpsString)
	{
		m_FpsString->Shutdown();
		delete m_FpsString;
		m_FpsString = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	return;
}


bool ApplicationClass::Frame() {
	if (!UpdateFps())
		return false;

	m_Timer->Frame();
	float frameTime = m_Timer->GetTime();

	MoveCamera(m_Input, m_Camera, frameTime);
	m_Sprite->Update(frameTime);

	return Render();
}



bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	XMMATRIX scaleMatrix, rotateMatrix, translateMatrix, srMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	m_Direct3D->TurnZBufferOn();
	int resolution = 100;
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (int i = 0, x = 0, z = 0; i < resolution* resolution; x++, i++) {
		if (x == resolution) {
			x = 0;
			z++;
		}
		scaleMatrix = XMMatrixScaling(8.0 / (z*0.2 + 1), 8.0 / (z * 0.2 + 1), 8.0 / (z * 0.2 + 1));
		rotateMatrix = XMMatrixRotationY(x);
		srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
		translateMatrix = XMMatrixTranslation(x * 5.5, 0, z*5.5);
		worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);
		m_Model->Render(m_Direct3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
			m_Lights->GetDirection(), m_Lights->GetAmbientColor(), m_Lights->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Lights->GetSpecularColor(), m_Lights->GetSpecularPower()
		);
		if (!result) return false;
	}

	// Disable the Z buffer and enable alpha blending for 2D rendering.
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->EnableAlphaBlending();

	// Render the FPS text string using the font shader.
	m_FpsString->Render(m_Direct3D->GetDeviceContext());
	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(), XMMatrixIdentity(), XMMatrixIdentity(), orthoMatrix,
		m_Font->GetTexture(), m_FpsString->GetPixelColor());
	if (!result) return false;

	// Enable the Z buffer and disable alpha blending now that 2D rendering is complete.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


bool ApplicationClass::UpdateFps()
{
	int fps;
	char tempString[16], finalString[16];
	float red, green, blue;
	bool result;


	// Update the fps each frame.
	m_Fps->Frame();

	// Get the current fps.
	fps = m_Fps->GetFps();

	// Check if the fps from the previous frame was the same, if so don't need to update the text string.
	if (m_previousFps == fps)
	{
		return true;
	}

	// Store the fps for checking next frame.
	m_previousFps = fps;

	// Truncate the fps to below 100,000.
	if (fps > 99999)
	{
		fps = 99999;
	}

	// Convert the fps integer to string format.
	sprintf_s(tempString, "%d", fps);

	// Setup the fps string.
	strcpy_s(finalString, "Fps: ");
	strcat_s(finalString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = m_FpsString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 10, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}
