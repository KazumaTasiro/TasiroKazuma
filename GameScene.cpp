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
	////DirectX�����������@��������
	//dxCommon_->Initialize(winApp_);
	//�X�v���C�g���ʕ����̏�����
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

	//���L�����̐���
	player_ = new Player();

	//���L�����̏�����
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
	//�f�X�t���O�̗������e���폜
	bullets2_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//�f�X�t���O�̗������e���폜
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


	//�e�X�V
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
			//�G�L�����̕`��
			enemy->Draw();
		}



		//�e�`��
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
	//����Ώ�A��B�̍��W
	XMFLOAT3 posA, posB;

	//���e���X�g�̎擾
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//�G�e���X�g�̎擾
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = GetBullets();

#pragma region ���L�����ƓG�e�̓����蔻��
#pragma endregion

	//���L���������W
	posA = player_->GetWorldPosition();

	//�G�L�����ƓG�e���ׂĂ̓����蔻��
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//�G�e�̍��W
		posB = bullet->GetWorldPosition();

		float lol = { (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) };

		float radius = { (1 + 1) * (1 + 1) };

		if (lol <= radius) {
			//���L�����̏Փˎ��R�[���o�b�N���Ăяo��
			player_->OnCollision();
			//���e�̏Փˎ��R�[���o�b�N���Ăяo��
			bullet->OnCollision();

			PlayerDead = true;
		}
	}

#pragma region ���e�ƓG�L�����̓����蔻��
#pragma endregion
	for (std::unique_ptr<Enemy>& enemy : enemy_) {



		//���e�ƓG���ׂĂ̓����蔻��
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {

			//�G�L���������W
			posA = enemy->GetWorldPosition();

			//���e�̍��W
			posB = bullet->GetWorldPosition();

			float lol = { (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) };

			float radius = { (3 + 3) * (3 + 3) };

  			if (lol <= radius) {
				//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
				enemy->OnCollision();
				//���e�̏Փˎ��R�[���o�b�N���Ăяo��
				bullet->OnCollision();

				EnemyDeadCount++;
			}
		}
	}

#pragma region ���e�ƓG�e�̓����蔻��
#pragma endregion
	////���e�ƓG�e���ׂĂ̓����蔻��
	//for (const std::unique_ptr<EnemyBullet>& bullet2 : enemyBullets) {
	//	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
	//		//���e�̍��W
	//		posA = bullet2->GetWorldPosition();
	//		//�G�e�̍��W
	//		posB = bullet->GetWorldPosition();

	//		float lol = { (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) };

	//		float radius = { (1 + 1) * (1 + 1) };

	//		if (lol <= radius) {
	//			//���L�����̏Փˎ��R�[���o�b�N���Ăяo��
	//			bullet2->OnCollision();
	//			//���e�̏Փˎ��R�[���o�b�N���Ăяo��
	//			bullet->OnCollision();
	//		}
	//	}
	//}
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet)
{
	//���X�g�ɓo�^����
	bullets2_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	//�t�@�C������e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	if (waitflag)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			//�ҋ@����
			waitflag = false;
		}
		return;
	}

	//1�s���̕����������ϐ�
	std::string line;

	//�R�}���h���s���[�v
	while (getline(enemyPopCommands, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//,��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0)
		{
			//�R�����g�s�͔�΂�
			continue;
		}

		//POP�R�}���h
		if (word.find("POP") == 0)
		{
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			//�G�𔭐�������
			ExistenceEnemy(XMFLOAT3(x, y, z));
		}

		//WAIT�R�}���h
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�ҋ@�J�n
			waitflag = true;
			waitTimer = waitTime;

			//�R�}���h���[�v�𔲂���
			break;
		}

	}
}

void GameScene::ExistenceEnemy(const XMFLOAT3& EnemyPos)
{
	//�G�L�����̐���
//�G�L�����̐���
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize( EnemyPos);

	//�G�L�����ɃA�h���X��n��
	newEnemy->SetPlayer(player_);

	//���X�g�ɓo�^����
	enemy_.push_back(std::move(newEnemy));
}

void GameScene::PhaseReset()
{
	//���L�����̏�����
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
