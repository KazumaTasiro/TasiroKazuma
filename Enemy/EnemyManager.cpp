#include"EnemyManager.h"
#include "Player.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize(DirectXCommon* dxCommon, Input* input, SpriteCommon* spriteCommon,Camera* camera)
{
	assert(camera);
	assert(input);
	assert(spriteCommon);
	camera_ = camera;
	spriteCommon_ = spriteCommon;
	input_ = input;
	LoadEnemyPopData();
}

void EnemyManager::Update()
{
	UpdateEnemyPopCommands();

	//�f�X�t���O�̗������e���폜
	enemy_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		enemy->SetGameScene(gameScene_);
		enemy->Update(player_);
	}
}

void EnemyManager::Draw()
{
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//�G�L�����̕`��
		enemy->Draw();
	}
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
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/enemyPop2.csv");
	assert(file.is_open());

	//�t�@�C������e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	//�t�@�C�������
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

void EnemyManager::ExistenceEnemy(const Vector3& EnemyPos)
{
	//�G�L�����̐���
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(EnemyPos,input_, spriteCommon_);

	//���X�g�ɓo�^����
	enemy_.push_back(std::move(newEnemy));
}

void EnemyManager::EnemyCollision(Player* player)
{
	//����Ώ�A��B�̍��W
	Vector3 posA, posB;

	//���e���X�g�̎擾
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	

#pragma region ���L�����ƓG�e�̓����蔻��
#pragma endregion

	//���L���������W
	posA = player->GetWorldPosition();


#pragma region ���e�ƓG�L�����̓����蔻��
#pragma endregion
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//���e�ƓG���ׂĂ̓����蔻��
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {

			//�G�L���������W
			posA = enemy->GetWorldPosition();

			//���e�̍��W
			posB = bullet->GetWorldPosition();

			if (Collision::CircleCollision(posB,posA,1.5f,1.5f)) {
				//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
				enemy->OnCollision();
				//���e�̏Փˎ��R�[���o�b�N���Ăяo��
				bullet->OnCollision();

			}
		}
	}
	for (std::unique_ptr<Enemy>& enemy : enemy_) {

			//�G�L���������W
			posA = enemy->GetWorldPosition();

			Vector2 posR;
			//���e�̍��W
			posR = player->GetReticlePos();

			if (Collision::RaySphere({0,0,0}, posA, 3.0f, player->GetFarNear())) {
				if (input_->PushMouse(1)) {
					//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
					enemy->LockOnTrue();
				}
		}
	}

#pragma region ���L�����ƓG�e�̓����蔻��
#pragma endregion

	////���L���������W
	//posA = player_->GetWorldPosition();

	////�G�L�����ƓG�e���ׂĂ̓����蔻��
	//for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
	//	//�G�e�̍��W
	//	posB = bullet->GetWorldPosition();

	//	float lol = { (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) };

	//	float radius = { (1 + 1) * (1 + 1) };

	//	if (lol <= radius) {
	//		//���L�����̏Փˎ��R�[���o�b�N���Ăяo��
	//		player_->OnCollision();
	//		//���e�̏Փˎ��R�[���o�b�N���Ăяo��
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
	Update();

}
