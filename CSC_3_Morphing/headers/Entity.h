#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "Vertex.h",
#include "Shader.h"
#include "..\Render.h"

using namespace DirectX;

class RenderSys;

class Entity
{
	ID3D11Buffer* pVertexBuffer, * pIndexBuffer;
	UINT uVertexCount, uIndexCount;
	D3D_PRIMITIVE_TOPOLOGY topology;
	ShaderClass* pShader;
	///@TODO Model matrix;
	mtx mxModel;
public:
	Entity();
	void Initialize(ID3D11Buffer* _pVertexBuffer, UINT _uVertexCount, ID3D11Buffer* _pIndexBuffer, UINT _uIndexCount,
		D3D_PRIMITIVE_TOPOLOGY _topology, ShaderClass* _pShader);
	void Release();
	~Entity();

	void Render(ID3D11DeviceContext* _pImmediateContext, const mtx& PV, ID3D11Buffer* _pFrameCB, ID3D11Buffer* _pModelCB);
	void SetTopology(D3D_PRIMITIVE_TOPOLOGY _topology);
};