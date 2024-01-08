#include"EnemyManager.h"
#include "Player.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Initialize(SpriteCommon* spriteCommon,Camera* camera,ParticleManager* particle)
{

	assert(camera);
	assert(particle);
	assert(spriteCommon);
	camera_ = camera;
	spriteCommon_ = spriteCommon;
	input_ = Input::GetInstance();
	enemyModel_ = Model::LoadFormOBJ("Sakaban");
	enemyBulletModel_ = Model::LoadFormOBJ("EnemyBullet");
	enemyReticleModel_ = Model::LoadFormOBJ("Reticle");
	LoadEnemyPopData();

	spriteRight = new Sprite();
	spriteLeft = new Sprite();
	spriteRight->Initialize(spriteCommon_,22);
	spriteLeft->Initialize(spriteCommon_,21);
	//パーティクル生成
	enemyDeadParticle = particle;
	/*enemyDeadParticle->Update();*/

	boss = new Boss();
	boss->Initialize(enemyBulletModel_,enemyReticleModel_);

}

void EnemyManager::Update()
{
	clearTime--;
	int count = enemyDeath;
	if ( count == 3 )
	{
		enemyDeath = 0;
		clearCount++;
		clearTime = clearTimeRis;
		EnemyPopComandReset();
	}
	UpdateEnemyPopCommands();

	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{
//敵キャラの描画
		if ( enemy->IsDead() )
		{
			EnemyDeadParticle(enemy->GetWorldPosition());
		}
	}
	enemyDeadParticle->Update();
	//デスフラグの立った弾を削除
	enemy_.remove_if([ ] (std::unique_ptr<Enemy>& enemy)
 {
	 return enemy->IsDead();
		});
	enemy_.remove_if([ ] (std::unique_ptr<Enemy>& enemy)
 {
	 return enemy->IsTackleDead();
		});
	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{
		enemyDeath += enemy->ReturnOnColl();
		enemy->SetGameScene(gameScene_);
		enemy->Update(player_);
	}
}

void EnemyManager::BossUpdate()
{
	boss->Update(player_);
	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{
		enemy->OnCollision();
	}
	enemy_.remove_if([ ] (std::unique_ptr<Enemy>& enemy)
 {
	 return enemy->IsTackleDead();
		});
	enemyDeadParticle->Update();

	if ( boss->IsDead() )
	{
		if ( EffectTime == EffectTimeRis )
		{
			BossDeadParticle(boss->GetWorldPosition());
		}
		EffectTime--;

		if ( EffectTime <= EffectTimeRisEnd )
		{
			EfectEnd = true;
		}
	}
}

void EnemyManager::Draw()
{
	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{
//敵キャラの描画
		enemy->Draw();
	}

}

void EnemyManager::BossDraw()
{
	boss->Draw();
}

void EnemyManager::DrawUI()
{
	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{
		enemy->DrawUI();
	}
}


bool EnemyManager::IsAllEnemyDead()
{
	return false;
}

void EnemyManager::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop2.csv");
	assert(file.is_open());

	//ファイルを内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void EnemyManager::EnemyPopComandReset()
{
	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();
}

void EnemyManager::UpdateEnemyPopCommands()
{
	if ( waitflag )
	{
		waitTimer--;
		if ( waitTimer <= 0 )
		{
			//待機完了
			waitflag = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while ( getline(enemyPopCommands,line) )
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream,word,',');

		//"//"から始まる行はコメント
		if ( word.find("//") == 0 )
		{
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if ( word.find("POP") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());
			//y座標
			getline(line_stream,word,',');
			float y = ( float ) std::atof(word.c_str());
			//z座標
			getline(line_stream,word,',');
			float z = ( float ) std::atof(word.c_str());
			//敵を発生させる
			ExistenceEnemy(Vector3(x,y,z));
		}

		//WAITコマンド
		else if ( word.find("WAIT") == 0 )
		{
			getline(line_stream,word,',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			waitflag = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}

	}
}

void EnemyManager::ExistenceEnemy(const Vector3& EnemyPos)
{

	randEnemyNmb = rand() % two;
	randEnemyRoot = rand() % two;
	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(EnemyPos,spriteCommon_,enemyModel_,enemyBulletModel_,enemyReticleModel_,randEnemyNmb,randEnemyRoot);

	//リストに登録する
	enemy_.push_back(std::move(newEnemy));
}

void EnemyManager::EnemyCollision(Player* player)
{
	//判定対象AとBの座標
	Vector3 posA,posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();



#pragma region 自キャラと敵弾の当たり判定
#pragma endregion

	//自キャラも座標
	posA = player->GetWorldPosition();


#pragma region 自弾と敵キャラの当たり判定
#pragma endregion
	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{
//自弾と敵すべての当たり判定
		for ( const std::unique_ptr<PlayerBullet>& bullet : playerBullets )
		{

//敵キャラも座標
			posA = enemy->GetWorldPosition();

			//自弾の座標
			posB = bullet->GetWorldPosition();

			if ( Collision::CircleCollision(posB,posA,enemyWide,enemyWide) )
			{
//敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();

			}
		}
	}
	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{

//敵キャラも座標
		posA = enemy->GetWorldPosition();

		Vector2 posR;
		//レティクルの座標
		posR = player->GetReticlePos();

		if ( Collision::RaySphere(RayPos,posA,enemyWide,player->GetFarNear()) )
		{
			if ( enemy->GetMoveFlag() == true )
			{
				if ( input_->PushMouse(1) )
				{
//敵キャラのロックオンコールバックを呼び出す
					enemy->LockOnTrue();
				}
			}
		}
	}

	if ( clearNum <= clearCount )
	{
		if ( boss->IsDead() == false )
		{
			//自弾とボスの当たり判定
			for ( const std::unique_ptr<PlayerBullet>& bullet : playerBullets )
			{

				//敵キャラも座標
				posA = boss->GetWorldPosition();

				//自弾の座標
				posB = bullet->GetWorldPosition();

				if ( Collision::CircleCollision(posB,posA,bossWide,bossWide) )
				{
					//敵キャラの衝突時コールバックを呼び出す
					boss->OnCollision();
					//自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();

				}
			}
		}
		if ( boss->IsDead() )
		{
			for ( const std::unique_ptr<PlayerBullet>& bullet : playerBullets )
			{
				bullet->OnCollision();
			}
		}
		//敵キャラも座標
		posA = boss->GetWorldPosition();

		Vector2 posR;
		//自弾の座標
		posR = player->GetReticlePos();

		if ( Collision::RaySphere(RayPos,posA,bossWide,player->GetFarNear()) )
		{
			if ( input_->PushMouse(one) )
			{
//敵キャラの衝突時コールバックを呼び出す
				boss->LockOnTrue();
			}
		}
	}

#pragma region 自キャラと敵弾の当たり判定
#pragma endregion

	////自キャラも座標
	//posA = player_->GetWorldPosition();

	////敵キャラと敵弾すべての当たり判定
	//for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
	//	//敵弾の座標
	//	posB = bullet->GetWorldPosition();

	//	float lol = { (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) };

	//	float radius = { (1 + 1) * (1 + 1) };

	//	if (lol <= radius) {
	//		//自キャラの衝突時コールバックを呼び出す
	//		player_->OnCollision();
	//		//自弾の衝突時コールバックを呼び出す
	//		bullet->OnCollision();

	//		PlayerDead = true;
	//	}
	//}

}

void EnemyManager::EnemyReset()
{
	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{
		enemy->CollTackle();
	}
	EnemyPopComandReset();
	clearCount = clearCountRis;
	enemyDeath = clearCountRis;
	//clearNum = 0;
	clearTime = clearTimeRis;
	Update();
	EffectTime = EffectTimeRis;
	EfectEnd = false;
	boss->Reset();
	BossUpdate();

}

bool EnemyManager::Clear()
{

	if ( clearNum <= clearCount )
	{
		return true;
	}

	return false;
}

bool EnemyManager::BossClear()
{
	if ( EfectEnd )
	{
		return true;
	}
	return false;
}

void EnemyManager::EnemyDeadParticle(Vector3 EnemyPos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = rnd_posS;
		Vector3 pos = EnemyPos;
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
		enemyDeadParticle->Add(enemyParticleLife,pos,vel,acc,enemyParticleScaleStert,enemyParticleScaleEnd,one);
		enemyDeadParticle->Add(enemyParticleLife,pos,vel,acc,enemyParticleScaleStert,enemyParticleScaleEnd,two);
		enemyDeadParticle->Update();
	}
}

void EnemyManager::BossDeadParticle(Vector3 EnemyPos)
{
	//パーティクル範囲
	for ( int i = 0; i < 10; i++ )
	{
//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = rnd_posS;
		Vector3 pos = EnemyPos;
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
		enemyDeadParticle->Add(bossParticleLife,pos,vel,acc,bossParticleScaleStert,bossParticleScaleEnd,one);
		enemyDeadParticle->Add(bossParticleLife,pos,vel,acc,bossParticleScaleStert,bossParticleScaleEnd,two);
		enemyDeadParticle->Update();
	}
}

void EnemyManager::ParticleDraw()
{
	enemyDeadParticle->Draw();
	boss->ParticleDraw();
	for ( std::unique_ptr<Enemy>& enemy : enemy_ )
	{
		enemy->ParticleDraw();
	}
}

void EnemyManager::bossSeenTest()
{
	clearCount = 3;
}

void EnemyManager::ImguiUpdate()
{
	boss->ImGuiUpdate();
}
