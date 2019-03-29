#include "Moon.h"

struct MoonVertex
{
	DirectX::XMFLOAT3 Pos;
	//DirectX::XMFLOAT3 Norm;
	DirectX::XMFLOAT4 Color;
};

struct ConstantBufferMoon
{
	DirectX::XMMATRIX mWorld;
	//DirectX::XMMATRIX gWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
	DirectX::XMVECTOR mEyePosition;
	DirectX::XMVECTOR lightPosition;
};

Moon::Moon()
	: m_pIndexBuffer{ nullptr }, m_pVertexBuffer{ nullptr }, m_pConstantBuffer{ nullptr },
	m_pPixelShader{ nullptr }, m_pVertexShader{ nullptr }, m_pInputLayout{ nullptr }, lightPos{ 0.0,0.0,0.0,0.0 }
{

}

bool Moon::createMeshMoon(const Renderer &renderer, const D3D11_FILL_MODE *const wireFrame)
{
	bool hr{ 0 };

	//Create Vertex buffer
	MoonVertex vertices[]
	{
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0) },

		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

		//Bottom
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},


		//Left
	  { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},


	  //Right
	  { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},


	  //Front
	 { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},


	 //Back
	 { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0) },

	 { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f,0.0f,0.0f,0.0)},
	};

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(MoonVertex) * ARRAYSIZE(vertices);
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
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferMoon);
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
	const UINT stride = sizeof(MoonVertex);
	const UINT offset = 0;
	renderer.GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set Index Buffer
	renderer.GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//Set Topology
	renderer.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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


bool Moon::createShadersMoon(const Renderer & const renderer)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	const UINT layoutSize = ARRAYSIZE(layout);

	Shape::createShaders(renderer, layout, layoutSize, L"Moon.fx");
	m_pVertexShader = Shape::getVertexShader();
	m_pPixelShader = Shape::getPixelShader();

	
	lightPos = XMVectorSet(0.0f, 80.0f, 80.0f, 0.0f);

	return true;
}

void Moon::draw(ID3D11DeviceContext *const deviceContext, const DirectX::XMMATRIX *const viewMatrix, const DirectX::XMMATRIX * const projectionMatrix, float const time)
{
//	const auto deviceContext = renderer.GetDeviceContext();

	lightPos = XMVectorSet(-20.0f, -5.0f, -10.0f, 0.0f);
	m_WorldMatrix = DirectX::XMMatrixIdentity();

	const XMMATRIX mRotate = XMMatrixRotationZ(time * 1.5);
	lightPos = XMVector3Transform(lightPos, mRotate);

	m_WorldMatrix *= XMMatrixTranslation(-20.0f, -5.0f, 10.0f);
	m_WorldMatrix *= mRotate;

	ConstantBufferMoon cb{};
	cb.mWorld = XMMatrixTranspose(m_WorldMatrix);
	cb.mView = XMMatrixTranspose(*viewMatrix);
	cb.mProjection = XMMatrixTranspose(*projectionMatrix);
	cb.lightPosition = lightPos;
	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	deviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(5, 1, &m_pConstantBuffer);
	deviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	deviceContext->DrawIndexed(36, 0, 0);
	Shape::reset(m_pVertexBuffer, m_pIndexBuffer, m_pConstantBuffer);

}


Moon::~Moon()
{
	try
	{
		m_pIndexBuffer->Release();
		m_pVertexBuffer->Release();
		m_pConstantBuffer->Release();
		m_pPixelShader->Release();
		m_pVertexShader->Release();
		m_rasterState->Release();
	}
	catch (std::exception &e)
	{
		MessageBox(nullptr, "Error in Moon while shutting down", "Error in Moon", MB_OK);
	}
}
