#pragma once
#include "Shape.h"
class Moon :
	public Shape
{
public:
	Moon();
	Moon(const Moon &source) = delete;
	Moon& operator = (const Moon &t) = delete;
	bool createMeshMoon(const Renderer &renderer, const D3D11_FILL_MODE *const wireFrame);
	bool createShadersMoon(const Renderer & const renderer);

	const inline XMVECTOR &getLightPos()const
	{
		return lightPos;
	}

	void draw(ID3D11DeviceContext *const deviceContext, const DirectX::XMMATRIX *const viewMatrix, const DirectX::XMMATRIX * const projectionMatrix, float const time);
	~Moon();

private:
	ID3D11RasterizerState* m_rasterState = nullptr;

	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_viewMatrix;

	DirectX::XMMATRIX m_ProjectionMatrix;

	//ID3D11SamplerState *colorMapSampler_ = nullptr;
	ID3D11Buffer *m_pIndexBuffer;
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pConstantBuffer;

	ID3D11PixelShader *m_pPixelShader;
	ID3D11VertexShader *m_pVertexShader;

	ID3D11InputLayout *m_pInputLayout;
	XMVECTOR lightPos;

};

