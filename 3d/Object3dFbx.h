#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>


class Object3dFbx {
protected:
	//Microsoft::WRL::��Z�k
	template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
	// DirectX::�ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static void SetDevice(ID3D12Device* device) { Object3dFbx::device = device; }
	static void SetCamera(Camera* camera) { Object3dFbx::camera = camera; }
public:
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;
};
