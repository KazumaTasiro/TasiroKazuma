#pragma once
#include"SpriteCommon.h"
#include <DirectXMath.h>

using namespace DirectX;

//スプライト
class Sprite {
public:
	//初期化
	void Initialize(SpriteCommon* spritecommon_);

	void Draw();
private:
	SpriteCommon* spritecomon;
	HRESULT result;
	// 頂点データ
	XMFLOAT3 vertices[3] = {
		{ -0.5f, -0.5f, 0.0f },	// 左下
		{-0.5f, +0.5f, 0.0f },	// 左上
		{+0.5f, -0.5f, 0.0f },	// 右下
	};
	//XMFLOAT3 vertices[] = {
	//{ -0.5f, -0.5f, 0.0f }, // 左下
	//{ -0.5f, +0.5f, 0.0f }, // 左上
	//{ +0.5f, -0.5f, 0.0f }, // 右下
	//};
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

};