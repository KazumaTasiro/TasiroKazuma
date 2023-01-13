#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Model.h"
#include "Enemy.h"
#include "Player.h"
#include <sstream>
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
	void Initialize(DirectXCommon* dxcomon,Input* input_);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void UpdateEnemyPopCommands();

	void ExistenceEnemy(const XMFLOAT3& EnemyPos);

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }

	//ゲームフェーズ
	enum class Phase {
		TITLE,//タイトル
		GAME,//本編
		CLEAR,//クリア画面
		GAMEOVER,//ゲームオーバー画面
	};

	void PhaseReset();

	void EnemyPopComandReset();

private:
	///自キャラ
	Player* player_ = nullptr;

	Input* input = nullptr;
	///敵キャラ

	std::list<std::unique_ptr<Enemy>> enemy_;

	DirectXCommon* dxCommon_ = nullptr;

	Audio* audio = nullptr;
	SpriteCommon* spritecommon = nullptr;
	Sprite* title = nullptr;
	Sprite* gameClear = nullptr;
	Sprite* gameOver = nullptr;
	Sprite* attackMove = nullptr;
	Sprite* attackTex = nullptr;
	Sprite* goTitle = nullptr;
	Sprite* moveTex = nullptr;
	Object3d* object3d = nullptr;
	Model* model = nullptr;

	std::list<std::unique_ptr<EnemyBullet>> bullets2_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	bool waitflag = false;
	int waitTimer = 0;

	//フェーズ
	Phase phase_ = Phase::TITLE;

	int EnemyDeadCount = 0;

	bool PlayerDead = false;

	Skydome* skydome = nullptr;

};