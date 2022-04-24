#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wchar.h>
#include <vector>
#include <winerror.h>
#include <DirectXMath.h>

#include "Formats.h"
#include "Camera.h"
#include "Scene.h"

#include "headers\Entity.h"
using namespace DirectX;


class Entity;


struct ConstBufferPerObj
{
	XMMATRIX MVP;
};

struct ConstBufferPerFrame
{
	float time;
	float dummy[3];
};

class RenderSys
{
	D3D_DRIVER_TYPE         g_driverType;
	D3D_FEATURE_LEVEL       g_featureLevel;
	ID3D11Device*			g_pd3dDevice;
	ID3D11DeviceContext*	g_pImmediateContext;
	IDXGISwapChain*			g_pSwapChain;
	ID3D11RenderTargetView* g_pRenderTargetView;
	ID3D11DepthStencilView* DepthBuffer;

	ID3D11Buffer* pModelCB; //perObj
	ID3D11Buffer* pFrameCB; //perFrame

	std::vector<Entity*> objects;
	Camera* mCamera;
	UINT width;
	UINT height;
	///@todo Window resizing must recalc width and height
public:
	RenderSys();
	~RenderSys();
	bool Initialize(HWND* hWnd);
	void CleanupDevice();
	void Render();
	void createMorphingScene();
	void callScene();

	ID3D11Buffer* createVertexBuffer(Vertex* _mem, UINT _ptCount);
	ID3D11Buffer* createIndexBuffer(UINT* _mem, UINT _indexCount);
	ID3D11Buffer* createAnimatedVxBuffer(AnimatedVertex* _mem, UINT _ptCount);

	void createCamera(fvec3 eye, fvec3 lookAt, fvec3 up);
};