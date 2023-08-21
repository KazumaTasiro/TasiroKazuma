#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include "Vector3.h"
#include "SplinePosition.h"

class LockOnBullet
{
public:
	LockOnBullet();
	~LockOnBullet();

	///<summary>
	///������
	///</summary>
	void Initialize(Model* model, const Vector3& position);
	///<summary>
	///�X�V
	///</summary>
	void Update(const Vector3& enemyPos);
	///<summary>
	///�`��
	///</summary>
	void Draw();

	bool IsDead()const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	Vector3 GetWorldPosition();
private:
	//���[���h�ϊ��f�[�^
	Object3d* worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	float speed = 3.0f;

	//���x
	Vector3 velocity_;
	//����<fim>
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;

	float time = 0;

	Vector3 worldNormal;
};
