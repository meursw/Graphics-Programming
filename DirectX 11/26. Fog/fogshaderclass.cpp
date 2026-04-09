#include "fogshaderclass.h"

FogShaderClass::FogShaderClass() : ShaderClass()
{
	m_sampleState = 0;
	m_fogBuffer = 0;
	m_colorTexture = 0;
}


FogShaderClass::FogShaderClass(const FogShaderClass& other)
{
}


FogShaderClass::~FogShaderClass()
{
}

bool FogShaderClass::InitializeShaderDerived(ID3D11Device* device)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC fogBufferDesc;
	int numElements;

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

    // Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // Create the vertex input layout.
    result = device->CreateInputLayout(polygonLayout, numElements, m_vertexShaderBuffer->GetBufferPointer(),
        m_vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

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
    fogBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    fogBufferDesc.ByteWidth = sizeof(FogBufferType);
    fogBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    fogBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    fogBufferDesc.MiscFlags = 0;
    fogBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&fogBufferDesc, NULL, &m_fogBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}


void FogShaderClass::Shutdown()
{
    ShutdownShader();
}

void FogShaderClass::ShutdownShader()
{
    ShaderClass::Shutdown();
    // Release the light constant buffer.
    if (m_fogBuffer)
    {
        m_fogBuffer->Release();
        m_fogBuffer = 0;
    }

    // Release the sampler state.
    if (m_sampleState)
    {
        m_sampleState->Release();
        m_sampleState = 0;
    }

    if (m_colorTexture) {
        m_colorTexture->Release();
        m_colorTexture = 0;
    }

    return;
}

bool FogShaderClass::SetShaderParametersDerived(ID3D11DeviceContext* deviceContext)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    FogBufferType* dataPtr;

    // Set shader texture resources in the pixel shader.
    deviceContext->PSSetShaderResources(0, 1, &m_colorTexture);

    // Lock the light constant buffer so it can be written to.
    result = deviceContext->Map(m_fogBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // Get a pointer to the data in the constant buffer.
    dataPtr = (FogBufferType*)mappedResource.pData;

    // Copy the lighting variables into the constant buffer.
    dataPtr->fogStart = m_fogStart;
    dataPtr->fogEnd = m_fogEnd;

    // Unlock the constant buffer.
    deviceContext->Unmap(m_fogBuffer, 0);

    // Set the position of the light constant buffer in the pixel shader.

    // Finally set the light constant buffer in the pixel shader with the updated values.
    deviceContext->VSSetConstantBuffers(1, 1, &m_fogBuffer);

    return true;
}

void FogShaderClass::RenderShaderDerived(ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);
}