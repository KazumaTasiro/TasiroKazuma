#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input/Input.h"
///<summary>
///���L����
///</summary>
class PlayerBullet {
public:
	///<summary>
	///������
	///</summary>
	void Initialize(Model* model, const XMFLOAT3& position, const XMFLOAT3& velocity);;
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

	XMFLOAT3 GetWorldPosition();

private:
	//���[���h�ϊ��f�[�^
	Object3d* worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	XMFLOAT3 velocity_;
	//����<fim>
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};
