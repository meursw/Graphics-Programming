#include "bitmapclass.h"

BitmapClass::BitmapClass()
{
    m_vertexBuffer = 0;
    m_indexBuffer = 0;
    m_Texture = 0;
}


BitmapClass::BitmapClass(const BitmapClass& other)
{
}


BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, char* textureFilename, int renderX, int renderY, int scaleWidth, int scaleHeight)
{
    bool result;

    // Store the screen size.
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Store where the bitmap should be rendered to.
    m_renderX = renderX;
    m_renderY = renderY;

    // Initialize the vertex and index buffer that hold the geometry for the bitmap quad.
    result = InitializeBuffers(device);
    if (!result)
    {
        return false;
    }

    // Load the texture for this bitmap.
    result = LoadTexture(device, deviceContext, textureFilename, scaleWidth, scaleHeight);
    if (!result)
    {
        return false;
    }

    return true;
}

void BitmapClass::Shutdown()
{
    // Release the bitmap texture.
    ReleaseTexture();

    // Release the vertex and index buffers.
    ShutdownBuffers();

    return;
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext)
{
    // Update the buffers if the position of the bitmap has changed from its original position.
    if (!UpdateBuffers(deviceContext))
        return false;
    
    // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
    RenderBuffers(deviceContext);

    return true;
}

int BitmapClass::GetIndexCount()
{
    return m_indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
    return m_Texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    m_prevPosX = -1;
    m_prevPosY = -1;

    m_vertexCount = 6;
    m_indexCount = m_vertexCount;

    vertices = new VertexType[m_vertexCount];
    indices = new unsigned long[m_indexCount];

    memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

    for (int i = 0; i < m_indexCount; i++)
        indices[i] = i;

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
        return false;

    // Set up the description of the index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Release the arrays now that the vertex and index buffers have been created and loaded.
    delete[] vertices;
    vertices = 0;

    delete[] indices;
    indices = 0;

    return true;
}

void BitmapClass::ShutdownBuffers()
{
    // Release the index buffer.
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    // Release the vertex buffer.
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }

    return;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
    float left, right, top, bottom;
    VertexType* vertices;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    VertexType* dataPtr;
    HRESULT result;

    if((m_prevPosX == m_renderX) && (m_prevPosY == m_renderY))
        return true;

    m_prevPosX = m_renderX;
    m_prevPosY = m_renderY;

    vertices = new VertexType[m_vertexCount];

    left = (float)((m_screenWidth / 2) * -1) + (float)m_renderX;
    right = left + (float)m_bitmapWidth;

    top = (float)(m_screenHeight / 2) - (float)m_renderY;
    bottom = top - (float)m_bitmapHeight;

    // Load the vertex array with data.
    // First triangle.
    vertices[0].position = XMFLOAT3(left, top, 0.5f);  // Top left.
    vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

    vertices[1].position = XMFLOAT3(right, bottom, 0.5f);  // Bottom right.
    vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

    vertices[2].position = XMFLOAT3(left, bottom, 0.5f);  // Bottom left.
    vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

    // Second triangle.
    vertices[3].position = XMFLOAT3(left, top, 0.5f);  // Top left.
    vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

    vertices[4].position = XMFLOAT3(right, top, 0.5f);  // Top right.
    vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

    vertices[5].position = XMFLOAT3(right, bottom, 0.5f);  // Bottom right.
    vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

    // Lock the vertex buffer.
    result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    dataPtr = (VertexType*)mappedResource.pData;

    memcpy(dataPtr, (void*)vertices, sizeof(VertexType) * m_vertexCount);

    deviceContext->Unmap(m_vertexBuffer, 0);

    dataPtr = 0;
    delete[] vertices;
    vertices = 0;

    return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride, offset;
    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool BitmapClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, int scaleWidth, int scaleHeight)
{
    bool result;


    // Create and initialize the texture object.
    m_Texture = new TextureClass;

    result = m_Texture->Initialize(device, deviceContext, filename, scaleWidth, scaleHeight);
    if (!result)
    {
        return false;
    }

    // Store the size in pixels that this bitmap should be rendered at.
    m_bitmapWidth = m_Texture->GetWidth();
    m_bitmapHeight = m_Texture->GetHeight();

    return true;
}

void BitmapClass::ReleaseTexture()
{
    // Release the texture object.
    if (m_Texture)
    {
        m_Texture->Shutdown();
        delete m_Texture;
        m_Texture = 0;
    }

    return;
}

void BitmapClass::SetRenderLocation(int x, int y)
{
    m_renderX = x;
    m_renderY = y;
    return;
}