#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(Vector3 EnemyPos, Input* input, SpriteCommon* sptriteCommon)
{
	assert(input);
	assert(sptriteCommon);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	//���[���h�ϊ��̏�����
	worldTransform_ = Object3d::Create();;
	worldTransform_->wtf.position = EnemyPos;
	
	model_ = Model::LoadFormOBJ("cubeObj");
	enemyBulletModel_ = Model::LoadFormOBJ("bullet");
	enemyReticleModel_ = Model::LoadFormOBJ("Reticle");

	worldTransformReticle_ = Object3d::Create();
	worldTransformReticle_->wtf.position = EnemyPos;
	worldTransformReticle_->SetModel(enemyReticleModel_);
	worldTransformReticle_->wtf.scale = { 10,10,10 };

	spriteLock = new Sprite();
	spriteLock->Initialize(sptriteCommon, 2);

	input_ = input;

	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = { 3,3,3 };
	EnemyMoveSpline0 = { worldTransform_->wtf.position.x + 100, worldTransform_->wtf.position.y - 25, 150.0f };
	spline = new SplinePosition(worldTransform_->wtf.position, EnemyMoveSpline1, EnemyMoveSpline2, EnemyMoveSpline0);
}

void Enemy::Update(Player* player)
{

	assert(player);
	this->player = player;
	worldTransformReticle_->wtf.position = worldTransform_->wtf.position;
	worldTransformReticle_->Update();
	float time = 0.02f;
	spline->Update(time);
	//worldTransformReticle_->wtf.position.z = 0;
	//�f�X�t���O�̗������e���폜
	EnemyLockBullets_.remove_if([](std::unique_ptr<LockOnBullet>& bullet) {
		return bullet->IsDead();
		});
	//�e�X�V
	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		bullet->Update(GetWorldPosition());
	}
	//�L�����N�^�[�ړ�����
	Move();
	spriteLock->SetPozition({ GetWorldPosition().x,GetWorldPosition().y });
	if (EnemyHp <= 0) {
		isDead_ = true;
	}
	OnColl();
}

void Enemy::Move()
{
	if (DemoEnemyMove == false) {
		worldTransform_->wtf.position = spline->NowPos;
		if (spline->NowPos.x == EnemyMoveSpline0.x && spline->NowPos.y == EnemyMoveSpline0.y && spline->NowPos.z == EnemyMoveSpline0.z) {
			DemoEnemyMove = true;
		}
	}
	if (DemoEnemyMove == true) {
		if (worldTransform_->wtf.position.x >= 5 || worldTransform_->wtf.position.x <= -5) {
			move = -move;
		}
		worldTransform_->wtf.position = { worldTransform_->wtf.position.x + move,worldTransform_->wtf.position.y,worldTransform_->wtf.position.z };

		if (input_->ReleaseMouse(1)) {
			if (lockOn == true) {
				//�e�𐶐����A������
				std::unique_ptr<LockOnBullet> newBullet = std::make_unique<LockOnBullet>();
				newBullet->Initialize(enemyBulletModel_, player->GetWorldPosition());
				//�e�𔭎˂���
				EnemyLockBullets_.push_back(std::move(newBullet));
				lockOn = false;
			}
		}
	}
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
	//�e�X�V
	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		bullet->Draw();
	}
	worldTransform_->Draw();
	if (lockOn) {
		worldTransformReticle_->Draw();
	}
}

void Enemy::DrawUI()
{
	//if (lockOn) {
	//	spriteLock->Draw();
	//}
}

void Enemy::OnCollision()
{
	EnemyHp--;

}

void Enemy::LockOnTrue()
{
	lockOn = true;
}

void Enemy::OnColl()
{
	//����Ώ�A��B�̍��W
	Vector3 posA, posB;

	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		//�G�L���������W
		posA = GetWorldPosition();

		//���e�̍��W
		posB = bullet->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA, 2.0f, 2.0f)) {
			//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
			OnCollision();
			//���e�̏Փˎ��R�[���o�b�N���Ăяo��
			bullet->OnCollision();
		}
	}
}

int Enemy::ReturnOnColl()
{
	if (EnemyHp <= 0) {
		return 1;
	}
	return 0;
}
