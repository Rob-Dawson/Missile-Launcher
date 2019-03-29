#include "Rocket.h"
#include "DDSTextureLoader.h"

struct RocketVertex
{
	XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexCoord;
};


struct ConstantBufferRocket
{
	XMMATRIX mWorld;
	XMMATRIX oWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

Rocket::Rocket()
	: m_pIndexBuffer{ nullptr }, m_pVertexBuffer{ nullptr }, m_pConstantBuffer{ nullptr },
	m_pPixelShader{ nullptr }, m_pVertexShader{ nullptr }, m_pInputLayout{ nullptr },
	m_pStoneHeightTexture{nullptr}, m_pStoneShader{nullptr}, m_pStoneSampler{nullptr} 
{

}

bool Rocket::createMeshRocket(const Renderer &renderer, const D3D11_FILL_MODE *const wireFrame)
{
	bool hr{ 0 };

	const float radius = -0.5f;
	const float heightHEX = -2.0f;
	const float theta = 2.0 * XM_PI / 6;

	//Create Vertex buffer
	RocketVertex vertices[] =
	{

		{XMFLOAT3(0.0f, 5.0f,0.0f), XMFLOAT2(0.5,0.5)},

		{ XMFLOAT3(radius * cos(0 * theta), heightHEX, radius * sin(0 * theta)), XMFLOAT2(radius * cos(0 * theta), 1.0f)},
		{ XMFLOAT3(radius * cos(1 * theta), heightHEX, radius * sin(1 * theta)), XMFLOAT2(0.5f, 1.0f)},
		{ XMFLOAT3(radius * cos(2 * theta), heightHEX, radius * sin(2 * theta)), XMFLOAT2(0.0f,0.0f)},
		{ XMFLOAT3(radius * cos(3 * theta), heightHEX, radius * sin(3 * theta)), XMFLOAT2(0.0f,0.0f)},
		{ XMFLOAT3(radius * cos(4 * theta), heightHEX, radius * sin(4 * theta)), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(radius * cos(5 * theta), heightHEX, radius * sin(5 * theta)), XMFLOAT2(0.0f,0.0f)},
		{ XMFLOAT3(radius * cos(6 * theta), heightHEX, radius * sin(6 * theta)),XMFLOAT2(0.0,0.0f)},
		{ XMFLOAT3(radius * cos(7 * theta), heightHEX, radius * sin(7 * theta)),XMFLOAT2(0.0f,0.0f)},

		{XMFLOAT3(0.0f, -5.0f,0.0f),XMFLOAT2(0.5,0.5) },

		{ XMFLOAT3(radius * cos(0 * theta), heightHEX - 5, radius * sin(0 * theta)),XMFLOAT2(0.5f, 0.5f)},
		{ XMFLOAT3(radius * cos(1 * theta), heightHEX - 5, radius * sin(1 * theta)),XMFLOAT2(0.5f, 0.0f)},
		{ XMFLOAT3(radius * cos(2 * theta), heightHEX - 5, radius * sin(2 * theta)), XMFLOAT2(0.5, 0.5f)},
		{ XMFLOAT3(radius * cos(3 * theta), heightHEX - 5, radius * sin(3 * theta)),XMFLOAT2(0.5f,0.5f) },
		{ XMFLOAT3(radius * cos(4 * theta), heightHEX - 5, radius * sin(4 * theta)), XMFLOAT2(0.5f,0.5f)},
		{ XMFLOAT3(radius * cos(5 * theta), heightHEX - 5, radius * sin(5 * theta)),XMFLOAT2(0.5f,0.5f)},
		{ XMFLOAT3(radius * cos(6 * theta), heightHEX - 5, radius * sin(6 * theta)),XMFLOAT2(0.5f,0.5f)},
		{ XMFLOAT3(radius * cos(7 * theta), heightHEX - 5, radius * sin(7 * theta)), XMFLOAT2(0.5f,0.5f)},

	};

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(RocketVertex) * ARRAYSIZE(vertices);
	vertexDesc.CPUAccessFlags = 0;
	vertexDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = vertices;

	hr = renderer.GetDevice()->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Error Creating vertex buffer", "Error", MB_OK);
		return hr;
	}

	//Create Index Buffer
	WORD indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		//9, 9, 10,
		//
		9, 10, 11,
		9, 11, 12,
		9, 12, 13,
		9, 13, 14,
		9, 15, 16,
		9, 16, 17,

		0, 9 ,2,
		9, 11 ,2,

				1, 10 ,2,
				10, 11 ,2,

		0, 9 ,3,
		9, 12 ,3,

				2, 11 ,3,
				11, 12 ,3,

		0, 9 ,4,
		9, 13 ,4,

				3, 12 ,4,
				12, 13 ,4,

		0, 9 ,5,
		9, 14 ,5,

				4, 13, 5,
				13, 14, 5,

		0, 9 ,6,
		9, 15 ,6,

				5, 14, 6,
				14, 15, 6,

		0, 9 ,7,
		9, 16 ,7,

				6, 15, 7,
				15, 16, 7,

		0, 9 ,8,
		9, 17 ,8,

				7, 16, 8,
				16, 17, 8,
	};
	D3D11_BUFFER_DESC indexDesc;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);
	 indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;
	resourceData.pSysMem = indices;
	hr = renderer.GetDevice()->CreateBuffer(&indexDesc, &resourceData, &m_pIndexBuffer);

	if (FAILED(hr))
	{
		MessageBox(nullptr, "Error creating index buffer", "Error", MB_OK);
		return hr;
	}

	//Create Constant Buffer
	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferRocket);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	hr = renderer.GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffer);

	if (FAILED(hr))
	{
		MessageBox(nullptr, "Error creating Constant buffer", "Error", MB_OK);
		return hr;
	}

	// Set vertex buffer
	const UINT stride = sizeof(RocketVertex);
	const UINT offset = 0;
	renderer.GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set Index Buffer
	renderer.GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//Set Topology
	renderer.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//renderer.GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_rasterState);
	//renderer.GetDeviceContext()->RSSetState(m_rasterState);

	m_WorldMatrix = DirectX::XMMatrixIdentity();

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = *wireFrame;
	rasterDesc.ScissorEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	hr = renderer.GetDevice()->CreateRasterizerState(&rasterDesc, &m_rasterState);
	renderer.GetDeviceContext()->RSSetState(m_rasterState);
	return 0;
}

bool Rocket::createShadersRocket(const Renderer& const renderer)
{
	//auto hr = DirectX::CreateDDSTextureFromFile(renderer.GetDevice(), L"Skymap.dds", nullptr, &m_pSkyTextureRV);

	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, "Texture not found", "Error in Sky.cpp", MB_OK);
	//	return hr;
	//}

	//D3D11_SAMPLER_DESC sampDesc;
	//ZeroMemory(&sampDesc, sizeof(sampDesc));
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//hr = renderer.GetDevice()->CreateSamplerState(&sampDesc, &m_pSkySampler);

	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, "Error Creating sampler state", "Error in Sky.cpp", MB_OK);
	//	return hr;
	//}

	///*Depth Testing*/
	//D3D11_DEPTH_STENCIL_DESC dsDesc;
	////Depth Test Parameters
	//dsDesc.DepthEnable = true;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	////Stencil Test Parameters
	//dsDesc.StencilEnable = false;
	//dsDesc.StencilReadMask = 0xFF;
	//dsDesc.StencilWriteMask = 0xFF;

	//hr = renderer.GetDevice()->CreateDepthStencilState(&dsDesc, &m_pDepthStencilStateSky);


	////Stencil Operations if Pixel is front facing
	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	////Stencil Operations if pixel is back facing
	//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;



	//hr = renderer.GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_pRasterStateSky);
	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	bool a = DirectX::CreateDDSTextureFromFile(renderer.GetDevice(), L"stones.DDS", nullptr, &m_pStoneShader);
	a = DirectX::CreateDDSTextureFromFile(renderer.GetDevice(), L"stones_NM_height.DDS", nullptr, &m_pStoneHeightTexture);
	renderer.GetDevice()->CreateSamplerState(&sampDesc, &m_pStoneSampler);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT layoutSize = ARRAYSIZE(layout);

	Shape::createShaders(renderer, layout, layoutSize, L"Rocket.fx");

	m_pVertexShader = Shape::getVertexShader();
	m_pPixelShader = Shape::getPixelShader();
	m_WorldMatrix = DirectX::XMMatrixIdentity();
	m_RocketMatrix = DirectX::XMMatrixIdentity();
	return 0;
}


void Rocket::draw(ID3D11DeviceContext *const deviceContext, const DirectX::XMMATRIX *const viewMatrix, const DirectX::XMMATRIX *const projectionMatrix, bool const launch)
{
	if (launch)
	{
		m_RocketMatrix *= DirectX::XMMatrixTranslation(0.0, 0.005, 0.0);
	}	//	XMStoreFloat4x4(&aa, m_RocketMatrix);

	//float y = aa._42;

	//if (y >= 7.0)
	//{
	//	m_RocketMatrix += DirectX::XMMatrixRotationZ(1);
	//	y = 7.0;
	//}
	//}
	//m_WorldMatrix *= DirectX::XMMatrixRotationZ(-time * 0.5);
	//m_WorldMatrix *= DirectX::XMMatrixTranslation(0.0f, 3.0f, 0.0f);

	m_WorldMatrix = XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	ConstantBufferRocket cb{};
	cb.mWorld = XMMatrixTranspose(m_RocketMatrix);
	cb.oWorld = XMMatrixTranspose(m_WorldMatrix);
	cb.mView = XMMatrixTranspose(*viewMatrix);
	cb.mProjection = XMMatrixTranspose(*projectionMatrix); //this should be inside the camera
	//class and call the projection matrix by using a get method
	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	deviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(4, 1, &m_pConstantBuffer);
	deviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	deviceContext->PSSetShaderResources(2, 1, &m_pStoneShader);
	deviceContext->PSSetSamplers(2, 1, &m_pStoneSampler);
	deviceContext->PSSetShaderResources(3, 1, &m_pStoneHeightTexture);

	deviceContext->DrawIndexed(100, 0, 0);
	Shape::reset(m_pVertexBuffer, m_pIndexBuffer, m_pConstantBuffer);

}

void Rocket::resetRocket()
{
	m_RocketMatrix = DirectX::XMMatrixIdentity();
}

Rocket::~Rocket()
{
	try
	{
		m_pIndexBuffer->Release();
		m_pVertexBuffer->Release();
		m_pConstantBuffer->Release();
		m_pPixelShader->Release();
		m_pVertexShader->Release();
		m_pStoneHeightTexture->Release();
		m_pStoneShader->Release();
	}
	catch (std::exception &e)
	{
		MessageBox(nullptr, "Error in rocket while shutting down", "Error in rocket", MB_OK);
	}
}