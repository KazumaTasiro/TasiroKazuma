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
	
	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	camera->SetEye({ 0,0,-100 });
	Object3d::SetCamera(camera);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);
	spriteCommon->LoadTexture(0, "Reticle.png");

	skydome = new Skydome();
	skydome->Initalize();

	//Object3dFbx::SetDevice(dxCommon_->GetDevice());
	//Object3dFbx::SetCamera(camera);
	//Object3dFbx::CreateGraphicsPipeline();
	////���f�������w�肵�ăt�@�C���ɓǂݍ���
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

	//�f�X�t���O�̗������e���폜
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


	//�f���E�B���h�E�̕\��ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();

}
void GameScene::Draw()
{



	Object3d::PreDraw(dxCommon_->GetCommandList());
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//�G�L�����̕`��
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
	//ImGui�̊J��
	delete ImGuiMan;

	//3D���f���J��
	delete model;

	//�X�v���C�g�̊J��
	delete spriteCommon;

}

void GameScene::CheckAllCollisions()
{
	//����Ώ�A��B�̍��W
	Vector3 posA, posB;

	//���e���X�g�̎擾
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

#pragma region ���L�����ƓG�e�̓����蔻��
#pragma endregion

	//���L���������W
	posA = player_->GetWorldPosition();


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

			}
		}
	}

}

void GameScene::LoadEnemyPopData()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/enemyPop2.csv");
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
			ExistenceEnemy(Vector3(x, y, z));
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

void GameScene::ExistenceEnemy(const Vector3& EnemyPos)
{
	//�G�L�����̐���
//�G�L�����̐���
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(EnemyPos);

	//���X�g�ɓo�^����
	enemy_.push_back(std::move(newEnemy));
}

void GameScene::PhaseReset()
{
	//���L�����̏�����
	player_->Reset();
}

void GameScene::EnemyPopComandReset()
{
	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();
}
