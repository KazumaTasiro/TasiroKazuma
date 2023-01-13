#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const XMFLOAT3& position, const XMFLOAT3& velocity)
{
	assert(model);

	model_ = model;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_= Object3d::Create();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_->SetPosition(position);

	//�����Ŏ󂯎�������x�������o�֐��ɑ��
	velocity_ = velocity;

	//���f�����Z�b�g����
	worldTransform_->SetModel(model_);

	worldTransform_->SetScale({ 1,1,1 });
}

void PlayerBullet::Update()
{
	velocity_ = { 0,0,5 };
	XMFLOAT3 position;
	position = worldTransform_->GetPosition();
	position.x += velocity_.x;
	position.y += velocity_.y;
	position.z += velocity_.z;
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_->SetPosition(position);

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

XMFLOAT3 PlayerBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	XMFLOAT3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_->GetmMatWorld().r[3].m128_f32[0];
	worldPos.y = worldTransform_->GetmMatWorld().r[3].m128_f32[1];
	worldPos.z = worldTransform_->GetmMatWorld().r[3].m128_f32[2];

	return worldPos;
}
