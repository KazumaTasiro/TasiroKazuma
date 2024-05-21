#include "EnemyBullet.h"

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity ,Model* bulletModel)
{

	model_ = bulletModel;

	//ワールドトランスフォームの初期化
	worldTransform_ = Object3d::Create();

	//引数で受け取った初期座標をセット
	worldTransform_->wtf.position = position;
	worldTransform_->SetModel(model_);
	//引数で受け取った速度をメンバ関数に代入
	velocity_ = velocity;

}

void EnemyBullet::Update()
{
	Vector3 pos = velocity_;
	//座標を移動させる(1フレーム文の移動量を足しこむ)

	worldTransform_->wtf.position += velocity_;
	worldTransform_->Update();
	deathTimer_--;
	//時間経過でデス
	if (deathTimer_ <= deathTimerEnd_ ) {
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
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}

void EnemyBullet::VecRot()
{
	//進行方向に見た目の回転を合わせる
	worldTransform_->wtf.rotation.y = std::atan2(velocity_.x, velocity_.z);
	Vector3 temp = velocity_;
	temp.y = 0.0f;
	worldTransform_->wtf.rotation.x = std::atan2(-velocity_.y, temp.length());
}
