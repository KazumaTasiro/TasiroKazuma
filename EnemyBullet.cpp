#include "EnemyBullet.h"

void EnemyBullet::Initialize( const XMFLOAT3& position, const XMFLOAT3& velocity)
{

	model_ = Model::LoadFormOBJ("EnemyBullet");
	//�e�N�X�`���ǂݍ���
	/*textureHandle_ = TextureManager::Load("EnemyBullet.jpg");*/

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_ = Object3d::Create();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_->position = position;
	worldTransform_->SetModel(model_);
	//�����Ŏ󂯎�������x�������o�֐��ɑ��
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	XMFLOAT3 pos = worldTransform_->GetPosition();
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	pos.x += velocity_.x;
	pos.y += velocity_.y;
	pos.z += velocity_.z;

	worldTransform_->Update();
	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw()
{
	worldTransform_->Draw();
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

XMFLOAT3 EnemyBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	XMFLOAT3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_->GetmMatWorld().r[3].m128_f32[0];
	worldPos.y = worldTransform_->GetmMatWorld().r[3].m128_f32[0];
	worldPos.z = worldTransform_->GetmMatWorld().r[3].m128_f32[0];

	return worldPos;
}
