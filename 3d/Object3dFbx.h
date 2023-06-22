#pragma once

#include "FbxModel.h"
#include "FbxLoader.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>


class Object3dFbx {
protected:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス
	// 定数バッファ用データ構造体（座標変換行列用）
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
	};

public: // 定数
	//ボーンの最大数
	static const int MAX_BONES = 32;

	//定数バッファ用のデータ構造体
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FbxModel* model) { this->model = model; }

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();
public:
	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	static void SetDevice(ID3D12Device* device) { Object3dFbx::device = device; }
	static void SetCamera(Camera* camera) { Object3dFbx::camera = camera; }
public:
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

protected: // メンバ変数
	// ローカルスケール
	XMFLOAT3 scale = { 0.1f,0.1f,0.1f };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// モデル
	FbxModel* model = nullptr;

	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;

	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション)
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
};
