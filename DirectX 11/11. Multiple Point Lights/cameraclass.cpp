#include "cameraclass.h";

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void CameraClass::SetCameraSpeed(float speed)
{
	m_cameraSpeed = speed;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

float CameraClass::GetCameraSpeed()
{
	return m_cameraSpeed;
}

void CameraClass::UpdateRotation(XMFLOAT2 offset)
{
	m_rotationY += offset.x * m_mouseSensitivity;
	m_rotationX += offset.y * m_mouseSensitivity;

	if (m_rotationX > 89.0f)
		m_rotationX = 89.0f;
	if (m_rotationX < -89.0f)
		m_rotationX = -89.0f;
	
	Render(NONE, 0.0f);

}


// The render function uses the position and rotation of the camera to build and update the view matrix. 
// We need and up vector, a position vector, a rotation, lookAt vector etc.
// At the origin we first rotate the camera based on the x, y, and z rotation of the camera.
// After rotation, we translate the position in 3D Space.
// With position, lookAt and up vectors filled in, we use XMMatrixLookAtLH to create the view matrix that represents our camera

void CameraClass::Render(Camera_Movement direction, float deltaTime) {
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f; up.y = 1.0f; up.z = 0.0f;
	upVector = XMLoadFloat3(&up);

	float velocity = m_cameraSpeed * deltaTime;

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;
    positionVector = XMLoadFloat3(&position);

	// also known as the front vector
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	lookAtVector = XMVector3Normalize(lookAtVector);

	// Right Vector for movement
	XMVECTOR rightVector = XMVector3Cross(upVector, lookAtVector);
	rightVector = XMVector3Normalize(rightVector);

	// Up Vector for movement
	XMVECTOR objectUpVector = XMVector3Cross(lookAtVector, rightVector);
	objectUpVector = XMVector3Normalize(objectUpVector);

	XMFLOAT3 posVec = position;
	XMVECTOR moveDirection = XMVectorZero();
	
	switch (direction)
	{
	case FORWARD:
		moveDirection += lookAtVector;
		break;
	case BACKWARD:
		moveDirection -= lookAtVector;
		break;
	case LEFT:
		moveDirection -= rightVector;
		break;
	case RIGHT:
		moveDirection += rightVector; 
		break;
	case UP:
		moveDirection += objectUpVector;
		break;
	case DOWN:
		moveDirection -= objectUpVector;
		break;
	}

	if (direction != NONE) {
		moveDirection = XMVector3Normalize(moveDirection);
		positionVector += moveDirection * velocity;

		XMStoreFloat3(&posVec, positionVector);
		SetPosition(posVec.x, posVec.y, posVec.z);
	}
	
	m_viewMatrix = XMMatrixLookAtLH(positionVector, positionVector + lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix) {
	viewMatrix = m_viewMatrix;
	return;
}
