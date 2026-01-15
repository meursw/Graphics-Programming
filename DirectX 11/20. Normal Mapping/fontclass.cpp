#include "fontclass.h"

FontClass::FontClass()
{
    m_Font = 0;
    m_Texture = 0;
}


FontClass::FontClass(const FontClass& other)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int fontChoice)
{
    char fontFilename[128];
    char fontTextureFilename[128];
    bool result;

    switch (fontChoice)
    {
        case 0:
        default:
        {

            strcpy_s(fontFilename, "../DX11/Assets/Fonts/font01.txt");
            strcpy_s(fontTextureFilename, "../DX11/Assets/Fonts/font01.tga");
            m_fontHeight = 32.0f;
            m_spaceSize = 3;
            break;
        }
    }

    result = LoadFontData(fontFilename);
    if (!result) return false;

    result = LoadTexture(device, deviceContext, fontTextureFilename);
    if (!result) return false;

    return true;
}

void FontClass::Shutdown()
{
    ReleaseTexture();
    ReleaseFontData();
}

bool FontClass::LoadFontData(char* filename)
{
    ifstream fin;
    char temp;

    m_Font = new FontType[95];

    fin.open(filename);
    if (fin.fail()) return false;

    for (int i = 0; i < 95; i++) {
        fin.get(temp);
        while (temp != ' ')
        {
            fin.get(temp);
        }
        fin.get(temp);
        while (temp != ' ')
        {
            fin.get(temp);
        }

        fin >> m_Font[i].left;
        fin >> m_Font[i].right;
        fin >> m_Font[i].size;
    }

    fin.close();

    return true;
}

void FontClass::ReleaseFontData()
{
    if(m_Font){
        delete[] m_Font;
        m_Font = 0;
    }
}

void FontClass::ReleaseTexture()
{
    // Release the texture object.
    if (m_Texture)
    {
        m_Texture->Shutdown();
        delete m_Texture;
        m_Texture = 0;
    }

    return;
}

bool FontClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
    m_Texture = new TextureClass;
    if (!m_Texture->Initialize(device, deviceContext, filename))
        return false;

    return true;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
    return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
    VertexType* vertexPtr;
    int numLetters, index, letter;

    vertexPtr = (VertexType*)vertices;

    numLetters = (int)strlen(sentence);
    index = 0;

    for (int i = 0; i < numLetters; i++)
    {
        letter = ((int)sentence[i]) - 32;

        if (letter == 0)
            drawX += m_spaceSize;
        else
        {
            // First triangle in quad.
            vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.5f);  // Top left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.5f);  // Bottom right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3(drawX, (drawY - m_fontHeight), 0.5f);  // Bottom left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
            index++;

            // Second triangle in quad.
            vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.5f);  // Top left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.5f);  // Top right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.5f);  // Bottom right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
            index++;

            // Update the x location for drawing by the size of the letter and one pixel.
            drawX = drawX + m_Font[letter].size + 1.0f;
        }

    }

    return;
}

// The GetSentencePixelLength function returns how long the sentence is in terms of pixels.
// You can use this for centering or otherwise positioning the text sentences on the screen.
int FontClass::GetSentencePixelLength(const char* sentence)
{
    int pixelLength, numLetters, i, letter;

    pixelLength = 0;
    numLetters = (int)strlen(sentence);

    for (i = 0; i < numLetters; i++)
    {
        letter = ((int)sentence[i]) - 32;

        // If the letter is a space then count it as three pixels.
        if (letter == 0)
        {
            pixelLength += m_spaceSize;
        }
        else
        {
            pixelLength += (m_Font[letter].size + 1);
        }
    }

    return pixelLength;
}

int FontClass::GetFontHeight()
{
    return (int)m_fontHeight;
}