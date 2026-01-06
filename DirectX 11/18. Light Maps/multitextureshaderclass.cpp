#include "multitextureshaderclass.h"

MultiTextureShaderClass::MultiTextureShaderClass()
{
    m_vertexShader = 0;
    m_pixelShader = 0;
    m_layout = 0;
    m_matrixBuffer = 0;
    m_sampleState = 0;
}


MultiTextureShaderClass::MultiTextureShaderClass(const MultiTextureShaderClass& other)
{
}


MultiTextureShaderClass::~MultiTextureShaderClass()
{
}

bool MultiTextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    int error;
    wchar_t vsFilename[128], psFilename[128];

    error = wcscpy_s(vsFilename, L"../DX11/multitexture.vs");
    if (error != 0) return false;

    error = wcscpy_s(psFilename, L"../DX11/multitexture.ps");
    if (error != 0) return false;


    if (!InitializeShader(device, hwnd, vsFilename, psFilename))
        return false;

    return true;
}

void MultiTextureShaderClass::Shutdown()
{
    ShutdownShader();
}

bool MultiTextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, 
    ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2)
{
    if (!SetShaderParameters(deviceContext, world, view, projection, texture1, texture2))
        return false;

    RenderShader(deviceContext, indexCount);

    return true;
}

bool MultiTextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
    HRESULT hr;
    ID3D10Blob* vertexShader;
    ID3D10Blob* pixelShader;
    ID3D10Blob* errorMessage;

    hr = D3DCompileFromFile(vsFilename, NULL, NULL, "MultiTextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShader, &errorMessage);
    if (FAILED(hr)) {
        if (errorMessage)
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        else
            MessageBox(hwnd, L"Missing Shader File", vsFilename, MB_OK);
        return false;
    }

    hr = D3DCompileFromFile(psFilename, NULL, NULL, "MultiTexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShader, &errorMessage);
    if (FAILED(hr)) {
        if (errorMessage)
            OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
        else
            MessageBox(hwnd, L"Missing Shader File", psFilename, MB_OK);
        return false;
    }

    hr = device->CreateVertexShader(vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), NULL, &m_vertexShader);
    if (FAILED(hr)) return false;

    hr = device->CreatePixelShader(pixelShader->GetBufferPointer(), pixelShader->GetBufferSize(), NULL, &m_pixelShader);
    if (FAILED(hr)) return false;

    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
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

    int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
    hr = device->CreateInputLayout(polygonLayout, numElements, vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), &m_layout);
    if (FAILED(hr)) return false;

    vertexShader->Release(); vertexShader = 0;
    pixelShader->Release(); pixelShader = 0;

    D3D11_BUFFER_DESC matrixBufferDesc;

    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    hr = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    if (FAILED(hr)) return false;

    D3D11_SAMPLER_DESC samplerDesc;
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

    hr = device->CreateSamplerState(&samplerDesc, &m_sampleState);
    if (FAILED(hr)) return false;

    return true;
}

void MultiTextureShaderClass::ShutdownShader()
{
    // Release the sampler state.
    if (m_sampleState)
    {
        m_sampleState->Release();
        m_sampleState = 0;
    }

    // Release the matrix constant buffer.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = 0;
    }

    // Release the layout.
    if (m_layout)
    {
        m_layout->Release();
        m_layout = 0;
    }

    // Release the pixel shader.
    if (m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = 0;
    }

    // Release the vertex shader.
    if (m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = 0;
    }

    return;
}

void MultiTextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
    ofstream errorFile;
    errorFile.open("shader-error.txt");

    char* compileErrors = (char*)errorMessage->GetBufferPointer();
    unsigned long long bufferSize = errorMessage->GetBufferSize();

    for (int i = 0; i < bufferSize; i++)
    {
        errorFile << compileErrors[i];
    }

    errorFile.close();

    errorMessage->Release();
    errorMessage = 0;

    MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message", shaderFilename, MB_OK);
}

bool MultiTextureShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection,
    ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2)
{
    HRESULT hr;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    int bufferNumber;

    // Transpose the matrices to prepare them for the shader.
    world = XMMatrixTranspose(world);
    view = XMMatrixTranspose(view);
    projection = XMMatrixTranspose(projection);

    hr = deviceContext->Map(m_matrixBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
    if (FAILED(hr)) return false;

    dataPtr = (MatrixBufferType*)mappedResource.pData;

    dataPtr->world = world;
    dataPtr->view = view;
    dataPtr->projection = projection;

    deviceContext->Unmap(m_matrixBuffer, NULL);

    bufferNumber = 0;
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

    deviceContext->PSSetShaderResources(0, 1, &texture1);
    deviceContext->PSSetShaderResources(1, 1, &texture2);

    return true;
}

void MultiTextureShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    deviceContext->IASetInputLayout(m_layout);
    
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);

    deviceContext->DrawIndexed(indexCount, 0, 0);
}