#pragma once
#include"DirectXCommon.h"




//スプライト共通部分
class SpriteCommon {
public:
	//初期化
	void Initialize(DirectXCommon* dxcommon);

	DirectXCommon* GetDxCommon() { return dxcommon_; }

	ID3D12RootSignature* GetRootSignature() { return rootSignature; }

	ID3D12PipelineState* GetPipelineState() { return pipelineState; }
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
};