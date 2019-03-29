#pragma once
#include <d3d11.h>
#include "Cube.h"
#include "Camera.h"
#include "Renderer.h"
#include "Shape.h"
#include  "DDSTextureLoader.h"
#include <DirectXColors.h>

class Sky : public Shape
{
public:
	Sky();
	Sky(const Sky &source) = delete;
	Sky& operator = (const Sky &t) = delete;
	bool createMeshSky(const Renderer & const renderer, const D3D11_FILL_MODE *const wireFrame);
	void createShadersSky(const Renderer & const renderer);

	void Draw(ID3D11DeviceContext *const deviceContext, const DirectX::XMMATRIX *const viewMatrix, const DirectX::XMMATRIX *const projectionMatrix)const;
	~Sky();

private:
	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMVECTOR g_eyePosition;
	ID3D11InputLayout *m_pInputLayout;

	ID3D11VertexShader*     g_pVertexShaderSky = nullptr;
	ID3D11PixelShader*      g_pPixelShaderSky = nullptr;
	ID3D11Buffer *m_pIndexBuffer;
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pConstantBuffer;

	ID3D11ShaderResourceView* m_pSkyTextureRV = nullptr;
	ID3D11SamplerState*		m_pSkySampler = nullptr;
	ID3D11DepthStencilState* m_pDepthStencilStateSky = nullptr;
	ID3D11RasterizerState*	m_pRasterStateSky = nullptr;
};

