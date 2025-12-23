#include "inputclass.h"

InputClass::InputClass() {}
InputClass::InputClass(const InputClass&) {}
InputClass::~InputClass() {}

void InputClass::Initialize() {
	for (int i = 0; i < 256; i++) {
		m_keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int input) {
	m_keys[input] = true;
}

void InputClass::KeyUp(unsigned int input) {
	m_keys[input] = false;
}

bool InputClass::isKeyDown(unsigned int key) {
	return m_keys[key];
}

void InputClass::SetMousePosition(XMFLOAT2 mousePos)
{
	m_mousePosition = mousePos;
}

XMFLOAT2 InputClass::GetMousePosition()
{
	return m_mousePosition;
}
