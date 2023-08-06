#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(Vector3 EnemyPos, Input* input, SpriteCommon* sptriteCommon)
{
	assert(input);
	assert(sptriteCommon);

	//引数として受け取ったデータをメンバ変数に記録する
	//ワールド変換の初期化
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
	//デスフラグの立った弾を削除
	EnemyLockBullets_.remove_if([](std::unique_ptr<LockOnBullet>& bullet) {
		return bullet->IsDead();
		});
	//弾更新
	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		bullet->Update(GetWorldPosition());
	}
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
		if (worldTransform_->wtf.position.x >= 5 || worldTransform_->wtf.position.x <= -5) {
			move = -move;
		}
		worldTransform_->wtf.position = { worldTransform_->wtf.position.x + move,worldTransform_->wtf.position.y,worldTransform_->wtf.position.z };

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
	//判定対象AとBの座標
	Vector3 posA, posB;

	for (std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_) {
		//敵キャラも座標
		posA = GetWorldPosition();

		//自弾の座標
		posB = bullet->GetWorldPosition();

		if (Collision::CircleCollision(posB, posA, 2.0f, 2.0f)) {
			//敵キャラの衝突時コールバックを呼び出す
			OnCollision();
			//自弾の衝突時コールバックを呼び出す
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
