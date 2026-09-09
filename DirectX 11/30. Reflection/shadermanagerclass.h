#pragma once

#include <vector>

#include "baseshaderclass.h"

#include "specmapshaderclass.h"
#include "normalmapshaderclass.h"

class ShaderManagerClass
{
public:
    ShaderManagerClass();
    ShaderManagerClass(const ShaderManagerClass&);
    ~ShaderManagerClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();

    bool RenderShaders(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, int);

    void InsertShader(ShaderClass*);
    vector<ShaderClass*>& GetShaders();

private:
    vector<ShaderClass*> m_Shaders;
};