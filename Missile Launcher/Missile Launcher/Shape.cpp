#include "Shape.h"
#include <vector>


Shape::Shape()
	: m_pVertexShader{ nullptr }, m_pPixelShader{ nullptr },
	  m_pInputLayout{ nullptr }, m_pVertexBuffer{ nullptr },
	  m_pIndexBuffer{ nullptr }, m_pConstantBuffer{ nullptr }
	  
{
}

void Shape::reset(ID3D11Buffer *const vertexBuffer, ID3D11Buffer *const indexBuffer, ID3D11Buffer *const constantBuffer)const
{
	vertexBuffer->Release();
	indexBuffer->Release();
	constantBuffer->Release();
}

//ID3D11VertexShader *Shape::createVertexShader(Renderer& renderer, const WCHAR *fileName, D3D11_INPUT_ELEMENT_DESC layout[], const int &layoutSize, ID3D11VertexShader *v)
//{
//	ID3DBlob* pVSBlob = nullptr;
//	auto hr = true;
//	hr = renderer.CompileShaderFromFile(fileName, "VS_main", "vs_4_0", &pVSBlob);
//
//	// Create the vertex shader
//	renderer.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &v);
//	if (FAILED(hr))
//	{
//		pVSBlob->Release();
//		MessageBox(nullptr, "The vertex shader connot be compiled", "Error", MB_OK);
//		//return hr;
//	}
//
//
//	D3D11_INPUT_ELEMENT_DESC inputLayout[5];
//	for (int i = 0; i < layoutSize; ++i)
//	{
//		inputLayout[i] = layout[i];
//	}
//
//	//// Create the input layout
//	hr = renderer.GetDevice()->CreateInputLayout(inputLayout, layoutSize, pVSBlob->GetBufferPointer(),
//		pVSBlob->GetBufferSize(), &m_pInputLayout);
//
//	
//	pVSBlob->Release();
//	pVSBlob = nullptr;
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr, "The input layout was not created correctly", "Error", MB_OK);
//	}
//	renderer.GetDeviceContext()->IASetInputLayout(m_pInputLayout);
//	return v;
//}
//
//ID3D11PixelShader *Shape::createPixelShader(Renderer& renderer, const WCHAR *fileName, ID3D11PixelShader *p)
//{
//	ID3DBlob* pPSBlob = nullptr;
//	auto hr = true;
//	hr = renderer.CompileShaderFromFile(fileName, "PS_main", "ps_4_0", &pPSBlob);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr, "The pixel shader connot be compiled", "Error", MB_OK);
//
//		//return hr;
//	}
//
//	// Create the pixel shader
//	hr = renderer.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &p);
//	pPSBlob->Release();
//	if (FAILED(hr))
//		return 0;
//
//	return p;
//}
//
//ID3D11InputLayout &Shape::createInputLayout(Renderer& renderer, D3D11_INPUT_ELEMENT_DESC layout[], const int layoutSize)
//{
//	ID3DBlob* pVSBlob = nullptr;
//	auto hr = true;
//	D3D11_INPUT_ELEMENT_DESC inputLayout[5];
//	for (int i = 0; i < layoutSize; ++i)
//	{
//		inputLayout[i] = layout[i];
//	}
//
//	//// Create the input layout
//	hr = renderer.GetDevice()->CreateInputLayout(inputLayout, layoutSize, pVSBlob->GetBufferPointer(),
//		pVSBlob->GetBufferSize(), &m_pInputLayout);
//
//	pVSBlob->Release();
//	pVSBlob = nullptr;
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr, "The input layout was not created correctly", "Error", MB_OK);
//	}
//	
//}

bool Shape::createShaders( const Renderer& const renderer, D3D11_INPUT_ELEMENT_DESC layout[], const int layoutSize, const WCHAR *const fileName/*, ID3D11VertexShader *v, ID3D11PixelShader *p*/) 
{
	auto const device = renderer.GetDevice();

	bool hr = true;

	ID3DBlob* pVSBlob = nullptr;
	hr = renderer.CompileShaderFromFile(fileName, "VS_main", "vs_4_0", &pVSBlob);

	hr = device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		MessageBox(nullptr, "The vertex shader connot be compiled", "Error", MB_OK);
		//return hr;
	}
	
	D3D11_INPUT_ELEMENT_DESC inputLayout[5];
	for(int i = 0; i < layoutSize; ++i)
	{
		inputLayout[i] = layout[i];
	}

	//// Create the input layout
	hr = renderer.GetDevice()->CreateInputLayout(inputLayout, layoutSize, pVSBlob->GetBufferPointer(),
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
	hr = renderer.CompileShaderFromFile(fileName, "PS_main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "The pixel shader connot be compiled", "Error", MB_OK);

		//return hr;
	}

	// Create the pixel shader
	hr = renderer.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	pPSBlob->Release();
	// Set the input layout
	return 0;
}


Shape::~Shape()
{
}
