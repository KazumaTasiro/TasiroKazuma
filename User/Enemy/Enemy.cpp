#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(Vector3 EnemyPos,SpriteCommon* sptriteCommon,Model* model,Model* enemyBulletModel,Model* enemyReticleModel,int EnemyNmb,int EnemyRootNmb)
{
	assert(sptriteCommon);
	assert(model);
	assert(enemyBulletModel);
	assert(enemyReticleModel);

	EnemyRootNmb_ = EnemyRootNmb;


	//引数として受け取ったデータをメンバ変数に記録する
	//ワールド変換の初期化
	worldTransform_ = Object3d::Create();;
	worldTransform_->wtf.position = EnemyPos;

	model_ = model;
	enemyBulletModel_ = enemyBulletModel;
	enemyReticleModel_ = enemyReticleModel;

	worldTransformReticle_ = Object3d::Create();
	worldTransformReticle_->wtf.position = EnemyPos;
	worldTransformReticle_->SetModel(enemyReticleModel_);
	worldTransformReticle_->wtf.scale = EnemyReticleScale;

	particleMana_ = new ParticleManager();
	particleMana_->Initialize();
	particleMana_->LoadTexture("EnemyDamageParticle.png");


	spriteLock = new Sprite();
	spriteLock->Initialize(sptriteCommon,two);

	input_ = Input::GetInstance();

	enemyNmb = EnemyNmb;

	worldTransform_->SetModel(model_);
	worldTransform_->wtf.rotation = { 0,( PI / 180 ) * 180,0 };

	worldTransform_->wtf.scale = EnemyScale;
	SplineMoveInitialize();



	//worldTransform_->ShadowUse();
	shadowNmb = LightData::GetInstance()->AddCircleShadow(worldTransform_->wtf.position,circleShadowDir,circleShadowAtten,circleShadowFactorAngle);

}

void Enemy::Update(Player* player)
{

	assert(player);
	this->player_ = player;
	worldTransformReticle_->wtf.position = worldTransform_->wtf.position;
	worldTransformReticle_->Update();
	float time_ = Updatetime;
	spline->Update(time_);
	//worldTransformReticle_->wtf.position.z = 0;
	//デスフラグの立った弾を削除
	EnemyLockBullets_.remove_if([ ] (std::unique_ptr<LockOnBullet>& LockBullet)
 {
	 return LockBullet->IsDead();
		});
	EnemyBullets_.remove_if([ ] (std::unique_ptr<EnemyBullet>& enemyBullet)
 {
	 return enemyBullet->IsDead();
		});

	//キャラクター移動処理
	Move();

	spriteLock->SetPozition({ GetWorldPosition().x,GetWorldPosition().y });
	if ( EnemyHp <= 0 )
	{
		isDead_ = true;
		lightActive = false;
	}
	LightData::GetInstance()->UpdateCircleShadow(shadowNmb,worldTransform_->wtf.position,circleShadowDir,circleShadowAtten,circleShadowFactorAngle,lightActive);
	particleMana_->Update();
	OnColl();
}

void Enemy::Move()
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
		Fire();
		if ( enemyNmb == 0 )
		{
			NormalBulletMove();
		}
		if ( enemyNmb == 1 )
		{
			TackleMove();
		}
		if ( enemyNmb == 2 )
		{
			ObstacleMove();
		}

	}
	velocity_ = player_->GetWorldPosition() - worldTransform_->wtf.position;
	velocity_.nomalize();
	velocity_ *= verocitySpeed;
	//進行方向に見た目の回転を合わせる
	worldTransform_->wtf.rotation.y = std::atan2(velocity_.x,velocity_.z);
	Vector3 temp = velocity_;
	temp.y = 0.0f;
	worldTransform_->wtf.rotation.x = std::atan2(-velocity_.y,temp.length());
	worldTransform_->Update();
}

void Enemy::Fire()
{

	if ( fireTime <= 0 )
	{
		fireFlag = true;
		fireTime = fireTimeRis;

	}
	if ( fireFlag == false )
	{
		fireTime--;
		velocityTackle = player_->GetWorldPosition() - worldTransform_->wtf.position;
		velocityTackle.nomalize();
		velocityTackle *= verocitySpeed;
	}
	if ( fireFlag )
	{


		if ( enemyNmb == 0 )
		{
			NormalBulletAttck();
		}
		if ( enemyNmb == 1 )
		{

			TackleAttck();
		}
		if ( enemyNmb == 2 )
		{
			ObstacleAttack();
		}
	}
	lockOnCoolTime--;
	if ( input_->ReleaseMouse(1) )
	{
		if ( player_->retrunIsDaed() == false )
		{
			if ( lockOn == true )
			{
	//弾を生成し、初期化
				std::unique_ptr<LockOnBullet> newLockBullet = std::make_unique<LockOnBullet>();
				newLockBullet->Initialize(enemyBulletModel_,player_->GetWorldPosition());
				//弾を発射する
				EnemyLockBullets_.push_back(std::move(newLockBullet));
				lockOn = false;
			}
		}
	}
	//弾更新
	for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
	{
		enemyBullet->Update();
	}
	//弾更新
	for ( std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_ )
	{
		LockBullet->Update(GetWorldPosition());
	}
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
	if ( isDead_ == false )
	{
//弾更新
		for ( std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_ )
		{
			LockBullet->Draw();
		}
		//弾更新
		for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
		{
			enemyBullet->Draw();
		}
		worldTransform_->Draw();
		if ( player_->retrunIsDaed() == false )
		{
			if ( lockOn )
			{
				worldTransformReticle_->Draw();
			}
		}
	}
}


void Enemy::DrawUI()
{
	//if (lockOn) {
	//	spriteLock->Draw();
	//}
}

void Enemy::ParticleDraw()
{
	for ( std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_ )
	{
		LockBullet->ParticleDraw();
	}
	particleMana_->Draw();
}

void Enemy::OnCollision()
{
	EnemyHp--;
	DamageParticle();

}

void Enemy::LockOnTrue()
{
	if ( lockOnCoolTime <= 0 )
	{
		lockOnCoolTime = lockOnCoolTimeRe;
		lockOn = true;
	}
}

void Enemy::OnColl()
{
	//判定対象AとBの座標
	Vector3 posA,posB;

	for ( std::unique_ptr<LockOnBullet>& LockBullet : EnemyLockBullets_ )
	{
//敵キャラも座標
		posA = GetWorldPosition();

		//自弾の座標
		posB = LockBullet->GetWorldPosition();

		if ( Collision::CircleCollision(posB,posA,LockWidth,LockWidth) )
		{
//敵キャラの衝突時コールバックを呼び出す
			OnCollision();
			//自弾の衝突時コールバックを呼び出す
			LockBullet->OnCollision();
		}
	}
	for ( std::unique_ptr<EnemyBullet>& enemyBullets : EnemyBullets_ )
	{
//敵キャラも座標
		posA = player_->GetWorldPosition();

		//自弾の座標
		posB = enemyBullets->GetWorldPosition();

		if ( Collision::CircleCollision(posB,posA,playerWidth,enemyWidth_) )
		{
//敵キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			enemyBullets->OnCollision();
		}
	}
	//敵キャラも座標
	posA = player_->GetWorldPosition();

	//自弾の座標
	posB = worldTransform_->GetWorldPosition();

	if ( Collision::CircleCollision(posB,posA,playerWidth,enemyWidth_) )
	{
//敵キャラの衝突時コールバックを呼び出す
		player_->OnCollision();
		//自弾の衝突時コールバックを呼び出す
		//CollTackle();
	}
}

int Enemy::ReturnOnColl()
{
	if ( EnemyHp <= EnemyHpEnd )
	{
		return 1;
	}
	return 0;
}

void Enemy::CollTackle()
{
	isTackleDead_ = true;
}

void Enemy::DamageParticle()
{
		//パーティクル範囲
	for ( int i = 0; i < 50; i++ )
	{
//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 0.0f;
		Vector3 pos = worldTransform_->wtf.position;
		pos.x += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 1.5f;
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
		particleMana_->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,one);
		particleMana_->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,two);
		particleMana_->Update();
	}
}

void Enemy::ImGuiUpdate()
{

}

void Enemy::NormalBulletAttck()
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
}

void Enemy::NormalBulletMove()
{
	if ( worldTransform_->wtf.position.x >= 5 || worldTransform_->wtf.position.x <= -5 )
	{
		move = -move;
	}
	worldTransform_->wtf.position = { worldTransform_->wtf.position.x + move,worldTransform_->wtf.position.y,worldTransform_->wtf.position.z };
}

void Enemy::TackleAttck()
{
	worldTransform_->wtf.position += velocityTackle;

	if ( worldTransform_->wtf.position.z <= tackPosLim )
	{
		TackleReMove = true;
	}
}

void Enemy::TackleMove()
{
	if ( TackleReMove )
	{
		float time_ = Movetime;
		splineReMove->Update(time_);
		worldTransform_->wtf.position = splineReMove->NowPos;
		if ( splineReMove->NowPos.x == EnemyMoveSpline0.x && splineReMove->NowPos.y == EnemyMoveSpline0.y && splineReMove->NowPos.z == EnemyMoveSpline0.z )
		{
			TackleReMove = false;
			fireFlag = false;
		}
	}
	else
	{
		splineReMove->Reset();

	}
}

void Enemy::ObstacleAttack()
{
	obstacleFlag = true;
	fireTime = fireTimeRis * 3;

	fireFlag = false;
}

void Enemy::ObstacleMove()
{
	float vel = AbsoluteValue(player_->GetWorldPosition().x,worldTransform_->wtf.position.x);
	Vector3 vecNom = player_->GetWorldPosition() - worldTransform_->wtf.position;
	vecNom.nomalize();
	if ( vel < obstacleEnemySpeed )
	{
		vecNom.x *= obstacleEnemySpeed;
		worldTransform_->wtf.position.x += vecNom.x;
	}
	else
	{
		worldTransform_->wtf.position.x += vecNom.x;
	}
}

bool Enemy::ReturnObstacleFlag()
{
	return obstacleFlag;
}

void Enemy::ResetObstacleFlag()
{
	obstacleFlag = false;
}

float Enemy::AbsoluteValue(float nmb1,float nmb2)
{
	float ans = 0;

	ans = sqrtf(( nmb1 * nmb1 ) - ( nmb2 * nmb2 ));
	ans = sqrtf(ans);

	return  ans;
}

void Enemy::SplineMoveInitialize()
{
	if ( EnemyRootNmb_ == zero )
	{
		oneEnemyMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - sprineY2, sprineZ3 };
		oneEnemyReMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - sprineY2, sprineZ3 };
		oneEnemyReMoveSpline1 = { worldTransform_->wtf.position.x - sprineX, sprineY1 , -sprineZ1 };
		oneEnemyReMoveSpline2 = { worldTransform_->wtf.position.x - sprineX, sprineY1, sprineZ2 };
		EnemyMoveSpline0 = oneEnemyMoveSpline0;
		EnemyMoveSpline1 = oneEnemyMoveSpline1;
		EnemyMoveSpline2 = oneEnemyMoveSpline2;
		EnemyMoveSpline0 = oneEnemyMoveSpline0;
		EnemyReMoveSpline0 = oneEnemyReMoveSpline0;
		EnemyReMoveSpline1 = oneEnemyReMoveSpline1;
		EnemyReMoveSpline2 = oneEnemyReMoveSpline2;
	}
	else if ( EnemyRootNmb_ == one )
	{
		twoEnemyMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - sprineY2, sprineZ3 };
		twoEnemyReMoveSpline0 = { worldTransform_->wtf.position.x, worldTransform_->wtf.position.y - sprineY2, sprineZ3 };
		twoEnemyReMoveSpline1 = { worldTransform_->wtf.position.x + sprineX, sprineY1 , -sprineZ1 };
		twoEnemyReMoveSpline2 = { worldTransform_->wtf.position.x + sprineX, sprineY1, sprineZ2 };
		EnemyMoveSpline0 = twoEnemyMoveSpline0;
		EnemyMoveSpline1 = twoEnemyMoveSpline1;
		EnemyMoveSpline2 = twoEnemyMoveSpline2;
		EnemyMoveSpline0 = twoEnemyMoveSpline0;
		EnemyReMoveSpline0 = twoEnemyReMoveSpline0;
		EnemyReMoveSpline1 = twoEnemyReMoveSpline1;
		EnemyReMoveSpline2 = twoEnemyReMoveSpline2;
	}
	else if ( EnemyRootNmb_ == three )
	{
		DemoEnemyMove = true;
		enemyNmb = 1;
	}
	else if ( EnemyRootNmb_ == four )
	{
		DemoEnemyMove = true;
		enemyNmb = 1;
	}

	spline = new SplinePosition(worldTransform_->wtf.position,EnemyMoveSpline1,EnemyMoveSpline2,EnemyMoveSpline0);
	splineReMove = new SplinePosition(worldTransform_->wtf.position,EnemyReMoveSpline1,EnemyReMoveSpline2,EnemyReMoveSpline0);

}

void Enemy::LightShadowOff()
{
	lightActive = false;
	worldTransform_->wtf.position = enemyResetPos;
	LightData::GetInstance()->UpdateCircleShadow(shadowNmb,enemyResetPos,circleShadowDir,circleShadowAtten,circleShadowFactorAngle,lightActive);
}
