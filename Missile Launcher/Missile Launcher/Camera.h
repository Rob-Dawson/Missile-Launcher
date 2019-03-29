#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(const Camera &camera) =  delete;
	Camera &operator=(const Camera &rhs) = delete;

	void setPosition(const XMVECTOR &pos);
	void setPosition(float const &xPos, float const &yPos, float const &zPos);

	void setRotation(const XMVECTOR &pos);
	void setRotation(float const &xRot, float const &yRot, float const &zRot);

	void AdjustPosition(const XMVECTOR & pos);
	void AdjustPosition(float const &xAPos, float const &yAPos, float const &zAPos);

	void AdjustRotation(const XMVECTOR & rot);
	void AdjustRotation(float const &xAPos, float const &yAPos, float const &zAPos);

	void setLookAt(XMFLOAT3 & lookAtPos);

	inline const XMMATRIX & getViewMatrix() const
	{
		return m_ViewMatrix;
	}
	
	inline const XMMATRIX & getProjectionMatrix() const
	{
 		return m_ProjectionMatrix;
	}

	inline const  XMVECTOR &GetForward() const 
	{
		return forward;
	}

	inline const XMVECTOR &GetBack() const
	{
		return back;

	}

	inline const XMVECTOR &GetLeft() const
	{
		return left;
	}

	inline const XMVECTOR &GetRight()const 
	{
		return right;
	}

	inline const XMVECTOR &GetUp()const
	{
		return up;
	}

	inline const XMVECTOR &GetDown()const
	{
		return down;
	}

	void RenderCamera();

	//XMFLOAT4X4 getViewMatrix();
	~Camera();

private:


	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMMATRIX m_ViewMatrix;
	XMMATRIX m_ProjectionMatrix;

	const XMVECTOR defaultForward   = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR defaultBack		= XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR defaultLeft		= XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR defaultRight		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	const XMVECTOR defaultUp		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR defaultDown		= XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	XMVECTOR forward;
	XMVECTOR back;
	XMVECTOR left;
	XMVECTOR right;
	XMVECTOR up;
	XMVECTOR down;

	//XMFLOAT4X4 m_ViewMatrix;
};

