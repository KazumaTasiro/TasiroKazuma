#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include <memory>
#include <list>
#include "Vector3.h"
#include "LockOnBullet.h"
#include "Sprite.h"
#include "Collision.h"
#include "SplinePosition.h"
#include "EnemyBullet.h"

///<summary>
///�G�L����
///</summary>

class GameScene;
class Player;
class BossSubordinate {
public:
	///<summary>
	///������
	///</summary>
	void Initialize(Vector3 EnemyPos, Input* input, SpriteCommon* sptriteCommon, Model* model, Model* enemyBulletModel, Model* enemyReticleModel, int EnemyNmb);
	///<summary>
	///�X�V
	///</summary>

	void Update(Player* player);

	void Move();


	void Fire();

	Vector3 GetWorldPosition();
	///<summary>
	///�`��
	///</summary>
	void Draw();
	///<summary>
	///�`��
	///</summary>
	void DrawUI();

	////�s���t�F�[�Y
	//enum class Phase {
	//	Approch,//�ڋ߂���
	//	Leave,//���E����
	//};
	//void Approch();

	//void Leave();


	////�x�N�g���𐳋K������
	//int Vec3Normalize(Vector3* pOut, Vector3* pV);

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	////�e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead()const { return isDead_; }

	bool IsTackleDead()const { return isTackleDead_; }

	void LockOnTrue();

	void setPlayer(Player* player_) { player = player_; }

	void OnColl();


	int ReturnOnColl();

	//�e���X�g���擾
	const std::list<std::unique_ptr<LockOnBullet>>& GetBullets() { return EnemyLockBullets_; }

	void CollTackle();

	bool GetMoveFlag() { return DemoEnemyMove; }

private:
	//���ˊԊu
	static const int kFireInterval = 100;

	Input* input_ = nullptr;

	GameScene* gameScene_ = nullptr;

	//���[���h�ϊ��f�[�^
	Object3d* worldTransform_;
	//���[���h�ϊ��f�[�^
	Object3d* worldTransformReticle_;
	//�G���f��
	Model* model_ = nullptr;
	//�G�̒e���f��
	Model* enemyBulletModel_ = nullptr;
	//�G�̏Ə����f��
	Model* enemyReticleModel_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	////�t�F�[�Y
	//Phase phase_ = Phase::Approch;
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 ApprochMove = { 0,0,0.0f };
	Vector3 LeaveMove = { -0.1f,0.1f,-0.1f };

	Vector3 EnemyMoveSpline0 = { 0,0,0 };
	Vector3 EnemyMoveSpline1 = { -50,20,50 };
	Vector3 EnemyMoveSpline2 = { -20,15,100 };

	Vector3 EnemyReMoveSpline0 = { 0,0,0 };
	Vector3 EnemyReMoveSpline1 = { -50,30,-50 };
	Vector3 EnemyReMoveSpline2 = { -20,30,100 };

	//�ړ��t���O
	bool moveEndFlag = false;

	//�e
	std::list<std::unique_ptr<LockOnBullet>> EnemyLockBullets_;
	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;

	int32_t time = 0;

	Player* player = nullptr;

	int EnemyHp = 1;

	//�f�X�t���O
	bool isDead_ = false;
	//�f�X�t���O
	bool isTackleDead_ = false;

	bool lockOn = false;
	float move = 0.1f;

	Sprite* spriteLock = nullptr;

	bool DemoEnemyMove = false;

	bool fireFlag = false;

	int fireTime = 45;

	float verocitySpeed = 3.0f;

	int enemyNmb = 1;

	bool TackleReMove = false;

	Vector3 velocity_;
	Vector3 velocityTackle;
};