#include "shadermanagerclass.h"

ShaderManagerClass::ShaderManagerClass() {}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass& other) {}


ShaderManagerClass::~ShaderManagerClass() {}

void ShaderManagerClass::InsertShader(ShaderClass* shader)
{
    m_Shaders.push_back(shader);
}

vector<ShaderClass*>& ShaderManagerClass::GetShaders()
{
    return m_Shaders;
}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;
    
    for (ShaderClass* Shader : m_Shaders) {
        if (dynamic_cast<SpecMapShaderClass*>(Shader)) {
            result = Shader->Initialize(device, hwnd, (WCHAR*)L"../DX11/specmap.vs", (WCHAR*)L"../DX11/specmap.ps", "SpecMapVertexShader", "SpecMapPixelShader");
            if(!result) return false;
        }

        if (dynamic_cast<NormalMapShaderClass*>(Shader)) {
            result = Shader->Initialize(device, hwnd, (WCHAR*)L"../DX11/normalmap.vs", (WCHAR*)L"../DX11/normalmap.ps", "NormalMapVertexShader", "NormalMapPixelShader");
            if (!result) return false;
        }
    }

    return true;
}

void ShaderManagerClass::Shutdown()
{
    for (ShaderClass* Shader : m_Shaders)
        Shader->Shutdown();
}

bool ShaderManagerClass::RenderShaders(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, int shaderIndex)
{
    bool result;
    result = m_Shaders[shaderIndex]->Render(deviceContext, indexCount, world, view, projection);
    if (!result) return false;

    return true;
}