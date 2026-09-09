#pragma once
#include "baseshaderclass.h"

class FogShaderClass : public ShaderClass
{
private:
	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		float padding1, padding2;
	};

public:
	FogShaderClass();
	FogShaderClass(const FogShaderClass&);
	~FogShaderClass();

	void Shutdown();

private:
	bool InitializeShaderDerived(ID3D11Device*);
	void ShutdownShader();

	bool SetShaderParametersDerived(ID3D11DeviceContext*);
	void RenderShaderDerived(ID3D11DeviceContext*);

private:
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_fogBuffer;

public:
	ID3D11ShaderResourceView* m_colorTexture;
	float m_fogStart, m_fogEnd;

};

