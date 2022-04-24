#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>


class ShaderClass
{
protected:
	ID3D11VertexShader* pVxSh;
	ID3D11PixelShader* pPxSh;
	ID3D11InputLayout* pVtxLayout;
public:
	ShaderClass();
	~ShaderClass();
	bool Initialize(ID3D11Device* _pDevice, LPCWSTR _vsName, LPCWSTR _psName);
	void Render(ID3D11DeviceContext* _pImmidiateContext, UINT _vertexCount);
	void SetShader(ID3D11DeviceContext* _pImmidiateContext);
};

class AnimatedShader : public ShaderClass
{
public:
	bool Initialize(ID3D11Device* _pDevice, LPCWSTR _vsName, LPCWSTR _psName);
};