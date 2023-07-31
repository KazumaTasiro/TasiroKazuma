#pragma once
#include "DirectXCommon.h"
#include "PlayerBullet.h"
#include "Collision.h"
#include "Enemy.h"
#include "Audio.h"
#include "Input.h"
#include "SpriteCommon.h"
#include "LockOnBullet.h"

class Player;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Initialize(DirectXCommon* dxCommon, Input* input,SpriteCommon* spriteCommon, Camera* camera);

	void Update();

	void Draw();

	void DrawUI();


	void SetPlayer(Player* player) { player_ = player; };

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsAllEnemyDead();

	/// <summary>
	/// �G��CSV�̓ǂݍ���
	/// </summary>
	void LoadEnemyPopData();

	//CSV���Z�b�g
	void EnemyPopComandReset();

	/// <summary>
	/// �G��CSV�̉��
	/// </summary>
	void UpdateEnemyPopCommands();

	//�G�̓o�^
	void ExistenceEnemy(const Vector3& EnemyPos);

	//�G�Ƃ̓����蔻��
	void EnemyCollision(Player* player);

	void EnemyReset();

	bool Clear();
public:
	//�����~�߂�֐�
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };

private:
	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Audio* audio = nullptr;
	Transform* origin = nullptr;

	GameScene* gameScene_ = nullptr;

	Player* player_ = nullptr;

	Input* input_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemy_;

	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	bool waitflag = false;
	int waitTimer = 0;
	Camera* camera_ = nullptr;
	int clearCount = 0;
	int clearNum = 3;
};