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

	worldNormal = worldTransform_->wtf.position;

	//���f�����Z�b�g����
	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = { 1,1,1 };
}

void LockOnBullet::Update(const Vector3& enemyPos)
{
	////�����Ŏ󂯎�������x�������o�֐��ɑ��
	Vector3 toEnemy = enemyPos;
	velocity_ = enemyPos - worldTransform_->wtf.position;

	/*toEnemy.nomalize();*/
	/*velocity_.nomalize();*/

	time = 0.5f;

	Vector3 position;
	velocity_ = Vector3::slerp(velocity_, toEnemy, time) * speed;

	//position.nomalize();
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_->wtf.position += velocity_;

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
