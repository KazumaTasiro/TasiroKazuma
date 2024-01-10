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

	worldTransform_->wtf.scale= worldSc;

	particle = new ParticleManager();
	particle->Initialize();
	particle->LoadTexture("LockOnParticle.png");
}

void PlayerBullet::Update()
{
	//velocity_ = { 0,0,5 };
	Vector3 vec = velocity_;
	vec*= speed;


	//座標を移動させる(1フレーム文の移動量を足しこむ)
	worldTransform_->wtf.position += vec;

	AddParticle();

	worldTransform_->Update();
	//時間経過でデス
	if (--deathTimer_ <= deathTimerEnd_ ) {
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

void PlayerBullet::AddParticle()
{
		//パーティクル範囲
	for ( int i = 0; i < 8; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = rnd_posS;
		Vector3 pos = worldTransform_->wtf.position;
		pos.x += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = rnd_velS;
		Vector3 vel{};
		vel.x = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = rnd_accS;
		Vector3 acc{};
		acc.x = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		particle->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,2);
		//particle->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,two);
		particle->Update();
	}
}

void PlayerBullet::ParticleDraw()
{
	particle->Draw();
}
