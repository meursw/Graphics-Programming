#pragma once

#include "baseshaderclass.h"

class NormalMapShaderClass : public ShaderClass
{
private:
    struct LightBufferType
    {
        XMFLOAT4 diffuseColor;
        XMFLOAT3 lightDirection;
        float padding;
    };

public:
    NormalMapShaderClass();
    NormalMapShaderClass(const NormalMapShaderClass&);
    ~NormalMapShaderClass();

    void Shutdown();

private:
    bool InitializeShaderDerived(ID3D11Device*);
    void ShutdownShader();

    bool SetShaderParametersDerived(ID3D11DeviceContext*);
    void RenderShaderDerived(ID3D11DeviceContext*);

private:
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_lightBuffer;

public:
    ID3D11ShaderResourceView* m_colorTexture;
    ID3D11ShaderResourceView* m_normalMapTexture;

    XMFLOAT4 m_diffuseColor;
    XMFLOAT3 m_lightDirection;
};

