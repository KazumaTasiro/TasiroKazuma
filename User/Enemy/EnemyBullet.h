#pragma once
#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"


///<summary>
///���L����
///</summary>
class EnemyBullet {
public:
	///<summary>
	///������
	///</summary>

	void Initialize(const Vector3& position, const Vector3& velocity, Model* bulletModel);



	///<summary>
	///�X�V
	///</summary>

	void Update();

	///<summary>
	///�`��
	///</summary>

	void Draw();

	bool IsDead()const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	Vector3 GetWorldPosition();

	void SetSize(Vector3 Size) { worldTransform_->wtf.scale = Size; }


private:
	//���[���h�ϊ��f�[�^
	Object3d* worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//����<fim>
	static const int32_t kLifeTime = 60 * 1000;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;
};