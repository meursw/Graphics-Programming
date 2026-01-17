#include "specmapshaderclass.h"

SpecMapShaderClass::SpecMapShaderClass() : ShaderClass()
{
	m_colorTexture = 0;
	m_normalMapTexture = 0;
	m_specMapTexture = 0;
}
SpecMapShaderClass::SpecMapShaderClass(const SpecMapShaderClass&){}
SpecMapShaderClass::~SpecMapShaderClass(){}

void SpecMapShaderClass::Shutdown()
{
    ShutdownShader();
}

void SpecMapShaderClass::ShutdownShader()
{
	ShaderClass::Shutdown();
	if (m_colorTexture) {
		m_colorTexture->Release();
		m_colorTexture = 0;
	}

	if (m_normalMapTexture) {
		m_normalMapTexture->Release();
		m_normalMapTexture = 0;
	}

	if (m_specMapTexture) {
		m_specMapTexture->Release();
		m_specMapTexture = 0;
	}

	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}
}

bool SpecMapShaderClass::InitializeShaderDerived(ID3D11Device* device)
{
    HRESULT result;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
    D3D11_SAMPLER_DESC samplerDesc;
    D3D11_BUFFER_DESC lightBufferDesc;
    D3D11_BUFFER_DESC cameraBufferDesc;
    int numElements;

    // Create the vertex input layout description.
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

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    polygonLayout[3].SemanticName = "TANGENT";
    polygonLayout[3].SemanticIndex = 0;
    polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[3].InputSlot = 0;
    polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[3].InstanceDataStepRate = 0;

    polygonLayout[4].SemanticName = "BINORMAL";
    polygonLayout[4].SemanticIndex = 0;
    polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[4].InputSlot = 0;
    polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[4].InstanceDataStepRate = 0;

    // Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // Create the vertex input layout.
    result = device->CreateInputLayout(polygonLayout, numElements, m_vertexShaderBuffer->GetBufferPointer(),
        m_vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
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

    // Create the texture sampler state.
    result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
    if (FAILED(result))
    {
        return false;
    }

    // Setup the description of the light dynamic constant buffer that is in the pixel shader.
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightBufferType);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
    if (FAILED(result))
    {
        return false;
    }

    cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
    cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cameraBufferDesc.MiscFlags = 0;
    cameraBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

bool SpecMapShaderClass::SetShaderParametersDerived(ID3D11DeviceContext* deviceContext)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CameraBufferType* dataPtr;
    LightBufferType* dataPtr2;
    int bufferNumber;

    // Set camera buffer
    result = deviceContext->Map(m_lightBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
    dataPtr = (CameraBufferType*)mappedResource.pData;
    dataPtr->cameraPosition = m_cameraPosition;
    dataPtr->padding = 0.0f;

    deviceContext->Unmap(m_lightBuffer, NULL);

    // 1 because we already set the matrix buffer in the base class
    bufferNumber = 1;
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
    
    // Set light buffer
    result = deviceContext->Map(m_lightBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
    dataPtr2 = (LightBufferType*)mappedResource.pData;
    dataPtr2->diffuseColor = m_diffuseColor;
    dataPtr2->specularColor = m_specularColor;
    dataPtr2->specularPower = m_specularPower;
    
    dataPtr2->lightDirection = m_lightDirection;
    deviceContext->Unmap(m_lightBuffer, NULL);

    bufferNumber = 0;
    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

    deviceContext->PSSetShaderResources(0, 1, &m_colorTexture);
    deviceContext->PSSetShaderResources(1, 1, &m_normalMapTexture);
    deviceContext->PSSetShaderResources(2, 1, &m_specMapTexture);

    return true;;
}

void SpecMapShaderClass::RenderShaderDerived(ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);
}