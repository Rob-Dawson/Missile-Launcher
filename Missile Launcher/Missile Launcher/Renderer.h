#pragma once
#include "Window.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <Keyboard.h>
#include "Camera.h"
#include <GeometricPrimitive.h>

class Camera;

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer &renderer) = delete;
	//Renderer(Renderer const &&renderer) noexcept;

	Renderer &operator=(const Renderer &source);

	bool InitDevice(Window const &window);
	bool CreateRenderTarget(Window const &window);
	bool CompileShaderFromFile(const WCHAR* const szFileName, const LPCSTR const szEntryPoint, const LPCSTR const szShaderModel,  ID3DBlob** const ppBlobOut) const;
	void BeginFrame() ;
	void EndFrame() const;
	void Update();
	void ShutDown();
	
	inline ID3D11Device		*GetDevice() const
	{
		return m_pDevice;
	}

	inline ID3D11DeviceContext *GetDeviceContext() const
	{
		return m_pDeviceContext;
	}

	~Renderer();

private:

	ID3D11Device		*m_pDevice;
	IDXGISwapChain		*m_pSwapChain;
	ID3D11DeviceContext	*m_pDeviceContext;

	ID3D11Texture2D		*m_pDepthStencil;
	ID3D11DepthStencilView *m_pDepthStencilView;
	ID3D11RenderTargetView *m_pRenderTargetView;

	D3D_DRIVER_TYPE m_DriverType;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	D3D11_TEXTURE2D_DESC backBufferDesc;
	HWND                                            m_window;
};

