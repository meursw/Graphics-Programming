#pragma once

#include <d3d11.h>
#include <stdio.h>

class TextureClass {
private:
	struct TargaHeader {
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, float = 1.0f, float = 1.0f);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	int GetWidth();
	int GetHeight();

private:
	bool LoadTarga32Bit(char*);
	bool LoadWithStbImage(char*);
	bool ResizeStbImage(float,float);

private:
	unsigned char* m_TextureData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
	int m_width, m_height;
};

