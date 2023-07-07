#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Model.h"
#include "Input.h"
#include "PostEffect.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "FbxLoader.h"
#include "Object3dFbx.h"
#include "Camera.h"
#include "Enemy.h"
#include "Player.h"
#include "Skydome.h"


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
	void Initialize(WinApp* winApp, DirectXCommon* dxcomon, Input* input_);

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	///// <summary>
	///// 衝突判定と応答
	///// </summary>
	//void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void UpdateEnemyPopCommands();

	void ExistenceEnemy(const Vector3& EnemyPos);

	////弾リストを取得
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }

	void PhaseReset();

	void EnemyPopComandReset();


private:
	///自キャラ
	Player* player_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemy_;

	Input* input = nullptr;

	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	ImGuiManager* ImGuiMan = nullptr;
	

	Audio* audio = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	Camera* camera = nullptr;

	//OBJからモデルデータを読み込む
	Model* model = nullptr;

	FbxModel* model1 = nullptr;
	Object3dFbx* object1 = nullptr;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	Skydome* skydome = nullptr;
	bool waitflag = false;
	int waitTimer = 0;
};