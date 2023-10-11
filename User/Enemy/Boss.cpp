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

void Boss::Update(Player* player_)
{

	
	assert(player_);
	if (fireCount >= 4.0f) {
		randBossAttackNmb = rand() % 2;
		fireCount = 0;
	}
	this->player = player_;
	worldTransformReticle_->wtf.position = worldTransform_->wtf.position;
	worldTransformReticle_->Update();
	float time_ = 0.02f;
	spline->Update(time_);
	//キャラクター移動処理
	Move();

	//worldTransformReticle_->wtf.position.z = 0;
	if (isDead_) {
		for (std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_) {
			//自弾の衝突時コールバックを呼び出す
			enemyBullets->OnCollision();
		}
	}
	//デスフラグの立った弾を削除
	EnemyLockBullets_.remove_if([](std::unique_ptr<LockOnBullet>& bullet) {
		return bullet->IsDead();
		});
	//弾更新
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
				//弾を生成し、初期化
				std::unique_ptr<LockOnBullet> newBullet = std::make_unique<LockOnBullet>();
				newBullet->Initialize(enemyBulletModel_, player->GetWorldPosition());
				//弾を発射する
				EnemyLockBullets_.push_back(std::move(newBullet));
				lockOn = false;
			}
		}
	}
	worldTransform_->Update();
}

Vector3 Boss::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}

void Boss::Draw()
{
	//弾更新
	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		bullet->Draw();
	}
	//弾更新
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
	//判定対象AとBの座標
	Vector3 posA, posB;

	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		//敵キャラも座標
		posA = GetWorldPosition();

		//自弾の座標
		posB = bullet->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA, 50.0f, 50.0f)) {
			//敵キャラの衝突時コールバックを呼び出す
			OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	for (std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_) {
		//敵キャラも座標
		posA = player->GetWorldPosition();

		//自弾の座標
		posB = enemyBullets->GetWorldPosition();

		if (randBossAttackNmb == 0) {
			if (Collision::CircleCollision(posB, posA, 1.5f, 1.5f)) {
				//プレイヤーの衝突時コールバックを呼び出す
				player->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				enemyBullets->OnCollision();
			}
		}
		else if (randBossAttackNmb == 1) {
			if (Collision::CircleCollision(posB, posA, 4.0f, 4.0f)) {
				//プレイヤーの衝突時コールバックを呼び出す
				player->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				enemyBullets->OnCollision();
			}
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
		//自弾の衝突時コールバックを呼び出す
		bullet->OnCollision();
	}
	for (std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_) {
		//自弾の衝突時コールバックを呼び出す
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
		if (randBossAttackNmb == 0) {
			fireFlag = true;
			fireTime = 35;
		}
		else if (randBossAttackNmb == 1) {
			fireFlag = true;
			fireTime = 70;
		}

	}
	if (fireFlag) {
		if (randBossAttackNmb == 0) {
			velocity_ = player->GetWorldPosition() - worldTransform_->wtf.position;
			velocity_.nomalize();
			velocity_ *= verocitySpeed;
			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
			newEnemyBullet->Initialize(worldTransform_->wtf.position, velocity_, enemyBulletModel_);
			//弾を発射する
			EnemyBullets_.push_back(std::move(newEnemyBullet));
			fireFlag = false;
			fireCount += 0.5f;
		}
		if (randBossAttackNmb == 1) {
			velocity_ = player->GetWorldPosition() - worldTransform_->wtf.position;
			velocity_.nomalize();
			velocity_ *= verocitySpeed;
			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
			newEnemyBullet->Initialize(worldTransform_->wtf.position, velocity_, enemyBulletModel_);
			newEnemyBullet->SetSize({ 4.0f,4.0f,4.0f });
			//弾を発射する
			EnemyBullets_.push_back(std::move(newEnemyBullet));
			fireFlag = false;
			fireCount += 1.0f;
		}
	}
	//弾更新
	for (std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_) {
		enemyBullet->Update();
	}
}
