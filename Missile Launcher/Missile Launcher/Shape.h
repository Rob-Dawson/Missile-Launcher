#pragma once
#include "Renderer.h"
/* This will be the base class which will allow classes such as cube and cylinder to inherit from
 * It may contain instances of light and texture classes which can be used in the cube and cylinder
 * The cube class may instead contain an instance of light.
 * Need to ask which might be best
 * 
 * 
 */
class Shape
{
public:
	Shape();
	Shape(const Shape &source) = delete;
	Shape& operator = (const Shape &t) = delete;
	bool createMesh(Renderer &renderer);
	bool createShaders( const Renderer & const renderer, D3D11_INPUT_ELEMENT_DESC layout[], const int layoutSize, const WCHAR * const fileName);

	inline ID3D11VertexShader *getVertexShader()const
	{
		return m_pVertexShader;
	}
	inline ID3D11PixelShader *getPixelShader()const
	{
		return m_pPixelShader;
	}


	void reset(ID3D11Buffer *const vertexBuffer, ID3D11Buffer *const indexBuffer, ID3D11Buffer *const constantBuffer)const;
	virtual ~Shape();

private:
	ID3D11VertexShader *m_pVertexShader;
	ID3D11PixelShader *m_pPixelShader;

	ID3D11InputLayout *m_pInputLayout;
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;
	ID3D11Buffer *m_pConstantBuffer;
};

