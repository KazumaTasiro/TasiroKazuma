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

	//引数として受け取ったデータをメンバ変数に記録する
	//ワールド変換の初期化
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
	//デスフラグの立った弾を削除
	EnemyLockBullets_.remove_if([](std::unique_ptr<LockOnBullet>& LockBullet) {
		return LockBullet->IsDead();
		});
	EnemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullet) {
		return enemyBullet->IsDead();
		});

	//キャラクター移動処理
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
			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
			newEnemyBullet->Initialize(worldTransform_->wtf.position, velocity_, enemyBulletModel_);
			//弾を発射する
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
			//弾を生成し、初期化
			std::unique_ptr<LockOnBullet> newLockBullet = std::make_unique<LockOnBullet>();
			newLockBullet->Initialize(enemyBulletModel_, player->GetWorldPosition());
			//弾を発射する
			EnemyLockBullets_.push_back(std::move(newLockBullet));
			lockOn = false;
		}
	}
	//弾更新
	for (std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_) {
		enemyBullet->Update();
	}
	//弾更新
	for (std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_) {
		LockBullet->Update(GetWorldPosition());
	}
}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}

void Enemy::Draw()
{
	//弾更新
	for (std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_) {
		LockBullet->Draw();
	}
	//弾更新
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
	//判定対象AとBの座標
	Vector3 posA, posB;

	for (std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_) {
		//敵キャラも座標
		posA = GetWorldPosition();

		//自弾の座標
		posB = LockBullet->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA, 2.0f, 2.0f)) {
			//敵キャラの衝突時コールバックを呼び出す
			OnCollision();
			//自弾の衝突時コールバックを呼び出す
			LockBullet->OnCollision();
		}
	}
	for (std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_) {
		//敵キャラも座標
		posA = player->GetWorldPosition();

		//自弾の座標
		posB = enemyBullets->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA, 1.5f, 1.5f)) {
			//敵キャラの衝突時コールバックを呼び出す
			player->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			enemyBullets->OnCollision();
		}
	}
	//敵キャラも座標
	posA = player->GetWorldPosition();

	//自弾の座標
	posB = worldTransform_->GetWorldPosition();

	if (Collision::CircleCollision(posB, posA, 1.0f, 1.0f)) {
		//敵キャラの衝突時コールバックを呼び出す
		player->OnCollision();
		//自弾の衝突時コールバックを呼び出す
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
