#pragma once
#include "Shape.h"
class Rocket :
	public Shape
{
public:
	Rocket();
	Rocket(const Rocket &source) = delete;
	Rocket& operator = (const Rocket &t) = delete;
	bool createMeshRocket(const Renderer &renderer, const D3D11_FILL_MODE *const wireFrame);
	bool createShadersRocket(const Renderer & const renderer);
	void resetRocket();
	void draw(ID3D11DeviceContext *deviceContext, const DirectX::XMMATRIX *const viewMatrix, const DirectX::XMMATRIX *const projectionMatrix, bool const launch);
	
	const inline DirectX::XMMATRIX *getWorldMatrix() const 
	{
		return &m_RocketMatrix;
	}
	
	~Rocket();

private:
	float y = 0.0f;
	ID3D11RasterizerState* m_rasterState = nullptr;
	DirectX::XMMATRIX m_RocketMatrix;
	DirectX::XMFLOAT4X4 aa;
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
	ID3D11ShaderResourceView *m_pStoneHeightTexture;

	ID3D11ShaderResourceView *m_pStoneShader;
	ID3D11SamplerState *m_pStoneSampler;
};

