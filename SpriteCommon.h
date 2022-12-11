#pragma once
#include"DirectXCommon.h"
#include <DirectXTex.h>

using namespace DirectX;


//スプライト共通部分
class SpriteCommon {
public:
	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial {
		XMFLOAT4 color; // 色 (RGBA)
	};

public:
	//初期化
	void Initialize(DirectXCommon* dxcommon);

	DirectXCommon* GetDxCommon() { return dxcommon_; }

	ID3D12RootSignature* GetRootSignature() { return rootSignature; }

	ID3D12PipelineState* GetPipelineState() { return pipelineState; }

	ID3D12Resource* GetConstBuffMaterial() { return constBuffMaterial; }

	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap; }

	//SRV用デスクリプタヒープ
	ID3D12DescriptorHeap* srvHeap = nullptr;
private:
	DirectXCommon* dxcommon_ = nullptr;
	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
	HRESULT result;


	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;


	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	ID3D12Resource* constBuffMaterial = nullptr;

	

	//横方向ピクセル数
	const size_t textureWidth = 256;
	//縦方向ピクセル数
	const size_t textureHeight = 256;
	//配列の要素数
	const size_t imageDataCount = textureWidth * textureHeight;
};