#include "Cube.h"
#include "DDSTextureLoader.h"

struct CubeVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


struct ConstantBufferCube
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

Cube::Cube()
	: m_pIndexBuffer{ nullptr }, m_pVertexBuffer{ nullptr }, m_pConstantBuffer{ nullptr },
	m_pPixelShader{ nullptr }, m_pVertexShader{ nullptr }, m_pInputLayout{ nullptr }
{
}

bool Cube::createMeshCube(Renderer &renderer, const D3D11_FILL_MODE *const wireFrame)
{
	bool hr{ 0 };

	auto device = renderer.GetDevice();
	auto deviceContext = renderer.GetDeviceContext();
	//Create Vertex buffer
	CubeVertex vertices[]
	{
		//Top
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0) },

		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

		//Bottom
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},


		//Left
	  { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},


	  //Right
	  { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	  { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},


	  //Front
	 { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},


	 //Back
	 { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},

	 { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0) },

	 { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0)},
	};


	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(CubeVertex) * ARRAYSIZE(vertices);
	vertexDesc.CPUAccessFlags = 0;
	vertexDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = vertices;

	hr = device->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
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
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
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
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferCube);
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
		// Set vertex buffer
	const UINT stride = sizeof(CubeVertex);
	const UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set Index Buffer
	renderer.GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//Set Topology
	renderer.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_WorldMatrix = DirectX::XMMatrixIdentity();

	ID3D11RasterizerState* m_rasterState = nullptr;
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

bool Cube::createShadersCube(Renderer& renderer)
{
	auto hr = true;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT layoutSize = ARRAYSIZE(layout);

	Shape::createShaders(renderer, layout, layoutSize, L"Triangle.fx");

	m_pVertexShader = Shape::getVertexShader();
	m_pPixelShader = Shape::getPixelShader();

	

	return 0;
}

void Cube::draw(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX *viewMatrix, DirectX::XMMATRIX *projectionMatrix, float time)
{
	m_WorldMatrix = DirectX::XMMatrixIdentity();
	m_WorldMatrix *= DirectX::XMMatrixRotationY(time);
	m_WorldMatrix *= DirectX::XMMatrixTranslation(0.0f, 3.0f, 0.0f);


	ConstantBufferCube cb{};
	cb.mWorld = XMMatrixTranspose(m_WorldMatrix);
	cb.mView = XMMatrixTranspose(*viewMatrix);
	cb.mProjection = XMMatrixTranspose(*projectionMatrix);
	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	deviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);
	deviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	deviceContext->DrawIndexed(36, 0, 0);
	Shape::reset(m_pVertexBuffer, m_pIndexBuffer, m_pConstantBuffer);
}


Cube::~Cube()
{
}
