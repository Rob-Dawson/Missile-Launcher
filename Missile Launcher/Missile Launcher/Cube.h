#pragma once
#include "Renderer.h"
#include "Camera.h"
#include <DirectXMath.h>
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube(/*Renderer const &renderer*/);
	Cube(const Cube &source);
	Cube &operator=(const Cube &rhs) = delete;
	//bool createMesh(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	bool createMeshCube(Renderer &renderer, const D3D11_FILL_MODE *const wireFrame);

	bool createShadersCube(Renderer &renderer);
	void draw(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX *viewMatrix, DirectX::XMMATRIX *projectionMatrix, float time);

	void update();
	
	void ShutDown();

	~Cube();

private:
	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_viewMatrix;

	DirectX::XMMATRIX m_ProjectionMatrix;

	//ID3D11SamplerState *colorMapSampler_ = nullptr;
	ID3D11Buffer *m_pIndexBuffer;
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pConstantBuffer;
	
	ID3D11PixelShader *m_pPixelShader;
	ID3D11VertexShader *m_pVertexShader ;

	ID3D11InputLayout *m_pInputLayout;

};

