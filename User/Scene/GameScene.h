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
#include "Player.h"
#include "Skydome.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "Road.h"
#include "ParticleManager.h"
#include "Title.h"
#include "SeenTransition.h"

///<summary>
///ボスの生成と行動
///</summary>
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
	void Initialize(WinApp* winApp, DirectXCommon* dxcomon, Input* input);

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




	////弾リストを取得
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }

	///<summary>
	////シーンをまたぐときのリセット
	///</summary>
	void PhaseReset();
	///<summary>
	////タイトルに戻るときのリセット
	///</summary>
	void TitleReset();




private:
	enum Scene
	{
		Title,
		Game,
		Boss,
		GameOver,
		GameClear
	};
	Scene scene;
private:
	///自キャラ
	Player* player_ = nullptr;

	//std::list<std::unique_ptr<Enemy>> enemy_;

	Input* input_ = nullptr;

	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	ImGuiManager* ImGuiMan_ = nullptr;


	Audio* audio_ = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;

	Camera* camera_ = nullptr;

	//OBJからモデルデータを読み込む
	Model* model_ = nullptr;

	FbxModel* model1 = nullptr;
	Object3dFbx* object1 = nullptr;

	Road* road_ = nullptr;

	Skydome* skydome_ = nullptr;

	EnemyManager* enemyManager_ = nullptr;


	Sprite* stert_ = nullptr;
	Sprite* gameClear_ = nullptr;
	Sprite* gameOver_ = nullptr;
	Vector2 spriteEnd_;

	bool playMove = true;

	float playPos = 0;

	bool TitleEnd = false;

	ParticleManager* ParticleMana_;



	Vector3 cameraTitle = { 0,0,10 };
	Vector3 cameraGame = { 0,0,-10 };

	Titles* title_ = nullptr;

	SeenTransition* seenTransition_ = nullptr;
	bool seenFlag = false;
};