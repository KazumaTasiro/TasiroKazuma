#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	model_ = model;

	//ワールドトランスフォームの初期化
	worldTransform_ = Object3d::Create();

	//引数で受け取った初期座標をセット
	worldTransform_->wtf.position=(position);

	//引数で受け取った速度をメンバ関数に代入
	velocity_ = velocity;

	//モデルをセットする
	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale={ 1,1,1 };
}

void PlayerBullet::Update()
{
	//velocity_ = { 0,0,5 };
	Vector3 vec = velocity_;
	vec*= speed;


	//座標を移動させる(1フレーム文の移動量を足しこむ)
	worldTransform_->wtf.position += vec;

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

Vector3 PlayerBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_->wtf.matWorld.m[3][0];
	worldPos.y = worldTransform_->wtf.matWorld.m[3][1];
	worldPos.z = worldTransform_->wtf.matWorld.m[3][2];

	return worldPos;
}
