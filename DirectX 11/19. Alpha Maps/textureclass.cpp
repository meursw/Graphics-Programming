#include "textureclass.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

TextureClass::TextureClass() {
	m_TextureData = 0;
	m_texture = 0;
	m_textureView = 0;
}

TextureClass::TextureClass(const TextureClass& o){}

TextureClass::~TextureClass() {}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, float scaleWidth, float scaleHeight) {
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hr;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	unsigned rowPitch;


	//if (!LoadTarga32Bit(filename)) return false;
	if (!LoadWithStbImage(filename)) return false;
	if (!ResizeStbImage(scaleWidth, scaleHeight)) return false;

	textureDesc.Height = m_height;
	textureDesc.Width = m_width;
	textureDesc.MipLevels = 4;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hr)) return false;

	rowPitch = (m_width*4) * sizeof(unsigned char);

	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_TextureData, rowPitch, 0);

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hr)) return false;
	
	deviceContext->GenerateMips(m_textureView);

	delete[] m_TextureData;
	m_TextureData = 0;

	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture view resource.
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	// Release the targa data.
	if (m_TextureData)
	{
		delete[] m_TextureData;
		m_TextureData = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

bool TextureClass::LoadTarga32Bit(char* filename) {
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) return false;

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1) return false;

	m_height = (int)targaFileHeader.height;
	m_width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	if (bpp != 32) return false;

	imageSize = m_width * m_height * 4;

	targaImage = new unsigned char[imageSize];

	// Read in the targa image data
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize) return false;

	error = fclose(filePtr);
	if (error != 0) return false;

	// Allocate memory for the targa destination data.
	m_TextureData = new unsigned char[imageSize];

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
	k = (m_width * m_height * 4) - (m_width * 4);

	for (j = 0; j < m_height; j++) {
		for (i = 0; i < m_width; i++) {
			m_TextureData[index + 0] = targaImage[k + 2]; // Red
			m_TextureData[index + 1] = targaImage[k + 1]; // Green
			m_TextureData[index + 2] = targaImage[k + 0]; // Blue
			m_TextureData[index + 3] = targaImage[k + 3]; // Alpha

			k += 4;
			index += 4;
		}
		k -= (m_width * 8);
	}

	delete[] targaImage;
	targaImage = 0;

	return true;
}

bool TextureClass::LoadWithStbImage(char* filename) {
	int width, height;
	m_TextureData = stbi_load(filename, &width, &height, NULL, 0);
	m_width = width;
	m_height = height;

	return true;
}

bool TextureClass::ResizeStbImage(float scaleWidth, float scaleHeight) {
	if (scaleWidth == 1.0f && scaleHeight == 1.0f)
		return true;

	int width = m_width / scaleWidth;
	int height = m_height / scaleHeight;

	int newImageSize = width * height * 4;

	unsigned char* newImageData = new unsigned char[newImageSize];
	if (!newImageData) return false;

	stbir_resize_uint8(
		m_TextureData, m_width, m_height, 0,
		newImageData, width, height, 0,
		4
	);

	delete[] m_TextureData;

	m_TextureData = newImageData;

	m_width = width;
	m_height = height;

	return true;
}


int TextureClass::GetWidth()
{
	return m_width;
}


int TextureClass::GetHeight()
{
	return m_height;
}