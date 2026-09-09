#pragma once
#include "baseshaderclass.h"

class ClipPlaneShaderClass : public ShaderClass
{
private:
	struct ClipPlaneBufferType
	{
		XMFLOAT4 clipPlane;
	};

public:
	ClipPlaneShaderClass();
	ClipPlaneShaderClass(const ClipPlaneShaderClass&);
	~ClipPlaneShaderClass();

	void Shutdown();

private:
	bool InitializeShaderDerived(ID3D11Device*);
	void ShutdownShader();

	bool SetShaderParametersDerived(ID3D11DeviceContext*);
	void RenderShaderDerived(ID3D11DeviceContext*);

private:
	ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_clipPlaneBuffer;

public:
	ID3D11ShaderResourceView* m_texture;
	XMFLOAT4 m_clipPlane;
};

