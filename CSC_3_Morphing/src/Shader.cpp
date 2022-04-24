#include "..\headers\Shader.h"

ShaderClass::ShaderClass()
{
	pVxSh = nullptr;
	pPxSh = nullptr;
	pVtxLayout = nullptr;
}

ShaderClass::~ShaderClass()
{
	if (pVxSh) pVxSh->Release();
	if (pPxSh) pPxSh->Release();
	if (pVtxLayout) pVtxLayout->Release();
}

bool ShaderClass::Initialize(ID3D11Device* _pDevice, LPCWSTR _vsName, LPCWSTR _psName)
{
	HRESULT hRes = S_OK;
	ID3DBlob* pShaderBuffer = nullptr;
	ID3DBlob* pErrorBuffer = nullptr;
	hRes = D3DCompileFromFile(_vsName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", NULL, NULL, &pShaderBuffer, &pErrorBuffer);
	if (pErrorBuffer)
	{
		pErrorBuffer->Release();
		pErrorBuffer = nullptr;
	}
	if (FAILED(hRes))
	{
		if (pShaderBuffer) pShaderBuffer->Release();
		return false;
	}
	hRes = _pDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), NULL, &this->pVxSh);
	if (FAILED(hRes))
	{
		if (pShaderBuffer) pShaderBuffer->Release();
		return false;
	}


	D3D11_INPUT_ELEMENT_DESC elemDesc[] = { {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
											{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}};
	UINT numElements = ARRAYSIZE(elemDesc);
	hRes = _pDevice->CreateInputLayout(elemDesc, numElements, pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), &this->pVtxLayout);
	
	if (pShaderBuffer) pShaderBuffer->Release();
	if (FAILED(hRes))
	{
		return false;
	}
	
	hRes = D3DCompileFromFile(_psName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", NULL, NULL, &pShaderBuffer, &pErrorBuffer);
	if (pErrorBuffer)
	{
		pErrorBuffer->Release();
		pErrorBuffer = nullptr;
	}
	hRes = _pDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), NULL, &this->pPxSh);
	if (pShaderBuffer) pShaderBuffer->Release();
	if (FAILED(hRes))
	{
		return false;
	}

	return true;
}

void ShaderClass::Render(ID3D11DeviceContext* _pImmidiateContext, UINT _vertexCount)
{
	_pImmidiateContext->VSSetShader(pVxSh, NULL, 0);
	_pImmidiateContext->PSSetShader(pPxSh, NULL, 0);
	_pImmidiateContext->IASetInputLayout(pVtxLayout);
	_pImmidiateContext->Draw(_vertexCount, 0);
}

void ShaderClass::SetShader(ID3D11DeviceContext* _pImmidiateContext)
{
	_pImmidiateContext->VSSetShader(pVxSh, NULL, 0);
	_pImmidiateContext->PSSetShader(pPxSh, NULL, 0);
	_pImmidiateContext->IASetInputLayout(pVtxLayout);
}

bool AnimatedShader::Initialize(ID3D11Device* _pDevice, LPCWSTR _vsName, LPCWSTR _psName)
{
	HRESULT hRes = S_OK;
	ID3DBlob* pShaderBuffer = nullptr, *pErrorBuffer = nullptr;

	//VS
	hRes = D3DCompileFromFile(_vsName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", NULL, NULL, &pShaderBuffer, &pErrorBuffer);
	if (pErrorBuffer)
	{
		pErrorBuffer->Release();
		pErrorBuffer = nullptr;
	}
	if (FAILED(hRes))
	{
		if (pShaderBuffer) pShaderBuffer->Release();
		return false;
	}
	hRes = _pDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), NULL, &pVxSh);
	if (FAILED(hRes))
	{
		if (pShaderBuffer) pShaderBuffer->Release();
		return false;
	}
	//InputLayout
	D3D11_INPUT_ELEMENT_DESC elemDesc[] = { {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TARGET_POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} };
	UINT numElements = ARRAYSIZE(elemDesc);
	hRes = _pDevice->CreateInputLayout(elemDesc, numElements, pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), &pVtxLayout);
	if (pShaderBuffer)
	{
		pShaderBuffer->Release();
		pShaderBuffer = nullptr;
	}
	if (FAILED(hRes))
	{
		return false;
	}
	//PS
	hRes = D3DCompileFromFile(_psName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", NULL, NULL, &pShaderBuffer, &pErrorBuffer);
	if (pErrorBuffer)
	{
		pErrorBuffer->Release();
		pErrorBuffer = nullptr;
	}
	if (FAILED(hRes))
	{
		return false;
	}
	hRes = _pDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), NULL, &pPxSh);
	if (pShaderBuffer)
	{
		pShaderBuffer->Release();
		pShaderBuffer = nullptr;
	}
	if (FAILED(hRes))
	{
		return false;
	}
	return true;
}
