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
	m_TextString1 = 0;
	m_TextString2 = 0;
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

	char testString1[32], testString2[32];

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
	m_Camera->SetCameraSpeed(8.0f);


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
	strcpy_s(testString1, "OwO Hello!");
	strcpy_s(testString2, ">_< EPIC ENGINE");
	int centerX = (screenWidth / 2) - m_Font->GetSentencePixelLength("OwO Hello!") / 2;
	int centerY = (screenHeight / 2) - m_Font->GetFontHeight() / 2;

	// Create and initialize the first text object.
	m_TextString1 = new TextClass;
	if (!m_TextString1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, testString1, centerX, centerY, 0.234f, 0.74123f, 0.450f))
		return false;

	// Create and initialize the second text object.
	m_TextString2 = new TextClass;
	if (!m_TextString2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, testString2, 800-150, 600-20, 0.9312f, 0.29934f, 0.6f))
		return false;

	// Set the number of lights we will use.
	m_numLights = 5;

	// Create and initialize the light objects array.
	m_Lights = new LightClass[m_numLights];

	// Manually set the color and position of each light.
	m_Lights[0].SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);  // Red
	m_Lights[0].SetPosition(-3.0f, 4.3f, 3.0f);

	m_Lights[1].SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);  // Green
	m_Lights[1].SetPosition(3.0f, 1.9f, 3.0f);

	m_Lights[2].SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);  // Blue
	m_Lights[2].SetPosition(-3.0f, 1.4f, -3.0f);

	m_Lights[3].SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);  // White
	m_Lights[3].SetPosition(0.0f, 6.0f, 0.0f);

	m_Lights[4].SetDiffuseColor(1.0f, 1.0f, 1.0, 1.0f); // Yellow
	m_Lights[4].SetPosition(-5.0f, 1.5f, 2);

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
		delete[] m_Lights;
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

	// Release the text string objects.
	if (m_TextString2)
	{
		m_TextString2->Shutdown();
		delete m_TextString2;
		m_TextString2 = 0;
	}

	if (m_TextString1)
	{
		m_TextString1->Shutdown();
		delete m_TextString1;
		m_TextString1 = 0;
	}

	return;
}


bool ApplicationClass::Frame() {
	m_Timer->Frame();
	float frameTime = m_Timer->GetTime();

	MoveCamera(m_Input, m_Camera, frameTime);
	m_Sprite->Update(frameTime);

	return Render();
}



bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	XMFLOAT4 diffuseColor[5], lightPosition[5];                                                       
	XMFLOAT3 attenuation;
	int i;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Get the light properties.
	for (i = 0; i < m_numLights; i++)
	{
		// Create the diffuse color array from the four light colors.
		diffuseColor[i] = m_Lights[i].GetDiffuseColor();

		// Create the light position array from the four light positions.
		lightPosition[i] = m_Lights[i].GetPosition();
	}
	attenuation.x = 1.0f; attenuation.y = 0.025; attenuation.z = 0.025;


	m_Direct3D->TurnZBufferOn();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());
	// Render the model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		diffuseColor, lightPosition, attenuation);
	if (!result) return false;

	// Disable the Z buffer and enable alpha blending for 2D rendering.
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->EnableAlphaBlending();
	result = m_Bitmap->Render(m_Direct3D->GetDeviceContext());
	if (!result) return false;
	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, XMMatrixIdentity(), orthoMatrix, m_Bitmap->GetTexture());
	if (!result) return false;

	// Put the sprite vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Sprite->Render(m_Direct3D->GetDeviceContext());
	if (!result) return false;
	// Render the sprite with the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Sprite->GetIndexCount(), worldMatrix, XMMatrixIdentity(), orthoMatrix, m_Sprite->GetTexture());
	if (!result) return false;

	// Render the first text string using the font shader.
	m_TextString1->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString1->GetIndexCount(), worldMatrix, XMMatrixIdentity(), orthoMatrix,
		m_Font->GetTexture(), m_TextString1->GetPixelColor());
	if (!result) return false;

	// Render the second text string using the font shader.
	m_TextString2->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString2->GetIndexCount(), worldMatrix, XMMatrixIdentity(), orthoMatrix,
		m_Font->GetTexture(), m_TextString2->GetPixelColor());
	if (!result) return false;

	// Enable the Z buffer and disable alpha blending now that 2D rendering is complete.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


