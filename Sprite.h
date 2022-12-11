#pragma once
#include"SpriteCommon.h"
#include <DirectXMath.h>

using namespace DirectX;


// 頂点データ構造体
struct Vertex
{
	XMFLOAT3 pos; // xyz座標
	XMFLOAT2 uv;  // uv座標
};
//スプライト
class Sprite {
public:

public:
	//初期化
	void Initialize(SpriteCommon* spritecommon_);

	void Draw();
private:
	SpriteCommon* spritecomon;
	HRESULT result;
	// 頂点データ
	Vertex vertices[4] = {
		// x      y     z       u     v
		{{-0.4f, -0.7f, 0.0f}, {0.0f, 1.0f}}, // 左下
		{{-0.4f, +0.7f, 0.0f}, {0.0f, 0.0f}}, // 左上
		{{+0.4f, -0.7f, 0.0f}, {1.0f, 1.0f}}, // 右下
		{{+0.4f, +0.7f, 0.0f}, {1.0f, 0.0f}}, // 右上
	};
	//XMFLOAT3 vertices[] = {
	//{ -0.5f, -0.5f, 0.0f }, // 左下
	//{ -0.5f, +0.5f, 0.0f }, // 左上
	//{ +0.5f, -0.5f, 0.0f }, // 右下
	//};
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

};