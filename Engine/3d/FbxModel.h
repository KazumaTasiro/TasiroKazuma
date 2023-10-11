#pragma once

#pragma warning(push)
#pragma warning(disable:26813)
#pragma warning(disable:5264)
#include <DirectXTex.h>
#pragma warning(pop)

#include <string>
#include <DirectXMath.h>
#include <vector>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>
#include "Vector3.h"
#include "Matrix4.h"


struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	Vector3 scaling = { 1,1,1 };
	//ローカル回転角
	Vector3 rotation = { 0,0,0 };
	// ローカル移動
	Vector3 translation = { 0,0,0};
	// ローカル変形行列
	Matrix4 transform;
	// グローバル変形行列
	Matrix4 globalTransform;
	// 親ノード
	Node* parent = nullptr;
};

class FbxModel
{
public://定数
	static const int MAX_BONE_INDICES = 4;
public:
	//頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標

		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

	// フレンドクラス
	friend class FbxLoader;

	//ボーン構造体
	struct Bone {
		std::string name;

		DirectX::XMMATRIX invInitialPose;

		FbxCluster* fbxCluster;

		Bone(const std::string& name) {
			this->name = name;
		}

	};


private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	// std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	~FbxModel();
	// バッファ生成
	void CreateBuffers(ID3D12Device* device);
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// モデルの変形行列取得
	const Matrix4& GetModelTransform() { return meshNode->globalTransform; }

	std::vector<Bone>& GetBones() { return bones; }

	FbxScene* GetFbxScene() { return fbxScene; }
private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	// メッシュを持つノード
	Node* meshNode = nullptr;
	// 頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	// ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	// テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	// スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	std::vector<Bone> bones;

	//FBXシーン
	FbxScene* fbxScene = nullptr;
};
