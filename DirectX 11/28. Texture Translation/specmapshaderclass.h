#pragma once
#include "baseshaderclass.h"

class SpecMapShaderClass : public ShaderClass
{
private:
    struct LightBufferType
    {
        XMFLOAT4 diffuseColor;
        XMFLOAT4 specularColor;
        float specularPower;
        XMFLOAT3 lightDirection;
    };

    struct CameraBufferType
    {
        XMFLOAT3 cameraPosition;
        float padding;
    };

public:
    SpecMapShaderClass();
    SpecMapShaderClass(const SpecMapShaderClass&);
    ~SpecMapShaderClass();

    void Shutdown();

private:
    bool InitializeShaderDerived(ID3D11Device*);
    void ShutdownShader();

    bool SetShaderParametersDerived(ID3D11DeviceContext*);
    void RenderShaderDerived(ID3D11DeviceContext*);

private:
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_cameraBuffer;
    ID3D11Buffer* m_lightBuffer;

public:
    ID3D11ShaderResourceView* m_colorTexture;
    ID3D11ShaderResourceView* m_normalMapTexture;
    ID3D11ShaderResourceView* m_specMapTexture;

    XMFLOAT4 m_diffuseColor;
    XMFLOAT4 m_specularColor;
    float m_specularPower;
    XMFLOAT3 m_lightDirection;

    XMFLOAT3 m_cameraPosition;
};

