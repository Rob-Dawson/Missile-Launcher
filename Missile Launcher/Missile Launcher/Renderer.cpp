#include "Renderer.h"
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <Keyboard.h>
#include "Camera.h"
#include "Cube.h"
#include "AntTweakBar.h"

Renderer::Renderer()
	:	m_pDevice{ nullptr }, m_pSwapChain{nullptr},
	m_pDeviceContext{ nullptr }, m_pDepthStencil{ nullptr }, m_pDepthStencilView{ nullptr },
	m_pRenderTargetView{ nullptr }, m_DriverType{ D3D_DRIVER_TYPE_NULL }, m_FeatureLevel{ D3D_FEATURE_LEVEL_11_0 }, m_window{nullptr}
{
	m_window = nullptr;
}


//
//Renderer::Renderer(const Renderer &renderer)
//	: m_pDevice{ nullptr }, m_pSwapChain{ nullptr },
//	m_pDeviceContext{ nullptr }, m_pRenderTargetView{ nullptr },
//	m_DriverType{ D3D_DRIVER_TYPE_NULL }, m_FeatureLevel{ D3D_FEATURE_LEVEL_11_0 }
//{
//
//}
//
//Renderer::Renderer(Renderer const &&renderer) noexcept
//{}

bool Renderer::InitDevice(Window const &window)
{

	//Create Swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = window.getWidth();
	swapChainDesc.BufferDesc.Height = window.getHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window.getHandle();
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	D3D_DRIVER_TYPE driverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	//std::vector<D3D_FEATURE_LEVEL> featureLevel =
	D3D_FEATURE_LEVEL featureLevel[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	auto const numFeatureLevels = ARRAYSIZE(featureLevel);

	auto result = 0;
	for (const auto driver : driverType)
	{
		result = D3D11CreateDeviceAndSwapChain(
			nullptr, driver, nullptr, 0,
			featureLevel, numFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc,
			&m_pSwapChain, &m_pDevice, &m_FeatureLevel, &m_pDeviceContext);

		if (SUCCEEDED(result))
		{
			m_DriverType = driver;
			break;
		}
	}
	if (FAILED(result))
	{
		MessageBox(nullptr, "Failed to create device and or swap chain", "Error", MB_OK);
		return result;
	}
	//m_keyboard1 = std::make_unique<DirectX::Keyboard>();
	m_window = window.getHandle();
	return 0;
}

bool Renderer::CreateRenderTarget(Window const &window)
{
	auto hr = 0;
	ID3D11Texture2D *m_pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBuffer));
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Failed to Create Back buffer", "Error", MB_OK);
		return hr;
	}
	
	hr = m_pDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, &m_pRenderTargetView);
	m_pBackBuffer->GetDesc(&backBufferDesc);
	m_pBackBuffer->Release();
	
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Failed to Create Render Target", "Error", MB_OK);
		return hr;
	}

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = window.getWidth();
	descDepth.Height = window.getHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	// Initialize the view matrix



	return 0;
}

bool Renderer::CompileShaderFromFile(const WCHAR* const szFileName, const LPCSTR const szEntryPoint, const LPCSTR const szShaderModel, ID3DBlob** const ppBlobOut) const
{
	auto hr = 0;

	const DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return 0;
}


void Renderer::BeginFrame() 
{
	//Bind Render Target
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	//Set Viewport

	/*Put inside Camera Class*/
	D3D11_VIEWPORT vp;
	vp.Width = 800.0f;
	vp.Height = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;



	/*=======================*/
	//DirectX::Keyboard a;

	//DirectX::Keyboard a;
	//auto keyboard = a.GetState();
	//if (keyboard.A)
	//{

	//}
	

	//else if(kb.A == true)
	//{
	//	Camera c;
	//	c.setPosition(2.0f, 2.0f, 2.0f);
	//}
	
	m_pDeviceContext->RSSetViewports(1, &vp);
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::MidnightBlue);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	

	//m_shape = GeometricPrimitive::CreateSphere(m_pDeviceContext);

}

void Renderer::EndFrame() const
{
	m_pSwapChain->Present(0, 0);
}

//void Renderer::Update()
//{
//
//	auto kb = m_keyboard1->GetState();
//	if (kb.Escape == true)
//	{
//		PostQuitMessage(0);
//	}
////	
//	if (kb.A)
//	{
//		m_pCamera.AdjustPosition(m_pCamera.GetForward() * cameraSpeed);
//	}
//}

void Renderer::ShutDown()
{
	if (m_pDevice) m_pDevice->Release();
	if (m_pDeviceContext) m_pDeviceContext->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
}

//ID3D11Device *Renderer::GetDevice() const
//{
//	return m_pDevice;
//}

//ID3D11DeviceContext *Renderer::GetDeviceContext() const
//{
//	return m_pDeviceContext;
//}


Renderer::~Renderer()
{
	try
	{
		ShutDown();
	}
	catch (std::exception &e)
	{
		MessageBox(nullptr, "Error shutting down", "Error", MB_OK);
	}
}
