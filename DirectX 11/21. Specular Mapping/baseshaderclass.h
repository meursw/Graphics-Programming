#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class ShaderClass {
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ShaderClass();
	ShaderClass(const ShaderClass*);
	~ShaderClass();

	bool Initialize(ID3D11Device*, HWND, WCHAR*, WCHAR*, const char*, const char*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

protected:
	bool InitializeShaderBase(ID3D11Device*, HWND, WCHAR*, WCHAR*, const char*, const char*);
	virtual bool InitializeShaderDerived(ID3D11Device*) = 0;
	
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	bool SetShaderParametersBase(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	virtual bool SetShaderParametersDerived(ID3D11DeviceContext*) = 0;
	
	void RenderShader(ID3D11DeviceContext*, int);
	void RenderShaderBase(ID3D11DeviceContext*);
	virtual void RenderShaderDerived(ID3D11DeviceContext*) = 0;

protected:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D10Blob* m_vertexShaderBuffer;
};