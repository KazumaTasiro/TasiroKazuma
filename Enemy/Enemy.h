#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include <memory>
#include <list>
#include "Vector3.h"

///<summary>
///�G�L����
///</summary>

class GameScene;
//class Player;
class Enemy {
public:
	///<summary>
	///������
	///</summary>
	void Initialize(Vector3 EnemyPos);
	///<summary>
	///�X�V
	///</summary>

	void Update();

	void Move();


	//void Fire();

	Vector3 GetWorldPosition();
	///<summary>
	///�`��
	///</summary>
	void Draw();
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

private:
	//���ˊԊu
	static const int kFireInterval = 100;


	GameScene* gameScene_ = nullptr;

	//���[���h�ϊ��f�[�^
	Object3d* worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//���f��
	Model* enemyBulletModel_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	////�t�F�[�Y
	//Phase phase_ = Phase::Approch;
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 ApprochMove = { 0,0,0.0f };
	Vector3 LeaveMove = { -0.1f,0.1f,-0.1f };


	int32_t time = 0;

	int EnemyHp = 1;

	//�f�X�t���O
	bool isDead_ = false;

	bool lockOn = false;
	float move = 0.1f;
};