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
	/// �R���X�g�N���^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxcomon,Input* input_);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void UpdateEnemyPopCommands();

	void ExistenceEnemy(const XMFLOAT3& EnemyPos);

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }

	//�Q�[���t�F�[�Y
	enum class Phase {
		TITLE,//�^�C�g��
		GAME,//�{��
		CLEAR,//�N���A���
		GAMEOVER,//�Q�[���I�[�o�[���
	};

	void PhaseReset();

	void EnemyPopComandReset();

private:
	///���L����
	Player* player_ = nullptr;

	Input* input = nullptr;
	///�G�L����

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

	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	bool waitflag = false;
	int waitTimer = 0;

	//�t�F�[�Y
	Phase phase_ = Phase::TITLE;

	int EnemyDeadCount = 0;

	bool PlayerDead = false;

	Skydome* skydome = nullptr;

};