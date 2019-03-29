#include "Sky.h"

struct SkyVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Norm;
	DirectX::XMFLOAT2 TexCoord;
};

struct ConstantBufferSky
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
	DirectX::XMVECTOR mEyePosition;
};

Sky::Sky()
	: g_eyePosition{ 0.0f,0.0f,0.0f }, m_pInputLayout{ nullptr }, 
	m_pIndexBuffer{ nullptr },m_pVertexBuffer { nullptr }, m_pConstantBuffer{ nullptr }
{
}

bool Sky::createMeshSky(const Renderer & const renderer, const D3D11_FILL_MODE *const wireFrame)
{
	bool hr{ 0 };

	//Create Vertex buffer
	SkyVertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },



		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },


		//Left
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },


		//Right
		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },


		//Front
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -0.0f), DirectX::XMFLOAT2(0.0f, .0f) },

		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },


		//Back
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(10.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(5.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },

	};

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(SkyVertex) * ARRAYSIZE(vertices);
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
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
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
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferSky);
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
	const UINT stride = sizeof(SkyVertex);
	const UINT offset = 0;
	renderer.GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set Index Buffer
	renderer.GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//Set Topology
	renderer.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_WorldMatrix = DirectX::XMMatrixIdentity();

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = *wireFrame;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	hr = renderer.GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_pRasterStateSky);
	renderer.GetDeviceContext()->RSSetState(m_pRasterStateSky);
	return 0;
}

void Sky::createShadersSky(const Renderer& const renderer)
{
	auto hr = DirectX::CreateDDSTextureFromFile(renderer.GetDevice(), L"Skymap.dds", nullptr, &m_pSkyTextureRV);

	if (FAILED(hr))
	{
		MessageBox(nullptr, "Texture not found", "Error in Sky.cpp", MB_OK);
		
	}

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	hr = renderer.GetDevice()->CreateSamplerState(&sampDesc, &m_pSkySampler);

	if (FAILED(hr))
	{
		MessageBox(nullptr, "Error Creating sampler state", "Error in Sky.cpp", MB_OK);
	}

	/*Depth Testing*/
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	//Depth Test Parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	//Stencil Test Parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	hr = renderer.GetDevice()->CreateDepthStencilState(&dsDesc, &m_pDepthStencilStateSky);


	//Stencil Operations if Pixel is front facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Stencil Operations if pixel is back facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR ;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;





	ID3DBlob* pVSBlob = nullptr;
	hr = renderer.CompileShaderFromFile(L"SkyMap.fx", "VS_main", "vs_4_0", &pVSBlob);

	// Create the vertex shader
	renderer.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShaderSky);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		MessageBox(nullptr, "The vertex shader connot be compiled", "Error", MB_OK);
		//return hr;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT layoutSize = ARRAYSIZE(layout);
	//// Create the input layout
	hr = renderer.GetDevice()->CreateInputLayout(layout, layoutSize, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_pInputLayout);

	pVSBlob->Release();
	pVSBlob = nullptr;
	if (FAILED(hr))
	{
		MessageBox(nullptr, "The input layout was not created correctly", "Error", MB_OK);

	}

	// Set the input layout
	renderer.GetDeviceContext()->IASetInputLayout(m_pInputLayout);

	//return hr;

// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = renderer.CompileShaderFromFile(L"SkyMap.fx", "PS_main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "The pixel shader connot be compiled", "Error", MB_OK);

		//return hr;
	}

	// Create the pixel shader
	hr = renderer.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShaderSky);
	pPSBlob->Release();

	// Set the input layout
}

void Sky::Draw(ID3D11DeviceContext *const deviceContext, const DirectX::XMMATRIX *const viewMatrix, const DirectX::XMMATRIX *const projectionMatrix)const
{
	//const auto deviceContext = renderer.GetDeviceContext();

	DirectX::XMMATRIX const g_world = DirectX::XMMatrixIdentity();

	ConstantBufferSky cb{};
	cb.mWorld = DirectX::XMMatrixTranspose(m_WorldMatrix);
	cb.mView = XMMatrixTranspose(*viewMatrix);
	cb.mProjection = XMMatrixTranspose(*projectionMatrix);
	cb.mEyePosition = DirectX::XMVectorSet(-6.0f, 8.0f,-20.0f,0.0f);

	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	deviceContext->OMSetDepthStencilState(m_pDepthStencilStateSky, 1);

	deviceContext->VSSetShader(g_pVertexShaderSky, nullptr, 0);
	deviceContext->VSSetConstantBuffers(2, 1, &m_pConstantBuffer);
	deviceContext->PSSetShader(g_pPixelShaderSky, nullptr, 0);
	deviceContext->RSSetState(m_pRasterStateSky);

	deviceContext->PSSetShaderResources(2, 1, &m_pSkyTextureRV);
	deviceContext->PSSetSamplers(1, 1, &m_pSkySampler);

	deviceContext->DrawIndexed(36, 0, 0);
	Shape::reset(m_pVertexBuffer, m_pIndexBuffer, m_pConstantBuffer);

}


Sky::~Sky()
{
	try
	{
		m_pIndexBuffer->Release();
		m_pVertexBuffer->Release();
		m_pConstantBuffer->Release();
		m_pInputLayout->Release();
		g_pPixelShaderSky->Release();
		g_pVertexShaderSky->Release();
		m_pSkySampler->Release();
		m_pSkyTextureRV->Release();
		m_pDepthStencilStateSky->Release();
		m_pRasterStateSky->Release();
	}
	catch (std::exception &e)
	{
		MessageBox(nullptr, "Error in Sky while shutting down", "Error in Sky", MB_OK);
	}
}
