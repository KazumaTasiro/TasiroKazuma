#include "Boss.h"
#include "Player.h"

Boss::Boss()
{
}

Boss::~Boss()
{

}

void Boss::Initialize(Model* enemyBulletModel,Model* enemyReticleModel)
{
	assert(enemyBulletModel);
	assert(enemyReticleModel);
	model_ = Model::LoadFormOBJ("Sakaban");
	enemyBulletModel_ = enemyBulletModel;
	enemyReticleModel_ = enemyReticleModel;
	input_ = Input::GetInstance();
	worldTransform_ = Object3d::Create();;
	worldTransform_->wtf.position = worldPs;
	worldTransform_->SetModel(model_);
	worldTransform_->wtf.scale = worldSc;
	worldTransform_->wtf.rotation = worldRt;
	EnemyMoveSpline0 = EnemyMoveSpline0Ris;

	worldTransformReticle_ = Object3d::Create();
	worldTransformReticle_->wtf.position = worldTransform_->wtf.position;
	worldTransformReticle_->SetModel(enemyReticleModel_);
	worldTransformReticle_->wtf.scale = ReticleSc;
	spline = new SplinePosition(worldTransform_->wtf.position,EnemyMoveSpline1,EnemyMoveSpline2,EnemyMoveSpline0);

	particleMana_ = new ParticleManager();
	particleMana_->Initialize();
	particleMana_->LoadTexture("EnemyDamageParticle.png");
}

void Boss::Update(Player* player)
{


	assert(player);
	if ( fireCount >= fireCountMax )
	{
		randBossAttackNmb = rand() % 2;
		fireCount = 0;
	}
	this->player_ = player;
	worldTransformReticle_->wtf.position = worldTransform_->wtf.position;
	worldTransformReticle_->Update();

	spline->Update(time_);
	//キャラクター移動処理
	Move();

	//worldTransformReticle_->wtf.position.z = 0;
	if ( isDead_ )
	{
		for ( std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_ )
		{
//自弾の衝突時コールバックを呼び出す
			enemyBullets->OnCollision();
		}
	}
	//デスフラグの立った弾を削除
	EnemyLockBullets_.remove_if([ ] (std::unique_ptr<LockOnBullet>& bullet)
 {
	 return bullet->IsDead();
		});
	//弾更新
	for ( std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_ )
	{
		bullet->Update(GetWorldPosition());
	}
	EnemyBullets_.remove_if([ ] (std::unique_ptr<EnemyBullet>& enemyBullet)
 {
	 return enemyBullet->IsDead();
		});

	if ( EnemyHp <= 0 )
	{
		isDead_ = true;
	}
	particleMana_->Update();
	OnColl();
}

void Boss::Move()
{
	if ( DemoEnemyMove == false )
	{
		worldTransform_->wtf.position = spline->NowPos;
		if ( spline->NowPos.x == EnemyMoveSpline0.x && spline->NowPos.y == EnemyMoveSpline0.y && spline->NowPos.z == EnemyMoveSpline0.z )
		{
			DemoEnemyMove = true;
		}
	}
	if ( DemoEnemyMove == true )
	{
		if ( worldTransform_->wtf.position.x >= 5 || worldTransform_->wtf.position.x <= -5 )
		{
			move = -move;
		}
		worldTransform_->wtf.position = { worldTransform_->wtf.position.x + move,worldTransform_->wtf.position.y,worldTransform_->wtf.position.z };
		Fire();
		if ( input_->ReleaseMouse(1) )
		{
			if ( lockOn == true )
			{
//弾を生成し、初期化
				std::unique_ptr<LockOnBullet> newBullet = std::make_unique<LockOnBullet>();
				newBullet->Initialize(enemyBulletModel_,player_->GetWorldPosition());
				//弾を発射する
				EnemyLockBullets_.push_back(std::move(newBullet));
				lockOn = false;
			}
		}
	}
	worldTransform_->Update();
}

Vector3 Boss::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}

void Boss::Draw()
{
	//弾更新
	for ( std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_ )
	{
		bullet->Draw();
	}
	//弾更新
	for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
	{
		enemyBullet->Draw();
	}
	if ( isDead_ == false )
	{
		worldTransform_->Draw();

		if ( lockOn )
		{
			worldTransformReticle_->Draw();
		}
	}
}

void Boss::ParticleDraw()
{
	for ( std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_ )
	{
		bullet->ParticleDraw();
	}
	particleMana_->Draw();
}

void Boss::LockOnTrue()
{
	lockOn = true;
}

void Boss::OnColl()
{
	if ( isDead_ == false )
	{
		if ( DemoEnemyMove )
		{
//判定対象AとBの座標
			Vector3 posA,posB;

			for ( std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_ )
			{
		//敵キャラも座標
				posA = GetWorldPosition();

				//自弾の座標
				posB = bullet->GetWorldPosition();

				if ( Collision::CircleCollision(posB,posA,BossColl,BossColl) )
				{
		//敵キャラの衝突時コールバックを呼び出す
					OnCollision();
					//自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
				}
			}
			for ( std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_ )
			{
		//敵キャラも座標
				posA = player_->GetWorldPosition();

				//自弾の座標
				posB = enemyBullets->GetWorldPosition();

				if ( randBossAttackNmb == 0 )
				{
					if ( Collision::CircleCollision(posB,posA,AttackSm,AttackSm) )
					{
		//プレイヤーの衝突時コールバックを呼び出す
						player_->OnCollision();
						//自弾の衝突時コールバックを呼び出す
						enemyBullets->OnCollision();
					}
				}
				else if ( randBossAttackNmb == 1 )
				{
					if ( Collision::CircleCollision(posB,posA,AttackBig,AttackBig) )
					{
		//プレイヤーの衝突時コールバックを呼び出す
						player_->OnCollision();
						//自弾の衝突時コールバックを呼び出す
						enemyBullets->OnCollision();
					}
				}
			}
		}
	}
}

void Boss::OnCollision()
{
	EnemyHp--;
	DamageParticle();
}

void Boss::Reset()
{
	isDead_ = false;
	DemoEnemyMove = false;
	lockOn = false;
	worldTransform_->wtf.position = PosRis;
	EnemyHp = 10;
	for ( std::unique_ptr<LockOnBullet>& bullet : EnemyLockBullets_ )
	{
//自弾の衝突時コールバックを呼び出す
		bullet->OnCollision();
	}
	for ( std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_ )
	{
//自弾の衝突時コールバックを呼び出す
		enemyBullets->OnCollision();
	}
	spline->Reset();
}


void Boss::Fire()
{
	fireTime--;
	if ( fireTime <= 0 )
	{
		if ( randBossAttackNmb == 0 )
		{
			fireFlag = true;
			fireTime = fireTimeRisSm;
		}
		else if ( randBossAttackNmb == 1 )
		{
			fireFlag = true;
			fireTime = fireTimeRisBig;
		}

	}
	if ( fireFlag )
	{
		if ( randBossAttackNmb == 0 )
		{
			velocity_ = player_->GetWorldPosition() - worldTransform_->wtf.position;
			velocity_.nomalize();
			velocity_ *= verocitySpeed;
			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
			newEnemyBullet->Initialize(worldTransform_->wtf.position,velocity_,enemyBulletModel_);
			//弾を発射する
			EnemyBullets_.push_back(std::move(newEnemyBullet));
			fireFlag = false;
			fireCount += fireCountSm;
		}
		if ( randBossAttackNmb == 1 )
		{
			velocity_ = player_->GetWorldPosition() - worldTransform_->wtf.position;
			velocity_.nomalize();
			velocity_ *= verocitySpeed;
			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
			newEnemyBullet->Initialize(worldTransform_->wtf.position,velocity_,enemyBulletModel_);
			newEnemyBullet->SetSize(BulletSize);
			//弾を発射する
			EnemyBullets_.push_back(std::move(newEnemyBullet));
			fireFlag = false;
			fireCount += fireCountBig;
		}
	}
	//弾更新
	for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
	{
		enemyBullet->Update();
	}
}

void Boss::DamageParticle()
{
			//パーティクル範囲
	for ( int i = 0; i < 100; i++ )
	{
//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 0.0f;
		Vector3 pos = worldTransform_->wtf.position;
		pos.x += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 6.0f;
		Vector3 vel{};
		vel.x = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.0f;
		Vector3 acc{};
		acc.x = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		particleMana_->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,1);
		//particleMana_->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,two);
		particleMana_->Update();
	}
}
