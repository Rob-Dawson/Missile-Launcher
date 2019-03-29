#pragma once
#include "Shape.h"
class Launcher :
	public Shape
{
public:
	Launcher();
	Launcher(const Launcher &source) = delete;
	Launcher& operator = (const Launcher &t) = delete;
	bool createMeshLauncher(const Renderer &renderer, const D3D11_FILL_MODE *const wireFrame);
	bool createShadersLauncher(Renderer &renderer);
	void draw(ID3D11DeviceContext *const deviceContext, const DirectX::XMMATRIX *const viewMatrix, const DirectX::XMMATRIX *const projectionMatrix/*, float time*/)const;

	~Launcher();

private:
	DirectX::XMMATRIX m_RocketMatrix;

	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_viewMatrix;

	DirectX::XMMATRIX m_ProjectionMatrix;


	ID3D11VertexShader *m_pVertexShader;
	ID3D11PixelShader *m_pPixelShader;

	ID3D11InputLayout *m_pInputLayout;

	ID3D11Buffer *m_pIndexBuffer;
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pConstantBuffer;

	ID3D11ShaderResourceView *m_pStoneHeightTexture;
	ID3D11ShaderResourceView *m_pStoneShader;
	ID3D11SamplerState *m_pStoneSampler;
	ID3D11RasterizerState* m_rasterState;



};

