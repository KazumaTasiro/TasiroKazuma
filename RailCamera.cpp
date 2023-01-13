#include "RailCamera.h"

void RailCamera::Initalize()
{
	worldTransform_ = Object3d::Create();

	
}

void RailCamera::Update()
{
}

XMFLOAT3 RailCamera::MatVector(XMMATRIX matrix4, XMFLOAT3 vector3)
{
	return XMFLOAT3();
}

void RailCamera::Reset()
{
}
