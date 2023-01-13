#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const XMFLOAT3& position, const XMFLOAT3& velocity)
{
	assert(model);

	model_ = model;

	//ワールドトランスフォームの初期化
	worldTransform_= Object3d::Create();

	//引数で受け取った初期座標をセット
	worldTransform_->SetPosition(position);

	//引数で受け取った速度をメンバ関数に代入
	velocity_ = velocity;

	//モデルをセットする
	worldTransform_->SetModel(model_);

	worldTransform_->SetScale({ 1,1,1 });
}

void PlayerBullet::Update()
{
	velocity_ = { 0,0,5 };
	XMFLOAT3 position;
	position = worldTransform_->GetPosition();
	position.x += velocity_.x;
	position.y += velocity_.y;
	position.z += velocity_.z;
	//座標を移動させる(1フレーム文の移動量を足しこむ)
	worldTransform_->SetPosition(position);

	worldTransform_->Update();
	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	worldTransform_->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

XMFLOAT3 PlayerBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	XMFLOAT3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_->GetmMatWorld().r[3].m128_f32[0];
	worldPos.y = worldTransform_->GetmMatWorld().r[3].m128_f32[1];
	worldPos.z = worldTransform_->GetmMatWorld().r[3].m128_f32[2];

	return worldPos;
}
