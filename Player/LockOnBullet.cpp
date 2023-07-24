#include "LockOnBullet.h"

LockOnBullet::LockOnBullet()
{
}

LockOnBullet::~LockOnBullet()
{
}

void LockOnBullet::Initialize(Model* model, const Vector3& position)
{
	assert(model);

	model_ = model;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_ = Object3d::Create();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_->wtf.position = (position);



	//���f�����Z�b�g����
	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = { 1,1,1 };
}

void LockOnBullet::Update(const Vector3& enemyPos)
{
	////�����Ŏ󂯎�������x�������o�֐��ɑ��
	//velocity_ = velocity;

	//velocity_ = { 0,0,5 };

	velocity_ = enemyPos - worldTransform_->wtf.position;

	Vector3 position;
	position.x = velocity_.x;
	position.y = velocity_.y;
	position.z = velocity_.z;

	position.nomalize();
	position *= speed;
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_->wtf.position += (position);

	worldTransform_->Update();
	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void LockOnBullet::Draw()
{
	worldTransform_->Draw();
}

void LockOnBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 LockOnBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_->wtf.matWorld.m[3][0];
	worldPos.y = worldTransform_->wtf.matWorld.m[3][1];
	worldPos.z = worldTransform_->wtf.matWorld.m[3][2];

	return worldPos;
}
