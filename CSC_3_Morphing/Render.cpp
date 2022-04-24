#include "Render.h"


RenderSys::RenderSys()
{
	g_pd3dDevice = nullptr;
	g_pImmediateContext = nullptr;
	g_pSwapChain = nullptr;
	g_pRenderTargetView = nullptr;
	DepthBuffer = nullptr;
	mCamera = nullptr;
}

RenderSys::~RenderSys()
{
	CleanupDevice();
}

bool RenderSys::Initialize(HWND* hWnd)
{
	bool result;

	HRESULT hRes = S_OK;

	RECT rc;
	GetClientRect(*hWnd, &rc);

	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 120;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = *hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;


	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hRes = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hRes))
			break;
	}
	if (FAILED(hRes))
		return false;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hRes = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hRes))
		return false;

	hRes = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hRes))
		return false;

	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = width;
	depthTextureDesc.Height = height;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* DepthStencilTexture;
	hRes = g_pd3dDevice->CreateTexture2D(&depthTextureDesc, NULL, &DepthStencilTexture);

	if (FAILED(hRes))
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = depthTextureDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hRes = g_pd3dDevice->CreateDepthStencilView(DepthStencilTexture, &dsvDesc, &DepthBuffer);
	DepthStencilTexture->Release();

	if (FAILED(hRes))
		return false;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, DepthBuffer);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	createCamera({ 0,0,0 }, { 1,0,0 }, { 0,1,0 });

	return true;
}

void RenderSys::CleanupDevice()
{
	for (std::vector<Entity*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->Release();
		delete (*it);
	}
	objects.clear();
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = nullptr;
	}
	if (g_pImmediateContext)
	{
		g_pImmediateContext->Release();
		g_pImmediateContext = nullptr;
	}
	if (g_pSwapChain)
	{
		g_pSwapChain->Release();
		g_pSwapChain = nullptr;
	}
	if (g_pRenderTargetView)
	{
		g_pRenderTargetView->Release();
		g_pRenderTargetView = nullptr;
	}
	if (DepthBuffer)
	{
		DepthBuffer->Release();
		DepthBuffer = nullptr;
	}
}

void RenderSys::Render()
{
	const static float ClearColor[4] = { 1.0f, 1.f, 1.f, 1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	for (std::vector<Entity*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->Render(g_pImmediateContext, mCamera->getViewProjMx(), pFrameCB, pModelCB);
	}
	g_pSwapChain->Present(1, 0);
	g_pImmediateContext->ClearDepthStencilView(DepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderSys::createMorphingScene()
{
	AnimatedVertex* arr = new AnimatedVertex[4];
	memset(&arr[0], 0, sizeof(AnimatedVertex) * 4);
	arr[0].Pos = XMFLOAT3(-0.5, 0.5, 0);
	arr[0].Color = XMFLOAT4(1, 0, 0, 1);
	arr[1].Pos = XMFLOAT3(0.5, 0.5, 0);
	arr[1].Color = XMFLOAT4(0, 1, 0, 1);
	arr[2].Pos = XMFLOAT3(-0.5, -0.5, 0);
	arr[2].Color = XMFLOAT4(0, 0, 1, 1);
	arr[3].Pos = XMFLOAT3(0.5, -0.5, 0);
	arr[3].Color = XMFLOAT4(0, 0, 0, 1);

	UINT  indices[] = {0, 1, 2, 3, 2, 1};
	Entity* obj = new Entity();
	//ShaderClass* shader = new ShaderClass();
	//auto res = shader->Initialize(g_pd3dDevice, L"shaders/normalVtx.hlsl", L"shaders/normalPSh.hlsl");
	AnimatedShader* shader = new AnimatedShader();
	auto res = shader->Initialize(g_pd3dDevice, L"shaders/Morphing.hlsl", L"shaders/normalPSh.hlsl");
	if (!res) throw "Cannt create shader class!";
	obj->Initialize(createAnimatedVxBuffer(&arr[0], 4), 4, createIndexBuffer(&indices[0], 6), 6,
								D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, shader);
	
	//Per frame CB
	D3D11_BUFFER_DESC const_desc;
	memset(&const_desc, 0, sizeof(const_desc));
	const_desc.ByteWidth = sizeof(ConstBufferPerFrame);
	const_desc.Usage = D3D11_USAGE_DEFAULT;
	const_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ConstBufferPerFrame cbFrame = {0};
	D3D11_SUBRESOURCE_DATA init_frame;
	memset(&init_frame, 0, sizeof(init_frame));
	init_frame.pSysMem = &cbFrame;
	auto status = g_pd3dDevice->CreateBuffer(&const_desc, &init_frame, &pFrameCB);
	if (status != S_OK) throw "Cannt create frame const buf!";
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &pFrameCB);

	//Per object CB
	memset(&const_desc, 0, sizeof(const_desc));
	const_desc.ByteWidth = sizeof(ConstBufferPerObj);
	const_desc.Usage = D3D11_USAGE_DEFAULT;
	const_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ConstBufferPerObj MVP;
	MVP.MVP = mCamera->getViewProjMx() ;
	D3D11_SUBRESOURCE_DATA init_data;
	memset(&init_data, 0, sizeof(init_data));
	init_data.pSysMem = &MVP;
	status = g_pd3dDevice->CreateBuffer(&const_desc, &init_data, &pModelCB);
	if (status != S_OK) throw "Cannt create model const buf!";
	g_pImmediateContext->VSSetConstantBuffers(1, 1, &pModelCB);

	objects.push_back(obj);
}

void RenderSys::callScene()
{
	AnimatedShader* shader = new AnimatedShader();
	auto res = shader->Initialize(g_pd3dDevice, L"shaders/Morphing.hlsl", L"shaders/normalPSh.hlsl");
	if (!res) throw "Cannt create shader class!";

	Entity* obj = new Entity();
	
	//auto buf = genPyramideScene(1);
	//obj->Initialize(createAnimatedVxBuffer(pVx, 4), 4, createIndexBuffer(&indices[0], 6), 6,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, shader);

}

void RenderSys::createCamera(fvec3 _eye, fvec3 _lookAt, fvec3 _up)
{
	if (mCamera) mCamera->setPos(_eye, _lookAt, _up);
	else mCamera = new Camera(_eye, _lookAt, _up, width, height);
}

ID3D11Buffer* RenderSys::createVertexBuffer(Vertex* _mem, UINT _ptCount)
{
	ID3D11Buffer* res;
	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.ByteWidth = _ptCount * sizeof(Vertex);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA dataStorage;
	memset(&dataStorage, 0, sizeof(dataStorage));
	dataStorage.pSysMem = _mem;
	auto status = g_pd3dDevice->CreateBuffer(&desc, &dataStorage, &res);
	if (status != S_OK) throw "Cannt create vertex buffer!\n";


	return res;
}

ID3D11Buffer* RenderSys::createAnimatedVxBuffer(AnimatedVertex* _mem, UINT _ptCount)
{
	ID3D11Buffer* res;
	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.ByteWidth = _ptCount * sizeof(AnimatedVertex);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA dataStorage;
	memset(&dataStorage, 0, sizeof(dataStorage));
	dataStorage.pSysMem = _mem;
	auto status = g_pd3dDevice->CreateBuffer(&desc, &dataStorage, &res);
	if (status != S_OK) throw "Cannt create vertex buffer!\n";


	return res;
}

ID3D11Buffer* RenderSys::createIndexBuffer(UINT* _mem, UINT _indexCount)
{
	ID3D11Buffer* res;
	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.ByteWidth = _indexCount * sizeof(UINT);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA dataStorage;
	memset(&dataStorage, 0, sizeof(dataStorage));
	dataStorage.pSysMem = _mem;
	auto status = g_pd3dDevice->CreateBuffer(&desc, &dataStorage, &res);
	if (status != S_OK) throw "Cannt create index buffer!\n";

	return res;
}
