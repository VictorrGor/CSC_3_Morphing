
#include "..\headers\Entity.h"

Entity::Entity()
{
	pVertexBuffer = nullptr;
	pIndexBuffer = nullptr;
	pShader = nullptr;
}

void Entity::Initialize(ID3D11Buffer* _pVertexBuffer, UINT _uVertexCount, ID3D11Buffer* _pIndexBuffer, UINT _uIndexCount,
								D3D_PRIMITIVE_TOPOLOGY _topology, ShaderClass* _pShader)
{
	uVertexCount = _uVertexCount;
	uIndexCount = _uIndexCount;
	mxModel = XMMatrixIdentity();
	pVertexBuffer = _pVertexBuffer;
	pIndexBuffer = _pIndexBuffer;
	topology = _topology;
	pShader = _pShader;
}

void Entity::Release()
{
	if (pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}
	if (pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer = nullptr;
	}
}

Entity::~Entity()
{
	Release();
}

void Entity::Render(ID3D11DeviceContext* _pImmediateContext, const mtx& PV, ID3D11Buffer* _pFrameCB, ID3D11Buffer* _pModelCB)
{
	static const UINT stride = sizeof(AnimatedVertex);
	static const UINT offset = 0;
	pShader->SetShader(_pImmediateContext);
	_pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_pImmediateContext->IASetPrimitiveTopology(topology);
	_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	DWORD actualTime = GetTickCount();
	static ConstBufferPerFrame cbFrame = { 0, {0,0,0} };
	cbFrame.time = (float)(sin(actualTime / 1000.0f) + 1) / 2;
	_pImmediateContext->UpdateSubresource(_pFrameCB, 0, NULL, &cbFrame, 0, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pFrameCB);

	float sc = (sin(actualTime / 1000.0f) + 1) / 2;
	//mxModel = XMMatrixScaling(sc, sc, sc);
	static ConstBufferPerObj cbModel;
	cbModel.MVP = mxModel;
	_pImmediateContext->UpdateSubresource(_pModelCB, 0, NULL, &cbModel, 0, 0);
	_pImmediateContext->VSSetConstantBuffers(1, 1, &_pModelCB);

	if (pIndexBuffer) _pImmediateContext->DrawIndexed(uIndexCount, 0, 0);
	else _pImmediateContext->Draw(uVertexCount, 0);
}

void Entity::SetTopology(D3D_PRIMITIVE_TOPOLOGY _topology)
{
	topology = _topology;
}
