#pragma once
#include <directxmath.h>
using namespace DirectX;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE
};

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetCameraSpeed(float);
	void SetLookAtVector(XMVECTOR);

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation();
	float GetCameraSpeed();
	XMVECTOR GetLookAtVector() const;

	void UpdateRotation(XMFLOAT2);

	void Render(Camera_Movement, float);
	void GetViewMatrix(XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_mouseSensitivity = 0.1f;
	float m_cameraSpeed;

	XMMATRIX m_viewMatrix;
	XMVECTOR m_lookAtVector;
};