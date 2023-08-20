#include "EnemyBullet.h"

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity ,Model* bulletModel)
{

	model_ = bulletModel;
	//�e�N�X�`���ǂݍ���
	/*textureHandle_ = TextureManager::Load("EnemyBullet.jpg");*/

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_ = Object3d::Create();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_->wtf.position = position;
	worldTransform_->SetModel(model_);
	//�����Ŏ󂯎�������x�������o�֐��ɑ��
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	Vector3 pos = velocity_;
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)

	worldTransform_->wtf.position += velocity_;
	worldTransform_->Update();
	deathTimer_--;
	//���Ԍo�߂Ńf�X
	if (deathTimer_ <= 0) {
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

Vector3 EnemyBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}