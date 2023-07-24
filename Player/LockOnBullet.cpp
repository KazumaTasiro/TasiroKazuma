#include "LockOnBullet.h"

LockOnBullet::LockOnBullet()
{
}

LockOnBullet::~LockOnBullet()
{
}

void LockOnBullet::Initialize(Model* model, const Vector3& position)
{
	assert(model);

	model_ = model;

	//ワールドトランスフォームの初期化
	worldTransform_ = Object3d::Create();

	//引数で受け取った初期座標をセット
	worldTransform_->wtf.position = (position);



	//モデルをセットする
	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = { 1,1,1 };
}

void LockOnBullet::Update(const Vector3& enemyPos)
{
	////引数で受け取った速度をメンバ関数に代入
	//velocity_ = velocity;

	//velocity_ = { 0,0,5 };

	velocity_ = enemyPos - worldTransform_->wtf.position;

	Vector3 position;
	position.x = velocity_.x;
	position.y = velocity_.y;
	position.z = velocity_.z;

	position.nomalize();
	position *= speed;
	//座標を移動させる(1フレーム文の移動量を足しこむ)
	worldTransform_->wtf.position += (position);

	worldTransform_->Update();
	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void LockOnBullet::Draw()
{
	worldTransform_->Draw();
}

void LockOnBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 LockOnBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_->wtf.matWorld.m[3][0];
	worldPos.y = worldTransform_->wtf.matWorld.m[3][1];
	worldPos.z = worldTransform_->wtf.matWorld.m[3][2];

	return worldPos;
}
