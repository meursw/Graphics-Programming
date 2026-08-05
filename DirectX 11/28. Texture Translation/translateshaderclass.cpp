#include "translateshaderclass.h"

TranslateShaderClass::TranslateShaderClass() : ShaderClass() 
{
	m_sampleState = 0;
	m_translateBuffer = 0;
	m_texture = 0;
}

TranslateShaderClass::TranslateShaderClass(const TranslateShaderClass&) {}
TranslateShaderClass::~TranslateShaderClass() {}

bool TranslateShaderClass::InitializeShaderDerived(ID3D11Device* device)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	int numElements;

	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC translateBufferDesc;

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result)) return false;

	m_vertexShaderBuffer->Release();
	m_vertexShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result)) return false;

	translateBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	translateBufferDesc.ByteWidth = sizeof(TranslateBufferType);
	translateBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	translateBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	translateBufferDesc.MiscFlags = 0;
	translateBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&translateBufferDesc, NULL, &m_translateBuffer);
	if (FAILED(result)) return false;

	return true;
}

void TranslateShaderClass::Shutdown()
{
	ShutdownShader();
}

void TranslateShaderClass::ShutdownShader()
{
	ShaderClass::Shutdown();

	if (m_sampleState) {
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (m_translateBuffer) {
		m_translateBuffer->Release();
		m_translateBuffer = 0;
	}

	return;
}

bool TranslateShaderClass::SetShaderParametersDerived(ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	TranslateBufferType* dataPtr;

	deviceContext->PSSetShaderResources(0, 1, &m_texture);

	result = deviceContext->Map(m_translateBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	dataPtr = (TranslateBufferType*)mappedResource.pData;

	dataPtr->translate = m_translate;

	deviceContext->Unmap(m_translateBuffer, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &m_translateBuffer);
	
	return true;
}

void TranslateShaderClass::RenderShaderDerived(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
}