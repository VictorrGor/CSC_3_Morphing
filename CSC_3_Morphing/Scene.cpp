#include "Scene.h"


void calcNewTriangle(const AnimatedVertex& _up, const AnimatedVertex& _left, const AnimatedVertex& _right,
	AnimatedVertex& _downW, AnimatedVertex& _leftW, AnimatedVertex& _rightW)
{
	XMVECTOR xmLeft, xmRight, xmUp, xmRes;
	xmUp = XMLoadFloat3(&_up.Pos);
	xmLeft = XMLoadFloat3(&_left.Pos);
	xmRight = XMLoadFloat3(&_right.Pos);

	xmRes = (xmLeft + xmRight) / 2;
	XMStoreFloat3(&_downW.Pos, xmRes);
	xmRes = (xmLeft + xmUp) / 2;
	XMStoreFloat3(&_leftW.Pos, xmRes);
	xmRes = (xmRight + xmUp) / 2;
	XMStoreFloat3(&_rightW.Pos, xmRes);
}

/*
std::pair<AnimatedVertex*, Indexes> genPyramideScene(UINT tesselation_lvl)
{
	//Vertex count per triangle by each tesselation level
	//Tess_lvl = tl E [0, n];
	// vertex_lvl = 2 * (tl + 1) - 1;
	// vertex_count = 0
	// vertex_count += i for i in range(1, vertex_lvl + 1) => sum = (1 + vertex_lvl) * vertex_lvl / 2

	UINT vertex_lvl = (tesselation_lvl) ? 2 * (tesselation_lvl + 1) - 1 : 2;
	UINT vertex_count = (1 + vertex_lvl) * vertex_lvl / 2;

	AnimatedVertex* pVtx = new AnimatedVertex[vertex_count];
	memset(&pVtx[0], 0, sizeof(AnimatedVertex) * vertex_count);
	pVtx[0].Pos = { 0, 1, 0 };

	pVtx[vertex_count - vertex_lvl].Pos = { -1, 0, 0 };
	pVtx[vertex_count - 1].Pos = { 1, 0, 0 };

	UINT upPt = 0, leftPt = vertex_count - vertex_lvl, rightPt = vertex_count - 1;
	UINT upPtLvl = 1, leftPtLvl = vertex_lvl, rightPtLvl = vertex_lvl;

	for (UINT iDivider = 1; iDivider <= tesselation_lvl; ++iDivider)
	{

	}
	UINT triangle_count = pow(4, tesselation_lvl);
	UINT indices_size = triangle_count * 3;
	UINT* indices = new UINT[indices_size];
	UINT index_ctr = 0;
	////
	///@todo if vertex_lvl == 1
	////
	for (UINT iLvl = 1; iLvl < vertex_lvl; ++iLvl)
	{
		UINT offset = iLvl * (iLvl - 1) / 2;
		UINT upperBound = iLvl + offset;


		indices[index_ctr] = offset;
		++index_ctr;
		indices[index_ctr] = offset + iLvl + 1;
		++index_ctr;
		indices[index_ctr] = offset + iLvl;
		++index_ctr;
		for (UINT upIndex = offset + 1; upIndex < upperBound; ++upIndex)
		{
			indices[index_ctr] = upIndex - 1;
			++index_ctr;
			indices[index_ctr] = upIndex;
			++index_ctr;
			indices[index_ctr] = upIndex + iLvl;
			++index_ctr;

			indices[index_ctr] = upIndex;
			++index_ctr;
			indices[index_ctr] = upIndex + iLvl + 1;
			++index_ctr;
			indices[index_ctr] = upIndex + iLvl;
			++index_ctr;
		}
	}
	return std::make_pair(pVtx, static_cast<Indexes>(std::make_pair(indices, indices_size)) );
}
*/