#include "Enemy.h"

void Enemy::Initialize(Vector3 EnemyPos)
{
	//引数として受け取ったデータをメンバ変数に記録する
	//ワールド変換の初期化
	worldTransform_ = Object3d::Create();;
	worldTransform_->wtf.position = EnemyPos;

	model_ = Model::LoadFormOBJ("cubeObj");

	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = { 3,3,3 };
}

void Enemy::Update()
{
	//キャラクター移動処理
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
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
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
