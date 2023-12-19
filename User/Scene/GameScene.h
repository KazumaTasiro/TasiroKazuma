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
#include "StertCount.h"
#include "GameOver.h"
#include "GameClear.h"
#include "RailCamera.h"
#include "LightGroup.h"

///<summary>
///ボスの生成と行動
///</summary>
class GameScene
{
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
	void Initialize(DirectXCommon* dxcomon);

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

	//カーソルが画面外に出ない処理
	void CursorLimit();

	void LightUpdate();

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

	RailCamera* railCamera = nullptr;
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

	RECT rcClip;
	RECT rcOldClip;

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
	Sprite* operation_ = nullptr;
	Sprite* cameraMoveOps_ = nullptr;

	Vector2 spriteEnd_;
	Vector2 operationPos = {128,64};

	bool playMove = true;

	float playPos = 0;

	bool TitleEnd = false;

	ParticleManager* ParticleMana_;

	GameOverSeen* gameOverSeen = nullptr;
	GameClearScene* gameClearScene = nullptr;


	Vector3 cameraTitle = { 0,0,10 };
	Vector3 cameraGame = { 0,0,-10 };

	Titles* title_ = nullptr;

	SeenTransition* seenTransition_ = nullptr;
	bool seenFlag = false;

	StertCount* stertCount_ = nullptr;

	Vector3 TargetCamRes = { 0,0,0 };
	enum Nmb
	{
		zero = 0,
		one = 1,
		two = 2,
		three = 3,
		four = 4,
		five = 5,
		six = 6,
		seven = 7,
		eight = 8,
		nine = 9,
		ten = 10,
	};

	bool DemoClear = false;
	float gameOverUp = 80.0f;
	float gamestertUp = 50.0f;

	float bossTime = 10.0f;

	LightGroup* lightGroup = nullptr;

	Vector4  circleShadowDir = { 0,-1,0,0 };
	Vector3  circleShadowAtten = { 0.5f,0.6f,0.0f };
	Vector2  circleShadowFactorAngle = { 0.0f,0.5f };
	
};