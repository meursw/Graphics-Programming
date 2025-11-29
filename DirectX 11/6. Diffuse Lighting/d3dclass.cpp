#include "d3dclass.h"

#define HR_TEST(result) {if(FAILED(result)) return false; }

D3DClass::D3DClass() {
	m_swapChain = 0; // used to swap frames from back buffer to front buffer.
	m_device = 0; // used to create dx11 resources.
	m_deviceContext = 0; // used to set states and issue rendering commands.

	m_renderTargetView = 0; // a view that points to one of the swap chains buffers.

	m_depthStencilBuffer = 0; // a texture containing depth information per pixel.
	m_depthStencilState = 0; // defines how the depth/stencil buffer should be used.
	m_depthStencilView = 0; // a view that tells the pipeline to use the depth texture.

	m_rasterState = 0; // defines how polygons are filled, sets backface culling & depth bias

}

D3DClass::D3DClass(const D3DClass& other) {}

D3DClass::~D3DClass() {}

// The Initialize function is what does the entire setup of Direct3D for DirectX 11.

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vysnc, HWND hwnd, bool fullscreen, float screenDepth, float screenNear) {
	HRESULT result;

	IDXGIFactory* factory; // Used to create DXGI objects
	IDXGIAdapter* adapter; // Represents display subsystem (GPUs etc)
	IDXGIOutput* adapterOutput; // Represents an adapter output (such as a monitor).
	DXGI_ADAPTER_DESC adapterDesc; // Graphics card description (name, VRAM, etc)

	DXGI_MODE_DESC* displayModeList; // Describes a display mode, display modes get infromation about the monitor and use this information for the swap chain buffers
	unsigned int numModes;  // How many display modes available
	unsigned int numerator, denominator; // refresh rate ex. 60 / 1 = 60hz

	DXGI_SWAP_CHAIN_DESC swapChainDesc; // Describes the settings of a swap chain

	D3D_FEATURE_LEVEL featureLevel; // D3D feature level like 11.0, 11.1 etc.

	ID3D11Texture2D* backBufferPtr; // Pointer to the back buffer

	D3D11_TEXTURE2D_DESC depthBufferDesc; // Describes the depth buffer texture
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc; // Describes depth-stencil state.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; // Specifies the subresources of a texture that are accessible from a depth-stencil view.

	D3D11_RASTERIZER_DESC rasterDesc; // Describes rasterizer state.

	unsigned long long stringLength;
	unsigned int i;
	float fieldOfView, screenAspect;
	int error;

	// Before we can initialize Direct3D we have to get the refresh rate from the video card/monitor.
	// Each computer may be slightly different so we will need to query for that information. 
	// We query for the numerator and denominator values and then pass them to DirectX during the setup and it will calculate the proper refresh rate.


	////// START OF GETTING INFORMATION ABOUT THE MONITOR AND VIDEO CARD and storing it to member variables.

	// Create DX graphics interface factory. We use uuidof to get identifier of an interace.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	HR_TEST(result);

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	HR_TEST(result);

	// Enumerate the primary adapter output (monitor)
	result = adapter->EnumOutputs(0, &adapterOutput);
	HR_TEST(result);

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	HR_TEST(result);

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	HR_TEST(result);

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	HR_TEST(result);

	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024); // in megabytes

	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0) return false;

	// Now that we have stored the numerator and denominator for the refresh rate and the video card information, 
	// we can release the structures and interfaces used to get that information.

	delete[] displayModeList; displayModeList = 0;
	adapterOutput->Release(); adapterOutput = 0;
	adapter->Release(); adapter = 0;
	factory->Release(); factory = 0;

	//////// END OF GETTING INFORMATION ABOUT THE MONITOR AND VIDEO CARD

	// Now that we have the refresh rate from the system, we can start the DirectX initialization.
	// The first thing we'll do is fill out the description of the swap chain. 
	// The swap chain is the front and back buffer to which the graphics will be drawn. 
	// Generally, you use a single back buffer, do all your drawing to it, and then swap it to the front buffer which then displays on the user's screen. 
	// That is why it is called a swap chain.

	
	//////// START OF SWAP CHAIN INITIALIZATION 

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer
	swapChainDesc.BufferCount = 1;

	// Set width and height of the back buffer
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// The next part of the description of the swap chain is the refresh rate. 
	// The refresh rate is how many times a second it draws the back buffer to the front buffer.

	if (m_vsync_enabled) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	swapChainDesc.Windowed = !fullscreen;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	//////// END OF SWAP CHAIN INITIALIZATION 


	// Now that the swap chain description and feature level have been filled out, we can create the swap chain, the Direct3D device, and the Direct3D device context.
	// The Direct3D device and Direct3D device context are very important, they are the interface to all of the Direct3D functions.

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	HR_TEST(result);

	// Get the pointer to the back buffer from the swapchain.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	HR_TEST(result);

	// Create the render target view with the back buffer pointer
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	HR_TEST(result);

	// Release as we no longer need.
	backBufferPtr->Release();
	backBufferPtr = 0;
	

	// We will also need to set up a depth buffer description. 
	// We'll use this to create a depth buffer so that our polygons can be rendered properly in 3D space. 
	// At the same time, we will attach a stencil buffer to our depth buffer. 
	// The stencil buffer can be used to achieve effects such as motion blur, volumetric shadows, and other things.

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create depth/stencil buffer texture. 
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	HR_TEST(result);

	// Now we need to setup the depth stencil description. 
	// This allows us to control what type of depth test Direct3D will do for each pixel.

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	HR_TEST(result);

	// With the created depth stencil state, we can now set it so that it takes effect.
	// Notice we use the device context to set it.
	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	HR_TEST(result);

	// Bind the render target and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);



	// Now that the render targets are setup, we can continue on to some extra functions that will give us more control over our scenes for future tutorials.
	// First thing we'll create is a rasterizer state.

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	HR_TEST(result);

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering.
	m_viewport.Width = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);



	// Now we will create the projection matrix. 
	// The projection matrix is used to translate the 3D scene into the 2D viewport space that we previously created. 
	// We will need to keep a copy of this matrix so that we can pass it to our shaders that will be used to render our scenes.

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();

	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
		
}

void D3DClass::Shutdown() {
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception
	if (m_swapChain) {
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState) {
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView) {
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState) {
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer) {
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView) {
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext) {
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device) {
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain) {
		m_swapChain->Release();
		m_swapChain = 0;
	}
}

// Initializes the buffers so they are blank and ready to draw to.
void D3DClass::BeginScene(float red, float green, float blue, float alpha) {
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene() {
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device* D3DClass::GetDevice() {
	return m_device;
}


ID3D11DeviceContext* D3DClass::GetDeviceContext() {
	return m_deviceContext;
}

void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix) {
	projectionMatrix = m_projectionMatrix;
	return;
}


void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix) {
	worldMatrix = m_worldMatrix;
	return;
}


void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix) {
	orthoMatrix = m_orthoMatrix;
	return;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory) {
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3DClass::SetBackBufferRenderTarget() {
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return;
}

void D3DClass::ResetViewport() {
	// Set the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);

	return;
}
