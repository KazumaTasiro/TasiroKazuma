#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(Vector3 EnemyPos, Input* input, SpriteCommon* sptriteCommon,Model* model,Model* enemyBulletModel,Model* enemyReticleModel, int EnemyNmb,int EnemyRootNmb)
{
	assert(input);
	assert(sptriteCommon);
	assert(model);
	assert(enemyBulletModel);
	assert(enemyReticleModel);

	EnemyRootNmb_ = EnemyRootNmb;

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	//���[���h�ϊ��̏�����
	worldTransform_ = Object3d::Create();;
	worldTransform_->wtf.position = EnemyPos;
	
	model_ = model;
	enemyBulletModel_ = enemyBulletModel;
	enemyReticleModel_ = enemyReticleModel;

	worldTransformReticle_ = Object3d::Create();
	worldTransformReticle_->wtf.position = EnemyPos;
	worldTransformReticle_->SetModel(enemyReticleModel_);
	worldTransformReticle_->wtf.scale = { 10,10,10 };

	spriteLock = new Sprite();
	spriteLock->Initialize(sptriteCommon, 2);

	input_ = input;

	enemyNmb = EnemyNmb;

	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = { 3,3,3 };
	if (EnemyRootNmb_ == 0) {
		EnemyMoveSpline0 = { 0,0,0 };
		EnemyMoveSpline1 = { -50,20,50 };
		EnemyMoveSpline2 = { -20,15,100 };
		EnemyMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - 25, 150.0f };
		EnemyReMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - 25, 150.0f };
		EnemyReMoveSpline1 = { worldTransform_->wtf.position.x - 100, 30 , -50.0f };
		EnemyReMoveSpline2 = { worldTransform_->wtf.position.x - 100, 30, 100.0f };
	}
	else if (EnemyRootNmb_ == 1) {
		EnemyMoveSpline0 = { 0,0,0 };
		EnemyMoveSpline1 = { +50,20,50 };
		EnemyMoveSpline2 = { +20,15,100 };
		EnemyMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - 25, 150.0f };
		EnemyReMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - 25, 150.0f };
		EnemyReMoveSpline1 = { worldTransform_->wtf.position.x + 100, 30 , -50.0f };
		EnemyReMoveSpline2 = { worldTransform_->wtf.position.x + 100, 30, 100.0f };
	}

	spline = new SplinePosition(worldTransform_->wtf.position, EnemyMoveSpline1, EnemyMoveSpline2, EnemyMoveSpline0);
	splineReMove = new SplinePosition(worldTransform_->wtf.position, EnemyReMoveSpline1, EnemyReMoveSpline2, EnemyReMoveSpline0);
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
	EnemyLockBullets_.remove_if([](std::unique_ptr<LockOnBullet>& LockBullet) {
		return LockBullet->IsDead();
		});
	EnemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullet) {
		return enemyBullet->IsDead();
		});

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
		Fire();
		if (enemyNmb == 0) {
			if (worldTransform_->wtf.position.x >= 5 || worldTransform_->wtf.position.x <= -5) {
				move = -move;
			}
			worldTransform_->wtf.position = { worldTransform_->wtf.position.x + move,worldTransform_->wtf.position.y,worldTransform_->wtf.position.z };
		}
		if (TackleReMove) {
			float time = 0.05f;
			splineReMove->Update(time);
			worldTransform_->wtf.position = splineReMove->NowPos;
			if (splineReMove->NowPos.x == EnemyMoveSpline0.x && splineReMove->NowPos.y == EnemyMoveSpline0.y && splineReMove->NowPos.z == EnemyMoveSpline0.z) {
				TackleReMove = false;
				fireFlag = false;
			}
		}
		else
		{
			splineReMove->Reset();

		}
	}

	worldTransform_->Update();
}

void Enemy::Fire()
{
	fireTime--;
	if (fireTime<=0) {
		fireFlag = true;
		fireTime = 45;

	}
	if (fireFlag == false) {
		velocityTackle = player->GetWorldPosition() - worldTransform_->wtf.position;
		velocityTackle.nomalize();
		velocityTackle *= verocitySpeed;
	}
	if (fireFlag) {


		if (enemyNmb == 0) {
			velocity_ = player->GetWorldPosition() - worldTransform_->wtf.position;
			velocity_.nomalize();
			velocity_ *= verocitySpeed;
			//�e�𐶐����A������
			std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
			newEnemyBullet->Initialize(worldTransform_->wtf.position, velocity_, enemyBulletModel_);
			//�e�𔭎˂���
			EnemyBullets_.push_back(std::move(newEnemyBullet));
			fireFlag = false;
		}
		if (enemyNmb == 1) {
			worldTransform_->wtf.position += velocityTackle;

			if (worldTransform_->wtf.position.z <= -50) {
				TackleReMove = true;
			}

		}
	}
	if (input_->ReleaseMouse(1)) {
		if (lockOn == true) {
			//�e�𐶐����A������
			std::unique_ptr<LockOnBullet> newLockBullet = std::make_unique<LockOnBullet>();
			newLockBullet->Initialize(enemyBulletModel_, player->GetWorldPosition());
			//�e�𔭎˂���
			EnemyLockBullets_.push_back(std::move(newLockBullet));
			lockOn = false;
		}
	}
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_) {
		enemyBullet->Update();
	}
	//�e�X�V
	for (std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_) {
		LockBullet->Update(GetWorldPosition());
	}
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
	for (std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_) {
		LockBullet->Draw();
	}
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_) {
		enemyBullet->Draw();
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

	for (std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_) {
		//�G�L���������W
		posA = GetWorldPosition();

		//���e�̍��W
		posB = LockBullet->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA, 2.0f, 2.0f)) {
			//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
			OnCollision();
			//���e�̏Փˎ��R�[���o�b�N���Ăяo��
			LockBullet->OnCollision();
		}
	}
	for (std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_) {
		//�G�L���������W
		posA = player->GetWorldPosition();

		//���e�̍��W
		posB = enemyBullets->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA, 1.5f, 1.5f)) {
			//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
			player->OnCollision();
			//���e�̏Փˎ��R�[���o�b�N���Ăяo��
			enemyBullets->OnCollision();
		}
	}
	//�G�L���������W
	posA = player->GetWorldPosition();

	//���e�̍��W
	posB = worldTransform_->GetWorldPosition();

	if (Collision::CircleCollision(posB, posA, 1.0f, 1.0f)) {
		//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
		player->OnCollision();
		//���e�̏Փˎ��R�[���o�b�N���Ăяo��
		CollTackle();
	}
}

int Enemy::ReturnOnColl()
{
	if (EnemyHp <= 0) {
		return 1;
	}
	return 0;
}

void Enemy::CollTackle()
{
	isTackleDead_ = true;
}
