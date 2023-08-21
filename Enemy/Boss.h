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


class GameScene;
class Player;
class Boss
{
public:
	Boss();
	~Boss();

	///<summary>
	///������
	///</summary>
	void Initialize(Model* enemyBulletModel, Model* enemyReticleModel, Input* input);
	///<summary>
	///�X�V
	///</summary>

	void Update(Player* player);

	void Move();


	//void Fire();

	Vector3 GetWorldPosition();
	///<summary>
	///�`��
	///</summary>
	void Draw();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead()const { return isDead_; }

	void LockOnTrue();

	void OnColl();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	void Reset();

	bool isDead();

	void Fire();
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
	Vector3 EnemyMoveSpline1 = { 0,150,200 };
	Vector3 EnemyMoveSpline2 = { 0,100,200 };

	//�e
	std::list<std::unique_ptr<LockOnBullet>> EnemyLockBullets_;
	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;

	int32_t time = 0;

	Player* player = nullptr;

	int EnemyHp = 10;

	//�f�X�t���O
	bool isDead_ = false;

	bool lockOn = false;
	float move = 0.1f;

	Sprite* spriteLock = nullptr;

	SplinePosition* spline = nullptr;
	bool DemoEnemyMove = false;

	float PI = 3.141592f;

	bool fireFlag = false;

	int fireTime = 45;

	float verocitySpeed = 3.0f;

	Vector3 velocity_;
};
