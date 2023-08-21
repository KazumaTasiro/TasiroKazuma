#include "Boss.h"
#include "Player.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::Initialize(Model* enemyBulletModel, Model* enemyReticleModel, Input* input)
{
	assert(input);
	assert(enemyBulletModel);
	assert(enemyReticleModel);
	model_ = Model::LoadFormOBJ("Sakaban");
	enemyBulletModel_ = enemyBulletModel;
	enemyReticleModel_ = enemyReticleModel;
	input_ = input;
	worldTransform_ = Object3d::Create();;
	worldTransform_->wtf.position = { 50,200,200 };
	worldTransform_->SetModel(model_);
	worldTransform_->wtf.scale = { 50,50,50 };
	worldTransform_->wtf.rotation = { 0,(89 / PI),0 };
	EnemyMoveSpline0 = { 0,50,200 };

	worldTransformReticle_ = Object3d::Create();
	worldTransformReticle_->wtf.position = worldTransform_->wtf.position;
	worldTransformReticle_->SetModel(enemyReticleModel_);
	worldTransformReticle_->wtf.scale = { 70,70,70 };
	spline = new SplinePosition(worldTransform_->wtf.position, EnemyMoveSpline1, EnemyMoveSpline2, EnemyMoveSpline0);
}

void Boss::Update(Player* player)
{
	assert(player);
	this->player = player;
	worldTransformReticle_->wtf.position = worldTransform_->wtf.position;
	worldTransformReticle_->Update();
	float time = 0.02f;
	spline->Update(time);
	//�L�����N�^�[�ړ�����
	Move();

	//worldTransformReticle_->wtf.position.z = 0;
	if (isDead_) {
		for (std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_) {
			//���e�̏Փˎ��R�[���o�b�N���Ăяo��
			enemyBullets->OnCollision();
		}
	}
	//�f�X�t���O�̗������e���폜
	EnemyLockBullets_.remove_if([](std::unique_ptr<LockOnBullet>& bullet) {
		return bullet->IsDead();
		});
	//�e�X�V
	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		bullet->Update(GetWorldPosition());
	}
	EnemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullet) {
		return enemyBullet->IsDead();
		});

	if (EnemyHp <= 0) {
		isDead_ = true;
	}
	OnColl();
}

void Boss::Move()
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
		Fire();
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

Vector3 Boss::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}

void Boss::Draw()
{
	//�e�X�V
	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		bullet->Draw();
	}
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_) {
		enemyBullet->Draw();
	}
	if (isDead_ == false) {
		worldTransform_->Draw();
	}
	if (lockOn) {
		worldTransformReticle_->Draw();
	}
}

void Boss::LockOnTrue()
{
	lockOn = true;
}

void Boss::OnColl()
{
	//����Ώ�A��B�̍��W
	Vector3 posA, posB;

	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		//�G�L���������W
		posA = GetWorldPosition();

		//���e�̍��W
		posB = bullet->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA, 50.0f, 50.0f)) {
			//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
			OnCollision();
			//���e�̏Փˎ��R�[���o�b�N���Ăяo��
			bullet->OnCollision();
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
}

void Boss::OnCollision()
{
	EnemyHp--;
}

void Boss::Reset()
{
	isDead_ = false;
	DemoEnemyMove = false;
	lockOn = false;
	worldTransform_->wtf.position = { 100,200,200 };
	EnemyHp = 10;
	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		//���e�̏Փˎ��R�[���o�b�N���Ăяo��
		bullet->OnCollision();
	}
	for (std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_) {
		//���e�̏Փˎ��R�[���o�b�N���Ăяo��
		enemyBullets->OnCollision();
	}
	spline->Reset();
}

bool Boss::isDead()
{
	return isDead_;
}

void Boss::Fire()
{
	fireTime--;
	if (fireTime <= 0) {
		fireFlag = true;
		fireTime = 45;

	}
	if (fireFlag) {
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
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_) {
		enemyBullet->Update();
	}
}
