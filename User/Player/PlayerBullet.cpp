#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	model_ = model;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_ = Object3d::Create();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_->wtf.position=(position);

	//�����Ŏ󂯎�������x�������o�֐��ɑ��
	velocity_ = velocity;

	//���f�����Z�b�g����
	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale={ 1,1,1 };
}

void PlayerBullet::Update()
{
	//velocity_ = { 0,0,5 };
	Vector3 vec = velocity_;
	vec*= speed;


	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_->wtf.position += vec;

	worldTransform_->Update();
	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	worldTransform_->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_->wtf.matWorld.m[3][0];
	worldPos.y = worldTransform_->wtf.matWorld.m[3][1];
	worldPos.z = worldTransform_->wtf.matWorld.m[3][2];

	return worldPos;
}
