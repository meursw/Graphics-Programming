#pragma once

#include "baseshaderclass.h"

class TranslateShaderClass : public ShaderClass
{
private:
	struct TranslateBufferType
	{
		float translate;
		XMFLOAT3 padding;
	};

public:
	TranslateShaderClass();
	TranslateShaderClass(const TranslateShaderClass&);
	~TranslateShaderClass();

	void Shutdown();

private:
	bool InitializeShaderDerived(ID3D11Device*);
	void ShutdownShader();

	bool SetShaderParametersDerived(ID3D11DeviceContext*);
	void RenderShaderDerived(ID3D11DeviceContext*);

private:
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_translateBuffer;

public:
	ID3D11ShaderResourceView* m_texture;
	float m_translate;
};

