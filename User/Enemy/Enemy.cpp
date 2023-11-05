#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(Vector3 EnemyPos, SpriteCommon* sptriteCommon,Model* model,Model* enemyBulletModel,Model* enemyReticleModel, int EnemyNmb,int EnemyRootNmb)
{
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
	worldTransformReticle_->wtf.scale = EnemyReticleScale;

	spriteLock = new Sprite();
	spriteLock->Initialize(sptriteCommon, two);

	input_ = Input::GetInstance();

	enemyNmb = EnemyNmb;

	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = EnemyScale;
	if (EnemyRootNmb_ == zero) {
		oneEnemyMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - sprineY2, sprineZ3 };
		oneEnemyReMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - sprineY2, sprineZ3 };
		oneEnemyReMoveSpline1 = { worldTransform_->wtf.position.x - sprineX, sprineY1 , -sprineZ1 };
		oneEnemyReMoveSpline2 = { worldTransform_->wtf.position.x - sprineX, sprineY1, sprineZ2 };
		EnemyMoveSpline0 = oneEnemyMoveSpline0;
		EnemyMoveSpline1 = oneEnemyMoveSpline1;
		EnemyMoveSpline2 = oneEnemyMoveSpline2;
		EnemyMoveSpline0 = oneEnemyMoveSpline0;
		EnemyReMoveSpline0 = oneEnemyReMoveSpline0;
		EnemyReMoveSpline1 = oneEnemyReMoveSpline1;
		EnemyReMoveSpline2 = oneEnemyReMoveSpline2;
	}
	else if (EnemyRootNmb_ == one) {
		twoEnemyMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - sprineY2, sprineZ3 };
		twoEnemyReMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - sprineY2, sprineZ3 };
		twoEnemyReMoveSpline1 = { worldTransform_->wtf.position.x + sprineX, sprineY1 , -sprineZ1 };
		twoEnemyReMoveSpline2 = { worldTransform_->wtf.position.x + sprineX, sprineY1, sprineZ2 };
		EnemyMoveSpline0 = twoEnemyMoveSpline0;
		EnemyMoveSpline1 = twoEnemyMoveSpline1;
		EnemyMoveSpline2 = twoEnemyMoveSpline2;
		EnemyMoveSpline0 = twoEnemyMoveSpline0;
		EnemyReMoveSpline0 = twoEnemyReMoveSpline0;
		EnemyReMoveSpline1 = twoEnemyReMoveSpline1;
		EnemyReMoveSpline2 = twoEnemyReMoveSpline2;
	}

	spline = new SplinePosition(worldTransform_->wtf.position, EnemyMoveSpline1, EnemyMoveSpline2, EnemyMoveSpline0);
	splineReMove = new SplinePosition(worldTransform_->wtf.position, EnemyReMoveSpline1, EnemyReMoveSpline2, EnemyReMoveSpline0);
}

void Enemy::Update(Player* player)
{

	assert(player);
	this->player_ = player;
	worldTransformReticle_->wtf.position = worldTransform_->wtf.position;
	worldTransformReticle_->Update();
	float time_ = Updatetime;
	spline->Update(time_);
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
			float time_ = Movetime;
			splineReMove->Update(time_);
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
		fireTime = fireTimeRis;

	}
	if (fireFlag == false) {
		velocityTackle = player_->GetWorldPosition() - worldTransform_->wtf.position;
		velocityTackle.nomalize();
		velocityTackle *= verocitySpeed;
	}
	if (fireFlag) {


		if (enemyNmb == 0) {
			velocity_ = player_->GetWorldPosition() - worldTransform_->wtf.position;
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

			if (worldTransform_->wtf.position.z <= tackPosLim ) {
				TackleReMove = true;
			}

		}
	}
	if (input_->ReleaseMouse(1)) {
		if (lockOn == true) {
			//弾を生成し、初期化
			std::unique_ptr<LockOnBullet> newLockBullet = std::make_unique<LockOnBullet>();
			newLockBullet->Initialize(enemyBulletModel_, player_->GetWorldPosition());
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

		if (Collision::CircleCollision(posB, posA,LockWidth,LockWidth)) {
			//敵キャラの衝突時コールバックを呼び出す
			OnCollision();
			//自弾の衝突時コールバックを呼び出す
			LockBullet->OnCollision();
		}
	}
	for (std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_) {
		//敵キャラも座標
		posA = player_->GetWorldPosition();

		//自弾の座標
		posB = enemyBullets->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA,playerWidth,playerWidth)) {
			//敵キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			enemyBullets->OnCollision();
		}
	}
	//敵キャラも座標
	posA = player_->GetWorldPosition();

	//自弾の座標
	posB = worldTransform_->GetWorldPosition();

	if (Collision::CircleCollision(posB, posA,playerWidth,playerWidth)) {
		//敵キャラの衝突時コールバックを呼び出す
		player_->OnCollision();
		//自弾の衝突時コールバックを呼び出す
		CollTackle();
	}
}

int Enemy::ReturnOnColl()
{
	if (EnemyHp <= EnemyHpEnd ) {
		return 1;
	}
	return 0;
}

void Enemy::CollTackle()
{
	isTackleDead_ = true;
}
