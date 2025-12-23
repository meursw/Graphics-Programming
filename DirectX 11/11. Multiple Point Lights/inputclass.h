#pragma once
#include <directxmath.h>
using namespace DirectX;

class InputClass {
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool isKeyDown(unsigned int);

	void SetMousePosition(XMFLOAT2);
	XMFLOAT2 GetMousePosition();

private:
	bool m_keys[256];
	XMFLOAT2 m_mousePosition;
};
