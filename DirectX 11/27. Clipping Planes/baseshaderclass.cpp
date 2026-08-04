#include "baseshaderclass.h"

ShaderClass::ShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ShaderClass::ShaderClass(const ShaderClass* o) {}

ShaderClass::~ShaderClass(){}

bool ShaderClass::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, const char* vsEntryPoint, const char* psEntryPoint)
{
	if (!InitializeShaderBase(device, hwnd, vsFilename, psFilename, vsEntryPoint, psEntryPoint))
		return false;

	if (!InitializeShaderDerived(device))
		return false;

	return true;
}

bool ShaderClass::InitializeShaderBase(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, const char* vsEntryPoint, const char* psEntryPoint)
{
	HRESULT result;
	ID3D10Blob* pixelShaderBuffer = 0;
	ID3D10Blob* errorMessage = 0;

	D3D11_BUFFER_DESC matrixBufferDesc;

	result = D3DCompileFromFile(vsFilename, NULL, NULL, vsEntryPoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_vertexShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		else
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
	}

	result = D3DCompileFromFile(psFilename, NULL, NULL, psEntryPoint, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		else
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);

		return false;
	}

	result = device->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result)) return false;

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result)) return false;

	pixelShaderBuffer->Release(); pixelShaderBuffer = 0;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)) return false;

	return true;
}

void ShaderClass::Shutdown()
{
	ShutdownShader();
}

bool ShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) {
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

void ShaderClass::ShutdownShader() {
	if (m_vertexShader) {
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	if (m_pixelShader) {
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_layout) {
		m_layout->Release();
		m_layout = 0;
	}

	if (m_matrixBuffer) {
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
}

void ShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors ;
	unsigned long long bufferSize;
	ofstream fout;

	bufferSize = errorMessage->GetBufferSize();
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	fout.open("shader-error.txt");

	for (int i = 0; i < bufferSize; i++) 
		fout << compileErrors[i];

	fout.close();

	errorMessage->Release(); errorMessage = 0;

	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}

bool ShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!SetShaderParametersBase(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
		return false;

	if (!SetShaderParametersDerived(deviceContext))
		return false;

	return true;
}

bool ShaderClass::SetShaderParametersBase(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) 
{	
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	result = deviceContext->Map(m_matrixBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	if (FAILED(result)) return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(m_matrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);
	
	return true;
}

void ShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	RenderShaderBase(deviceContext);
	RenderShaderDerived(deviceContext);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void ShaderClass::RenderShaderBase(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
}