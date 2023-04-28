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
	//Microsoft::WRL::を短縮
	template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
	// DirectX::省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static void SetDevice(ID3D12Device* device) { Object3dFbx::device = device; }
	static void SetCamera(Camera* camera) { Object3dFbx::camera = camera; }
public:
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
};
