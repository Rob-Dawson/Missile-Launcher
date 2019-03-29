#include "Camera.h"
#include <DirectXColors.h>

//USE XMFLOAT4X4

Camera::Camera()
	: forward{0.0f,0.0f,0.0f}, back{ 0.0f,0.0f,0.0f }, left{ 0.0f,0.0f,0.0f }, right{ 0.0f,0.0f,0.0f }, up{ 0.0f,0.0f,0.0f }, down{ 0.0f,0.0f,0.0f }
	{
	//this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//posVector = XMLoadFloat3(&pos);

	//rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//rotVector = XMLoadFloat3(&rot);
	//RenderCamera();

	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->RenderCamera();
}

void Camera::setPosition(const XMVECTOR &pos123)
{
	XMStoreFloat3(&this->pos, pos123);
	posVector = pos123;
	RenderCamera();
}

void Camera::setPosition(float const &xPos, float const &yPos, float const &zPos)
{
	this->pos = XMFLOAT3(xPos, yPos, zPos);
	this->posVector = XMLoadFloat3(&this->pos);
	this->RenderCamera();
}

void Camera::setRotation(float const &xRot, float const &yRot, float const &zRot)
{
	this->rot = XMFLOAT3(xRot, yRot, zRot);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->RenderCamera();

}

void Camera::AdjustRotation(float const &xAPos, float const &yAPos, float const &zAPos)
{
	this->rot.x += xAPos;
	this->rot.y += yAPos;
	this->rot.z += zAPos;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->RenderCamera();
}

void Camera::AdjustRotation(const XMVECTOR & rot123)
{
	rotVector += rot123;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->RenderCamera();
}

void Camera::AdjustPosition(const XMVECTOR & pos123)
{
	//send position of vector to sky
	posVector += pos123;
	XMStoreFloat3(&this->pos, this->posVector);
	this->RenderCamera();
}

void Camera::AdjustPosition(float const &xAPos, float const &yAPos, float const &zAPos)
{
	this->pos.x += xAPos;
	this->pos.y += yAPos;
	this->pos.z += zAPos;
	this->posVector = XMLoadFloat3(&this->pos);
 	this->RenderCamera();
}

void Camera::setLookAt(XMFLOAT3 & lookAtPos)
{
	const float epsilon = 0.0001;

	if ((lookAtPos.x - epsilon <= this->pos.x && this->pos.x <= epsilon + lookAtPos.x) && (lookAtPos.y - epsilon <= this->pos.y && this->pos.y <= epsilon + lookAtPos.y) && (lookAtPos.z - epsilon <= this->pos.z && this->pos.z <= epsilon + lookAtPos.z))
		return;
	//if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
	//	return;
	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	/*if ((lookAtPos.y - epsilon < 0.0f && 0.0f < lookAtPos.y + epsilon) || (lookAtPos.y - epsilon > 0.0f && 0.0f > lookAtPos.y + epsilon))*/
	if((lookAtPos.y - epsilon < 0.0f) || (lookAtPos.y + epsilon > 0.0f))
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x +  lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if ((lookAtPos.x - epsilon < lookAtPos.x) || (lookAtPos.x - epsilon > lookAtPos.x) || (lookAtPos.x + epsilon < lookAtPos.x) || (lookAtPos.x + epsilon > lookAtPos.x))
	/*if(lookAtPos.x != 0.0f)*/
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if ((lookAtPos.z - epsilon < lookAtPos.z) || (lookAtPos.z - epsilon > lookAtPos.z) || (lookAtPos.z + epsilon < lookAtPos.z) || (lookAtPos.z + epsilon > lookAtPos.z))
	{
		yaw += XM_PI;
	}

	this->setRotation(pitch, yaw, 0.0f);
}


void Camera::RenderCamera()
{

	XMMATRIX const camRotation = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMVECTOR camTarget = XMVector3TransformCoord(defaultForward, camRotation);
	camTarget += posVector;

	XMVECTOR const upDir = XMVector3TransformCoord(defaultUp, camRotation);
	m_ViewMatrix = XMMatrixLookAtLH(posVector, camTarget, upDir);//&XMMatrixLookAtLH(posVector, camTarget, upDir);
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);

	XMMATRIX const rotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
	forward = XMVector3TransformCoord(defaultForward, rotationMatrix);
	back = XMVector3TransformCoord(defaultBack, rotationMatrix);
	left = XMVector3TransformCoord(defaultLeft, rotationMatrix);
	right = XMVector3TransformCoord(defaultRight, rotationMatrix);
	up = XMVector3TransformCoord(defaultUp, rotationMatrix);
	down = XMVector3TransformCoord(defaultDown , rotationMatrix);
}

//void Camera::RenderCamera()
//{
//	const auto up = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	const auto upVector = XMLoadFloat3(&up);
//
//	const auto eye = XMFLOAT3(m_PosX, m_PosY, m_PosZ);
//	const auto eyeVector = XMLoadFloat3(&eye);
//
//	const auto at = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	const auto atVector = XMLoadFloat3(&at);
//
//	const auto pitch = XMConvertToRadians(m_RotX);
//	const auto yaw = XMConvertToRadians(m_RotY);
//	const auto roll = XMConvertToRadians(m_RotZ);
//
//	//const auto rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
//
//	//atVector = XMVector3TransformCoord(atVector, rotationMatrix);
//	//upVector = XMVector3TransformCoord(upVector, rotationMatrix);
//
//	//atVector = XMVectorAdd(eyeVector, atVector);
//
//	m_ViewMatrix = XMMatrixLookAtLH(eyeVector, atVector, upVector);
//	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
//
//}

//XMFLOAT4X4 Camera::getViewMatrix()
//XMMATRIX Camera::getViewMatrix()
//{
//	return m_ViewMatrix;
//
//}


Camera::~Camera()
{
}
