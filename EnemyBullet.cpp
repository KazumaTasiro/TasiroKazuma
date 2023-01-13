#include "EnemyBullet.h"

void EnemyBullet::Initialize( const XMFLOAT3& position, const XMFLOAT3& velocity)
{

	model_ = Model::LoadFormOBJ("EnemyBullet");
	//テクスチャ読み込み
	/*textureHandle_ = TextureManager::Load("EnemyBullet.jpg");*/

	//ワールドトランスフォームの初期化
	worldTransform_ = Object3d::Create();

	//引数で受け取った初期座標をセット
	worldTransform_->position = position;
	worldTransform_->SetModel(model_);
	//引数で受け取った速度をメンバ関数に代入
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	XMFLOAT3 pos = worldTransform_->GetPosition();
	//座標を移動させる(1フレーム文の移動量を足しこむ)
	pos.x += velocity_.x;
	pos.y += velocity_.y;
	pos.z += velocity_.z;

	worldTransform_->Update();
	//時間経過でデス
	if (--deathTimer_ <= 0) {
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

XMFLOAT3 EnemyBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	XMFLOAT3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_->GetmMatWorld().r[3].m128_f32[0];
	worldPos.y = worldTransform_->GetmMatWorld().r[3].m128_f32[0];
	worldPos.z = worldTransform_->GetmMatWorld().r[3].m128_f32[0];

	return worldPos;
}
