#include "Enemy.h"

void Enemy::Initialize(XMFLOAT3 EnemyPos)
{
	//引数として受け取ったデータをメンバ変数に記録する
	//ワールド変換の初期化
	worldTransform_= Object3d::Create();;
	worldTransform_->SetPosition(EnemyPos);

	model_ = Model::LoadFormOBJ("cube");

	worldTransform_->SetModel(model_);

	worldTransform_->SetScale({ 3,3,3 });
}

void Enemy::Update()
{
	//キャラクター移動処理
	Move();

	//Fire();
	if (EnemyHp <= 0) {
		isDead_ = true;
	}

	/*debugText_->SetPos(50, 70);
	debugText_->Printf("x:%f,y:%f,z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);*/
	////弾更新
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets2_) {
	//	bullet->Update();
	//}
}

void Enemy::Move()
{
	switch (phase_)
	{
	case Enemy::Phase::Approch:
	default:
		Approch();
		break;
	case Enemy::Phase::Leave:
		Leave();
		break;
	}

	//Approch();
	worldTransform_->Update();
}

void Enemy::Fire()
{
	//弾の速度
	const float kBulletSpeed = 0.1f;
	assert(player_);
	XMFLOAT3 velocity(0, 0, 0);

	XMFLOAT3 PlayerPos = player_->GetWorldPosition();
	XMFLOAT3 EnemyPos = GetWorldPosition();
	velocity.x = PlayerPos.x - EnemyPos.x;
	velocity.y = PlayerPos.y - EnemyPos.y;
	velocity.z = PlayerPos.z - EnemyPos.z;
	Vec3Normalize(&velocity, &velocity);
	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(worldTransform_->position, velocity);

	//弾を登録する
	//gameScene_->AddEnemyBullet(newBullet);
}

XMFLOAT3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	XMFLOAT3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_->GetmMatWorld().r[3].m128_f32[0];
	worldPos.y = worldTransform_->GetmMatWorld().r[3].m128_f32[1];
	worldPos.z = worldTransform_->GetmMatWorld().r[3].m128_f32[2];

	return worldPos;
}

void Enemy::Draw()
{
	worldTransform_->Draw();
}

void Enemy::Approch()
{
	XMFLOAT3 pos = worldTransform_->GetPosition();
	//移動（ベクトルを加算）
	worldTransform_->SetPosition({ pos.x + ApprochMove.x,pos.y + ApprochMove.y,pos.z + ApprochMove.z });
	//if (worldTransform_.translation_.z < 0.0f) {
	//	phase_ = Phase::Leave;
	//}

	if (worldTransform_->GetPosition().z < 0.0f) {
		worldTransform_->position.z = 30.0f;
	}
	time -= 1;

	if (time <= 0) {
		Fire();
		//発射タイマーを初期化
		time = kFireInterval;
	}
}

void Enemy::Leave()
{
	XMFLOAT3 pos = worldTransform_->GetPosition();
	//移動（ベクトルを加算）
	worldTransform_->SetPosition({ pos.x + LeaveMove.x,pos.y + LeaveMove.y,pos.z + LeaveMove.z });
}

int Enemy::Vec3Normalize(XMFLOAT3* pOut, XMFLOAT3* pV)
{
	double len;
	double x, y, z;

	x = (double)(pV->x);
	y = (double)(pV->y);
	z = (double)(pV->z);
	len = sqrt(x * x + y * y + z * z);

	if (len < (1e-6)) return 0;

	len = 1.0 / len;
	x *= len;
	y *= len;
	z *= len;

	pOut->x = (float)x;
	pOut->y = (float)y;
	pOut->z = (float)z;

	return 1;
}

void Enemy::OnCollision()
{
	EnemyHp--;
}
