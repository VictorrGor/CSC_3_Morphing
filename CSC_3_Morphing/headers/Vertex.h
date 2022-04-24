#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct AnimatedVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 targetPos;
	XMFLOAT4 Color;
};


struct TexturedVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 TextureCoord;
};

