#include "ClipPlaneShaderClass.h"

ClipPlaneShaderClass::ClipPlaneShaderClass() : ShaderClass()
{
    m_sampleState = 0;
    m_clipPlaneBuffer = 0;
}

ClipPlaneShaderClass::ClipPlaneShaderClass(const ClipPlaneShaderClass& o) {}

ClipPlaneShaderClass::~ClipPlaneShaderClass() {}

bool ClipPlaneShaderClass::InitializeShaderDerived(ID3D11Device* device)
{
    HRESULT result;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    D3D11_SAMPLER_DESC samplerDesc;
    D3D11_BUFFER_DESC clipPlaneBufferDesc;
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

    // Set up description for clip plane dynamic constant buffer that is in the vertex shader.
    clipPlaneBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    clipPlaneBufferDesc.ByteWidth = sizeof(ClipPlaneBufferType);
    clipPlaneBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    clipPlaneBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    clipPlaneBufferDesc.MiscFlags = 0;
    clipPlaneBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&clipPlaneBufferDesc, NULL, &m_clipPlaneBuffer);
    if (FAILED(result)) return false;

    return true;
}

void ClipPlaneShaderClass::Shutdown()
{
    ShutdownShader();
}

void ClipPlaneShaderClass::ShutdownShader()
{
    ShaderClass::Shutdown();

    // Release the clip plane constant buffer.
    if (m_clipPlaneBuffer)
    {
        m_clipPlaneBuffer->Release();
        m_clipPlaneBuffer = 0;
    }

    // Release the sampler state.
    if (m_sampleState)
    {
        m_sampleState->Release();
        m_sampleState = 0;
    }

    return;
}

bool ClipPlaneShaderClass::SetShaderParametersDerived(ID3D11DeviceContext* deviceContext)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ClipPlaneBufferType* dataPtr;

    // Set shader texture resource in the pixel shader.
    deviceContext->PSSetShaderResources(0, 1, &m_texture);

    // Lock the clip plane constant buffer so it can be written to
    result = deviceContext->Map(m_clipPlaneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) return false;

    dataPtr = (ClipPlaneBufferType*)mappedResource.pData;

    dataPtr->clipPlane = m_clipPlane;

    deviceContext->Unmap(m_clipPlaneBuffer, 0);

    deviceContext->VSSetConstantBuffers(1, 1, &m_clipPlaneBuffer);

    return true;
}

void ClipPlaneShaderClass::RenderShaderDerived(ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);
}