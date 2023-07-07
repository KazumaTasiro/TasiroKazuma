#include "Enemy.h"

void Enemy::Initialize(Vector3 EnemyPos)
{
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	//���[���h�ϊ��̏�����
	worldTransform_ = Object3d::Create();;
	worldTransform_->wtf.position = EnemyPos;

	model_ = Model::LoadFormOBJ("cubeObj");

	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = { 3,3,3 };
}

void Enemy::Update()
{
	//�L�����N�^�[�ړ�����
	Move();

	if (EnemyHp <= 0) {
		isDead_ = true;
	}
}

void Enemy::Move()
{

	if (worldTransform_->wtf.position.x >= 5 || worldTransform_->wtf.position.x <= -5) {
		move = -move;
	}
	worldTransform_->wtf.position = { worldTransform_->wtf.position.x + move,worldTransform_->wtf.position.y,worldTransform_->wtf.position.z };
	worldTransform_->Update();
}

Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}

void Enemy::Draw()
{
	worldTransform_->Draw();
}

void Enemy::OnCollision()
{
	/*EnemyHp--;*/
}
