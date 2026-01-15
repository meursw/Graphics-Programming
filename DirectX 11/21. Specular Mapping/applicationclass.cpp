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
	m_InputString = 0;
	m_SpecMapShader = 0;
	m_NormalMapShader = 0;

	m_prevMouseX = 0;
	m_prevMouseY = 0;

}

ApplicationClass::ApplicationClass(const ApplicationClass& o) {}
ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {

	char modelFilename[128];
	char textureFilename[128];
	char normalMapFilename[128];
	char specMapFilename[128];
	char bitmapFilename[128];
	char spriteFilename[128];
	char fpsString[128];
	char inputString[128];

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
	m_Camera->SetMouseSens(0.1f);

	// Create model class
	// Set the file name of the texture.
	// Set the file name of the model.
	strcpy_s(modelFilename, "../DX11/Assets/Models/plane.txt");
	strcpy_s(textureFilename, "../DX11/Assets/Textures/stone02.tga");
	strcpy_s(normalMapFilename, "../DX11/Assets/Textures/normal02.tga");
	strcpy_s(specMapFilename, "../DX11/Assets/Textures/spec02.tga");
	m_Model = new ModelClass[3];
	if (!m_Model[0].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename, normalMapFilename, specMapFilename)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	strcpy_s(modelFilename, "../DX11/Assets/Models/cube.txt");
	if (!m_Model[1].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename, normalMapFilename, specMapFilename)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	strcpy_s(modelFilename, "../DX11/Assets/Models/sphere.txt");
	if (!m_Model[2].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename, normalMapFilename, specMapFilename)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	
	m_SpecMapShader = new SpecMapShaderClass;
	if (!m_SpecMapShader->Initialize(m_Direct3D->GetDevice(), hwnd, (WCHAR*)L"../DX11/specmap.vs", (WCHAR*)L"../DX11/specmap.ps", "SpecMapVertexShader", "SpecMapPixelShader"))
	{
		MessageBox(hwnd, L"Could not initialize the specuular map shader object.", L"Error", MB_OK);
		return false;
	}

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
	// Set the initial fps and fps string.
	// Create and initialize the text object for the fps string.
	m_Fps = new FpsClass();
	m_Fps->Initialize();
	m_previousFps = -1;
	strcpy_s(fpsString, "Fps: 0");
	m_FpsString = new TextClass;
	if (!m_FpsString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, fpsString, 10, 10, 0.0f, 1.0f, 0.0f))
		return false;

	strcpy_s(inputString, "Input: ");
	m_InputString = new TextClass();
	if (!m_InputString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, inputString, 10, 40, 1.0f, 1.0f, 1.0f))
		return false;


	m_Lights = new LightClass;
	m_Lights->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights->SetDirection(0.0f, -1.0f, 1.0f);
	m_Lights->SetSpecularColor(2.0, 1.0, 1.0, 1.0);
	m_Lights->SetSpecularPower(8.0f);

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
		m_Model[0].Shutdown();
		m_Model[1].Shutdown();
		m_Model[2].Shutdown();
		delete[] m_Model;
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

	if (m_InputString)
	{
		m_InputString->Shutdown();
		delete m_InputString;
		m_InputString = 0;
	}


	return;
}


bool ApplicationClass::Frame(InputClass* Input, HWND hwnd) 
{
	// Check if the user pressed escape and wants to exit the application.
	if (Input->IsEscapePressed())
		return false;

	// Update FPS counter by calling m_Fps functions
	if (!UpdateFps())
		return false;

	m_Timer->Frame();
	float frameTime = m_Timer->GetTime();
	if(m_Sprite)
		m_Sprite->Update(frameTime);
		
	// Update camera position based on input from the user
	m_Camera->UpdatePosition(Input, frameTime);

	// Get the location of the mouse from the input object,
	int mouseX, mouseY;
	Input->GetMouseLocation(mouseX, mouseY);

	// Calculate mouse offset since last frame
	float offsetx = static_cast<float>(mouseX - m_prevMouseX);
	float offsety = static_cast<float>(mouseY - m_prevMouseY);
	m_prevMouseX = mouseX;
	m_prevMouseY = mouseY;

	// Update camera rotation based on mouse offset since last frame

	if (Input->IsTabPressed())
		m_mouseLocked = !m_mouseLocked;

	if (m_mouseLocked) {
		// After all calculation are done, referring to the camera rotation mainly, re center the cursor to the middle of the screen
		// This ensures the player can rotate around freely (assuming an FPS game or something similar)
		m_Camera->UpdateRotation(offsetx, offsety);
		CenterCursor(hwnd);
	}

	UpdateInput(Input);

	return Render();
}

bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	static float rotation = 360.0f;
	bool result;
	rotation -= 0.0174532925f * 0.0025f;
	if (rotation <= 0.0f)
	{
		rotation += 360.0f;
	}

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Direct3D->TurnZBufferOn();
	m_SpecMapShader->m_colorTexture = m_Model[0].GetTexture(0);
	m_SpecMapShader->m_normalMapTexture = m_Model[0].GetTexture(1);
	m_SpecMapShader->m_specMapTexture = m_Model[0].GetTexture(2);

	m_SpecMapShader->m_diffuseColor = m_Lights->GetDiffuseColor();
	m_SpecMapShader->m_specularColor = m_Lights->GetSpecularColor();
	m_SpecMapShader->m_specularPower = m_Lights->GetSpecularPower();
	m_SpecMapShader->m_lightDirection = m_Lights->GetDirection();
	
	m_SpecMapShader->m_cameraPosition = m_Camera->GetPosition();
	
	// Render the model using the normal map shader.
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			worldMatrix = XMMatrixScaling(0.0003, 0.0003, 0.0003);
		}
		else
			worldMatrix = XMMatrixMultiply(XMMatrixRotationY(rotation), XMMatrixTranslation(i * 3, 1, 0));
		m_Model[i].Render(m_Direct3D->GetDeviceContext());

		if (!m_SpecMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model[i].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
		{
			return false;
		}
	}

	

	RenderUI();
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
	if (fps > 99'999)
	{
		fps = 99'999;
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

bool ApplicationClass::UpdateInput(InputClass* Input)
{
	char inputString[128];
	strcpy_s(inputString, "Input: ");

	string temp;
	Input->GetAllPressedKeys(temp);

	strcat_s(inputString, temp.c_str());
	
	if(!m_InputString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, inputString, 10, 40, 1.0f, 1.0f, 1.0f))
		return false;

	return true;
}

void ApplicationClass::CenterCursor(HWND hwnd)
{
	// First get the client rect size
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	// Calculate the center of the screen
	int centerX = (clientRect.left + clientRect.right) / 2;
	int centerY = (clientRect.top + clientRect.bottom) / 2;

	// Turn to a point
	POINT center{ centerX, centerY };

	// Turn to screen coordinates
	ClientToScreen(hwnd, &center);

	// Set cursor to point in the center
	SetCursorPos(center.x, center.y);
}

bool ApplicationClass::RenderUI()
{
	XMMATRIX orthoMatrix;
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Disable the Z buffer and enable alpha blending for 2D rendering.
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->EnableAlphaBlending();

	// Render the FPS text string using the font shader.
	m_FpsString->Render(m_Direct3D->GetDeviceContext());
	if (!m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(), XMMatrixIdentity(), XMMatrixIdentity(), orthoMatrix,
		m_Font->GetTexture(), m_FpsString->GetPixelColor())
	) return false;

	m_InputString->Render(m_Direct3D->GetDeviceContext());
	if (!m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_InputString->GetIndexCount(), XMMatrixIdentity(), XMMatrixIdentity(), orthoMatrix,
		m_Font->GetTexture(), m_InputString->GetPixelColor())
	) return false;

	// Enable the Z buffer and disable alpha blending now that 2D rendering is complete.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();
}