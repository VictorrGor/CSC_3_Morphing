#pragma once
#include <DirectXMath.h>
#include "Formats.h"

using namespace DirectX;

class Camera
{
	fvec3 eye;
	fvec3 lookAt;
	fvec3 up;
	mtx mxProjection;
	mtx mxView; 
	mtx mxVP;//ViewProjectionMatrix
public:
	Camera(fvec3 _eye, fvec3 _lookAt, fvec3 _up, UINT _width, UINT _height) : eye(_eye), lookAt(_lookAt), up(_up)
	{
		mxProjection = XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, _width / (FLOAT)_height, 0.1, 100);

		XMVECTOR vEye, vLookAt, vUp;
		vEye = XMLoadFloat3(&eye);
		vLookAt = XMLoadFloat3(&lookAt);
		vUp = XMLoadFloat3(&up);
		mxView = DirectX::XMMatrixLookToLH(vEye, vLookAt, vUp);

		mxVP = mxProjection * mxView;
	};
	void setPos(fvec3 _eye, fvec3 _lookAt, fvec3 _up)
	{
		eye = _eye;
		lookAt = _lookAt;
		up = _up;

		XMVECTOR vEye, vLookAt, vUp;
		vEye = XMLoadFloat3(&eye);
		vLookAt = XMLoadFloat3(&lookAt);
		vUp = XMLoadFloat3(&up);
		mxView = DirectX::XMMatrixLookToLH(vEye, vLookAt, vUp);

		mxVP = mxProjection * mxView;
	};
	const mtx& getViewProjMx() { return mxVP; };
};