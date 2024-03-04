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
#include "ParticleLibrary.h"
#include "DirectionalLight.h"

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

	//カーソルが画面外に出ない処理
	void CursorLimit();

	void LightUpdate();

private:
	enum Scene
	{
		Title,
		Game,
		Boss,
		Over,
		Clear,
	};
	Scene scene;
private:
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
private://プレイヤー
	
	///自キャラ
	std::unique_ptr<Player> player_ = nullptr;

	bool playMove = true;

	float playPos = 0;

	bool TitleEnd = false;
private://スプライト

	std::unique_ptr <Sprite> stert_ = nullptr;
	std::unique_ptr <Sprite> gameClear_ = nullptr;
	std::unique_ptr <Sprite> gameOver_ = nullptr;
	std::unique_ptr <Sprite> operation_ = nullptr;
	std::unique_ptr <Sprite> cameraMoveOps_ = nullptr;
	std::unique_ptr <Sprite> moveTute_ = nullptr;

private:
	//ライト
	DirectionalLight* direLight = nullptr;
private:

	std::unique_ptr<RailCamera> railCamera = nullptr;
	

	//std::list<std::unique_ptr<Enemy>> enemy_;

	Input* input_ = nullptr;

	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	ImGuiManager* ImGuiMan_ = nullptr;


	std::unique_ptr < Audio> audio_ = nullptr;


	std::unique_ptr < Camera> camera_ = nullptr;

	RECT rcClip;
	RECT rcOldClip;


	std::unique_ptr <Road> road_ = nullptr;

	std::unique_ptr < Skydome> skydome_ = nullptr;

	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;



	Vector2 spriteEnd_;
	Vector2 operationPos = {128,64};

	

	std::unique_ptr<ParticleManager> ParticleMana_;

	std::unique_ptr <GameOver> gameOverSeen = nullptr;
	std::unique_ptr <GameClear> gameClearScene = nullptr;


	Vector3 cameraTitle = { 0,0,10 };
	Vector3 cameraGame = { 0,0,-10 };

	std::unique_ptr <Titles> title_ = nullptr;

	std::unique_ptr<SeenTransition> seenTransition_ = nullptr;
	bool seenFlag = false;

	std::unique_ptr <StertCount> stertCount_ = nullptr;

	Vector3 TargetCamRes = { 0,0,0 };


	bool DemoClear = false;
	float gameOverUp = 80.0f;
	float gamestertUp = 50.0f;

	float bossTime = 3.0f;

	LightGroup* lightGroupNon = nullptr;
	//LightGroup* lightGroupUse = nullptr;

	Vector4  circleShadowDir = { 0,0.5f,0,0 };
	Vector3  circleShadowAtten = { 1.1f,0.7f,0.0f };
	Vector2  circleShadowFactorAngle = { 0.5f,1.2f };

	Vector3 cameraImgui = { 0,0,0 };

	int32_t pointLight;
	Vector3 pointLightPos = { 0,0,0 };
	Vector3 pointLightColor = { 0,0,0 };
	Vector3 pointLightAtten = { 0,0,0 };
	
};