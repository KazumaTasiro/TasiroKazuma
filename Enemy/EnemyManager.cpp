#include"EnemyManager.h"
#include "Player.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize(DirectXCommon* dxCommon, Input* input, SpriteCommon* spriteCommon, Camera* camera)
{
	assert(camera);
	assert(input);
	assert(spriteCommon);
	camera_ = camera;
	spriteCommon_ = spriteCommon;
	input_ = input;
	enemyModel_ = Model::LoadFormOBJ("cubeObj");
	enemyBulletModel_ = Model::LoadFormOBJ("EnemyBullet");
	enemyReticleModel_ = Model::LoadFormOBJ("Reticle");
	LoadEnemyPopData();

	//パーティクル生成
	enemyDeadParticle = new ParticleManager();
	enemyDeadParticle->Initialize();
	enemyDeadParticle->LoadTexture("Explosion.png");
	/*enemyDeadParticle->Update();*/

	boss = new Boss();
	boss->Initialize(enemyBulletModel_, enemyReticleModel_,input);

}

void EnemyManager::Update()
{
	clearTime--;
	if (clearTime <= 0) {
		clearTime = 400;
		EnemyPopComandReset();
	}
	UpdateEnemyPopCommands();
	
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//敵キャラの描画
		if (enemy->IsDead()) {
			EnemyDeadParticle(enemy->GetWorldPosition());
		}
	}
	enemyDeadParticle->Update();
	//デスフラグの立った弾を削除
	enemy_.remove_if([](std::unique_ptr<Enemy>& enemy) {
	return enemy->IsDead();
		});
	enemy_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsTackleDead();
		});
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		clearCount += enemy->ReturnOnColl();
		enemy->SetGameScene(gameScene_);
		enemy->Update(player_);
	}
}

void EnemyManager::BossUpdate()
{
	enemyDeadParticle->Update();
	boss->Update(player_);
	if (boss->isDead()) {
		if (EffectTime == 50) {
			BossDeadParticle(boss->GetWorldPosition());
		}
		EffectTime--;
		
		if (EffectTime <= 0) {
			EfectEnd = true;
		}
	}
}

void EnemyManager::Draw()
{
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//敵キャラの描画
		enemy->Draw();
	}
	enemyDeadParticle->Draw();
}

void EnemyManager::BossDraw()
{
	boss->Draw();
	enemyDeadParticle->Draw();
}

void EnemyManager::DrawUI()
{
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
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
	if (waitflag)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			//待機完了
			waitflag = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0)
		{
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			//敵を発生させる
			ExistenceEnemy(Vector3(x, y, z));
		}

		//WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

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
	randEnemyNmb = rand() % 2;
	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(EnemyPos, input_, spriteCommon_, enemyModel_, enemyBulletModel_, enemyReticleModel_,randEnemyNmb);

	//リストに登録する
	enemy_.push_back(std::move(newEnemy));
}

void EnemyManager::EnemyCollision(Player* player)
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();



#pragma region 自キャラと敵弾の当たり判定
#pragma endregion

	//自キャラも座標
	posA = player->GetWorldPosition();


#pragma region 自弾と敵キャラの当たり判定
#pragma endregion
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//自弾と敵すべての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {

			//敵キャラも座標
			posA = enemy->GetWorldPosition();

			//自弾の座標
			posB = bullet->GetWorldPosition();

			if (Collision::CircleCollision(posB, posA, 2.0f, 2.0f)) {
				//敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();

			}
		}
	}
	for (std::unique_ptr<Enemy>& enemy : enemy_) {

		//敵キャラも座標
		posA = enemy->GetWorldPosition();

		Vector2 posR;
		//自弾の座標
		posR = player->GetReticlePos();

		if (Collision::RaySphere({ 0,0,0 }, posA, 3.0f, player->GetFarNear())) {
			if (input_->PushMouse(1)) {
				//敵キャラの衝突時コールバックを呼び出す
				enemy->LockOnTrue();
			}
		}
	}

	if (clearNum <= clearCount) {
		//自弾とボスの当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {

			//敵キャラも座標
			posA = boss->GetWorldPosition();

			//自弾の座標
			posB = bullet->GetWorldPosition();

			if (Collision::CircleCollision(posB, posA, 50.0f, 50.0f)) {
				//敵キャラの衝突時コールバックを呼び出す
				boss->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();

			}
		}
		//敵キャラも座標
		posA = boss->GetWorldPosition();

		Vector2 posR;
		//自弾の座標
		posR = player->GetReticlePos();

		if (Collision::RaySphere({ 0,0,0 }, posA, 50.0f, player->GetFarNear())) {
			if (input_->PushMouse(1)) {
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
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		enemy->OnCollision();
	}
	EnemyPopComandReset();
	clearCount = 0;
	//clearNum = 0;
	clearTime = 400;
	Update();
	EffectTime = 50;
	EfectEnd = false;
	boss->Reset();
	BossUpdate();

}

bool EnemyManager::Clear()
{

	if (clearNum <= clearCount) {
		return true;
	}

	return false;
}

bool EnemyManager::BossClear()
{
	if (EfectEnd) {
		return true;
	}
	return false;
}

void EnemyManager::EnemyDeadParticle(Vector3 EnemyPos)
{
	//パーティクル範囲
	for (int i = 0; i < 5; i++) {
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 5.0f;
		Vector3 pos = EnemyPos;
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.0f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.0000f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		enemyDeadParticle->Add(30, pos, vel, acc, 0.0f, 20.0f, 1);
		enemyDeadParticle->Add(30, pos, vel, acc, 0.0f, 20.0f, 2);
		enemyDeadParticle->Update();
	}
}

void EnemyManager::BossDeadParticle(Vector3 EnemyPos)
{
	//パーティクル範囲
	for (int i = 0; i < 10; i++) {
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 5.0f;
		Vector3 pos = EnemyPos;
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.0f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.0000f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		enemyDeadParticle->Add(40, pos, vel, acc, 0.0f, 50.0f, 1);
		enemyDeadParticle->Add(40, pos, vel, acc, 0.0f, 50.0f, 2);
		enemyDeadParticle->Update();
	}
}
