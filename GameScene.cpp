#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxcomon,Input*input_)
{
	assert(dxcomon);
	dxCommon_ = dxcomon;
	this->input = input_;
	////DirectX初期化処理　ここから
	//dxCommon_->Initialize(winApp_);
	//スプライト共通部分の初期化
	spritecommon = new SpriteCommon;
	spritecommon->Initialize(dxCommon_);
	spritecommon->LoadTexture(0, "Title.png");
	spritecommon->LoadTexture(1, "GameClear.png");
	spritecommon->LoadTexture(2, "GameOver.png");
	spritecommon->LoadTexture(3, "sousa.png");
	spritecommon->LoadTexture(4, "SPACE.png");
	spritecommon->LoadTexture(5, "goTitle.png");
	spritecommon->LoadTexture(6, "Move.png");
	spritecommon->LoadTexture(7, "Reticle.png");


	title = new Sprite();
	title->Initialize(spritecommon, 0);
	title->SetPozition({ 150, 150 });

	gameClear = new Sprite();
	gameClear->Initialize(spritecommon, 1);
	gameClear->SetPozition({ 150, 100 });

	gameOver = new Sprite();
	gameOver->Initialize(spritecommon, 2);
	gameOver->SetPozition({ 280, 100 });

	attackMove = new Sprite();
	attackMove->Initialize(spritecommon, 3);
	attackMove->SetPozition({ 400, 400 });

	attackTex = new Sprite();
	attackTex->Initialize(spritecommon, 4);
	attackTex->SetPozition({ 710, 540 });

	goTitle = new Sprite();
	goTitle->Initialize(spritecommon, 5);
	goTitle->SetPozition({ 150, 400 });

	moveTex = new Sprite();
	moveTex->Initialize(spritecommon, 6);
	moveTex->SetSize({ 600,300 });
	moveTex->SetPozition({ 100,500 });

	audio = new Audio();
	audio->Initialize();

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(spritecommon,input);

	skydome = new Skydome();
	skydome->Initalize();

	//SoundData soundData1 = SoundLoadWave("Resources/se_amd06.wav");
	/*audio->LoadWave("attack.wav");*/

	//SoundPlayWave(xAudio2.Get(), soundData1);

	LoadEnemyPopData();
}

void GameScene::Update()
{
	//デスフラグの立った弾を削除
	bullets2_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//デスフラグの立った弾を削除
	enemy_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});

	switch (phase_)
	{
	case GameScene::Phase::TITLE:
		if (input->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::GAME;
		}

		break;
	case GameScene::Phase::GAME:
		player_->Update();
		UpdateEnemyPopCommands();

		//if (input.TriggerKey(DIK_SPACE)) {
		//	audio->PlayWave("attack.wav");
		//}

		for (std::unique_ptr<Enemy>& enemy : enemy_) {
			enemy->SetGameScene(this);
			enemy->Update();
		}
		if (EnemyDeadCount >= 11) {
			phase_ = Phase::CLEAR;
			PhaseReset();
			EnemyPopComandReset();
		}
		if (PlayerDead) {
			phase_ = Phase::GAMEOVER;
			PhaseReset();
			EnemyPopComandReset();
			for (std::unique_ptr<EnemyBullet>& bullet : bullets2_) {
				bullet->OnCollision();
			}
			for (std::unique_ptr<Enemy>& enemy : enemy_) {
				enemy->OnCollision();
			}

		}

		break;
	case GameScene::Phase::CLEAR:
		if (input->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::TITLE;
		}
		for (std::unique_ptr<EnemyBullet>& bullet : bullets2_) {
			bullet->OnCollision();
		}
		for (std::unique_ptr<Enemy>& enemy : enemy_) {
			enemy->OnCollision();
		}
		break;
	case GameScene::Phase::GAMEOVER:
		if (input->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::TITLE;
		}
		for (std::unique_ptr<EnemyBullet>& bullet : bullets2_) {
			bullet->OnCollision();
		}
		for (std::unique_ptr<Enemy>& enemy : enemy_) {
			enemy->OnCollision();
		}
		break;
	}


	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets2_) {
		bullet->Update();
	}


	CheckAllCollisions();
	/*debugText_->SetPos(50, 70);
	debugText_->Printf("x:%f,y:%f,z:%f", eee.x, eee.y, eee.z);*/
}

void GameScene::Draw()
{


	Object3d::PreDraw(dxCommon_->GetCommandList());

	switch (phase_)
	{
	case GameScene::Phase::TITLE:

		break;
	case GameScene::Phase::GAME:
		player_->Draw();


		for (std::unique_ptr<Enemy>& enemy : enemy_) {
			//敵キャラの描画
			enemy->Draw();
		}



		//弾描画
		for (std::unique_ptr<EnemyBullet>& bullet : bullets2_) {
			bullet->Draw();
		}
		break;
	case GameScene::Phase::CLEAR:

		break;
	case GameScene::Phase::GAMEOVER:
		break;
	}

	skydome->Draw();

	Object3d::PostDraw();

	spritecommon->SpritePreDraw();

	switch (phase_)
	{
	case GameScene::Phase::TITLE:
		title->Draw();
		attackMove->Draw();
		attackTex->Draw();
		moveTex->Draw();
		break;
	case GameScene::Phase::GAME:
		player_->DrawUI();
		break;
	case GameScene::Phase::CLEAR:
		gameClear->Draw();
		goTitle->Draw();
		break;
	case GameScene::Phase::GAMEOVER:
		gameOver->Draw();
		goTitle->Draw();
		break;
	}

	spritecommon->SpritePostDraw();
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	XMFLOAT3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = GetBullets();

#pragma region 自キャラと敵弾の当たり判定
#pragma endregion

	//自キャラも座標
	posA = player_->GetWorldPosition();

	//敵キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		float lol = { (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) };

		float radius = { (1 + 1) * (1 + 1) };

		if (lol <= radius) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();

			PlayerDead = true;
		}
	}

#pragma region 自弾と敵キャラの当たり判定
#pragma endregion
	for (std::unique_ptr<Enemy>& enemy : enemy_) {



		//自弾と敵すべての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {

			//敵キャラも座標
			posA = enemy->GetWorldPosition();

			//自弾の座標
			posB = bullet->GetWorldPosition();

			float lol = { (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) };

			float radius = { (3 + 3) * (3 + 3) };

  			if (lol <= radius) {
				//敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();

				EnemyDeadCount++;
			}
		}
	}

#pragma region 自弾と敵弾の当たり判定
#pragma endregion
	////自弾と敵弾すべての当たり判定
	//for (const std::unique_ptr<EnemyBullet>& bullet2 : enemyBullets) {
	//	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
	//		//自弾の座標
	//		posA = bullet2->GetWorldPosition();
	//		//敵弾の座標
	//		posB = bullet->GetWorldPosition();

	//		float lol = { (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) };

	//		float radius = { (1 + 1) * (1 + 1) };

	//		if (lol <= radius) {
	//			//自キャラの衝突時コールバックを呼び出す
	//			bullet2->OnCollision();
	//			//自弾の衝突時コールバックを呼び出す
	//			bullet->OnCollision();
	//		}
	//	}
	//}
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet)
{
	//リストに登録する
	bullets2_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルを内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands()
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
			ExistenceEnemy(XMFLOAT3(x, y, z));
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

void GameScene::ExistenceEnemy(const XMFLOAT3& EnemyPos)
{
	//敵キャラの生成
//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize( EnemyPos);

	//敵キャラにアドレスを渡す
	newEnemy->SetPlayer(player_);

	//リストに登録する
	enemy_.push_back(std::move(newEnemy));
}

void GameScene::PhaseReset()
{
	//自キャラの初期化
	player_->Reset();


	EnemyDeadCount = 0;

	PlayerDead = false;
}

void GameScene::EnemyPopComandReset()
{
	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();
}
