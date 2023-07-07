#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete player_;
	//delete model1;
}
void GameScene::Initialize(WinApp* winApp, DirectXCommon* dxcomon, Input* input_)
{
	assert(dxcomon);
	assert(input_);

	input = input_;

	winApp_ = winApp;

	dxCommon_ = dxcomon;

	audio = new Audio();
	audio->Initialize();
	
	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	camera->SetEye({ 0,0,-100 });
	Object3d::SetCamera(camera);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);
	spriteCommon->LoadTexture(0, "Reticle.png");

	skydome = new Skydome();
	skydome->Initalize();

	//Object3dFbx::SetDevice(dxCommon_->GetDevice());
	//Object3dFbx::SetCamera(camera);
	//Object3dFbx::CreateGraphicsPipeline();
	////モデル名を指定してファイルに読み込み
	//model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");


	//object1 = new Object3dFbx;
	//object1->Initialize();
	//object1->SetModel(model1);
	//object1->PlayAnimation();
	
	player_ = new Player();
	player_->Initialize(spriteCommon, input,winApp);

	LoadEnemyPopData();
}
void GameScene::Update()
{

	UpdateEnemyPopCommands();

	//デスフラグの立った弾を削除
	enemy_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		enemy->SetGameScene(this);
		enemy->Update();
	}

	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		enemy->OnCollision();
	}

	//camera->SetEye({ 0,-300,300, });
	camera->Update();
	player_->Update();
	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });


	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();

}
void GameScene::Draw()
{



	Object3d::PreDraw(dxCommon_->GetCommandList());
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//敵キャラの描画
		enemy->Draw();
	}

	player_->Draw();
	skydome->Draw();

	Object3d::PostDraw();

	/*ImGuiMan->Draw();*/

	spriteCommon->PreDraw();

	player_->DrawUI();


}


void GameScene::Finalize()
{
	ImGuiMan->Finalize();
	audio->Finalize();

	delete audio;
	//ImGuiの開放
	delete ImGuiMan;

	//3Dモデル開放
	delete model;

	//スプライトの開放
	delete spriteCommon;

}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
#pragma endregion

	//自キャラも座標
	posA = player_->GetWorldPosition();


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

			}
		}
	}

}

void GameScene::LoadEnemyPopData()
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

void GameScene::ExistenceEnemy(const Vector3& EnemyPos)
{
	//敵キャラの生成
//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(EnemyPos);

	//リストに登録する
	enemy_.push_back(std::move(newEnemy));
}

void GameScene::PhaseReset()
{
	//自キャラの初期化
	player_->Reset();
}

void GameScene::EnemyPopComandReset()
{
	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();
}
