#include "EnemyBulletAttack.h"
#include "Enemy.h"
#include "Player.h"

void EnemyBulletAttack::Initialize(Model* bulletModel)
{
	assert(bulletModel);
	enemyBulletModel_ = bulletModel;
}

void EnemyBulletAttack::Update(Enemy* enemy)
{
	enemy_ = enemy;

	EnemyBullets_.remove_if([ ] (std::unique_ptr<EnemyBullet>& enemyBullet)
 {
	 return enemyBullet->IsDead();
		});

		//弾更新
	for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
	{
		enemyBullet->Update();
	}
}

void EnemyBulletAttack::Draw()
{
	//弾更新
	for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
	{
		enemyBullet->Draw();
	}
}

void EnemyBulletAttack::Move()
{
	if ( enemy_->GetWorldPosition().x >= 5 || enemy_->GetWorldPosition().x <= -5 )
	{
		move = -move;
	}
	enemy_->SetEnemyPos({ enemy_->GetWorldPosition().x + move,enemy_->GetWorldPosition().y,enemy_->GetWorldPosition().z	});
}

void EnemyBulletAttack::Attck()
{
	velocity_ = player_->GetWorldPosition() - enemy_->GetWorldPosition();
	velocity_.nomalize();
	velocity_ *= verocitySpeed;
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
	newEnemyBullet->Initialize(enemy_->GetWorldPosition(),velocity_,enemyBulletModel_);
	//弾を発射する
	EnemyBullets_.push_back(std::move(newEnemyBullet));
}

void EnemyBulletAttack::OnColl()
{
	//判定対象AとBの座標
	Vector3 posA,posB;

	player_ = enemy_->GetPlayer();

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
}
