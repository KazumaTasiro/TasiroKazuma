#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "PostEffect.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include"ParticleManager.h"
#include"ParticleManager2.h"

class GameScene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp,DirectXCommon* dxcomon, Input* input_);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

	void EffectAdd();

private:
	Input* input = nullptr;

	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	ImGuiManager* ImGuiMan = nullptr;
	
	Camera* camera = nullptr;
	Transform camWtf;

	Audio* audio = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	Sprite* sprite = nullptr;
	Sprite* sprite2 = nullptr;

	Object3d* obj = nullptr;
	Object3d* obj2 = nullptr;

	//OBJからモデルデータを読み込む
	Model* model = nullptr;
	Model* model2 = nullptr;

	//パーティクルクラスの初期化 
	ParticleManager* particleManager = nullptr;
	ParticleManager2* particleManager2 = nullptr;
};