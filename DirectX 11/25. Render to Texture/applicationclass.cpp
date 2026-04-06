#include "applicationclass.h"

ApplicationClass::ApplicationClass() { 
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
	m_RenderTexture = 0;
	m_DisplayPlane = 0;

	m_prevMouseX = 0;
	m_prevMouseY = 0;

	m_mouseLocked = true;

}

ApplicationClass::ApplicationClass(const ApplicationClass& o) {}
ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {

	char modelFilename[128];
	char textureFilename[128];

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
	m_Model = new ModelClass;

	strcpy_s(modelFilename, "../DX11/Assets/Models/cube.txt");
	strcpy_s(textureFilename, "../DX11/Assets/Sprites/Tenna Dance/tenna_dance (66).png");
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the render to texture object.
	m_RenderTexture = new RenderTextureClass;

	if (!m_RenderTexture->Initialize(m_Direct3D->GetDevice(), 4096, 4096, SCREEN_DEPTH, SCREEN_NEAR, 1))
		return false;

	// Create and initialize the display plane object.
	m_DisplayPlane = new DisplayPlaneClass;

	if (!m_DisplayPlane->Initialize(m_Direct3D->GetDevice(), 1.0f, 1.0f))
		return false;
	
	m_Timer = new TimerClass;
	if (!m_Timer->Initialize())
		return false;

	return true;
}

void ApplicationClass::Shutdown()
{
	// Release the display plane object.
	if (m_DisplayPlane)
	{
		m_DisplayPlane->Shutdown();
		delete m_DisplayPlane;
		m_DisplayPlane = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
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

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	return;
}


bool ApplicationClass::Frame(InputClass* Input, HWND hwnd) 
{
	// Check if the user pressed escape and wants to exit the application.
	if (Input->IsEscapePressed())
		return false;

	static float totalTime = 0;
	
	m_Timer->Frame();
	float frameTime = m_Timer->GetTime();
	totalTime += frameTime;
	if (totalTime >= 1000.0)
		totalTime -= 1000.0;

	if (!RenderSceneToTexture(totalTime))
		return false;
		
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

	if (Input->IsTabPressed()) {
		ShowCursor(m_mouseLocked);
		m_mouseLocked = !m_mouseLocked;
	}

	if (m_mouseLocked) {
		// After all calculation are done, referring to the camera rotation mainly, re center the cursor to the middle of the screen
		// This ensures the player can rotate around freely (assuming an FPS game or something similar)
		m_Camera->UpdateRotation(offsetx, offsety);
		CenterCursor(hwnd);
	}

	return Render();
}


bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_Direct3D->TurnZBufferOn();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);


	// Setup matrices - Top display plane.
	worldMatrix = XMMatrixTranslation(0.0f, 1.5f, 0.0f);
	// Render the display plane using the texture shader and the render texture resource.
	m_DisplayPlane->Render(m_Direct3D->GetDeviceContext());
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTexture->GetShaderResourceView()))
		return false;

	// Setup matrices - Bottom left display plane.
	worldMatrix = XMMatrixTranslation(-1.5f, -1.5f, 0.0f);

	// Render the display plane using the texture shader and the render texture resource.
	m_DisplayPlane->Render(m_Direct3D->GetDeviceContext());

	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTexture->GetShaderResourceView());
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTexture->GetShaderResourceView()))
		return false;

	// Setup matrices - Bottom right display plane.
	worldMatrix = XMMatrixTranslation(1.5f, -1.5f, 0.0f);
	m_DisplayPlane->Render(m_Direct3D->GetDeviceContext());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTexture->GetShaderResourceView());
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTexture->GetShaderResourceView()))
		return false;


	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::RenderSceneToTexture(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0, 0.5f, 1.0f, 1.0f);

	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_RenderTexture->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixRotationY(rotation);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture()))
		return false;

	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderUI(int renderCount)
{
	XMMATRIX orthoMatrix;
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Disable the Z buffer and enable alpha blending for 2D rendering.
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->EnableAlphaBlending();

	// Enable the Z buffer and disable alpha blending now that 2D rendering is complete.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

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