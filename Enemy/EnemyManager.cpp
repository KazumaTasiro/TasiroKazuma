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

	//�p�[�e�B�N������
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
		//�G�L�����̕`��
		if (enemy->IsDead()) {
			EnemyDeadParticle(enemy->GetWorldPosition());
		}
	}
	enemyDeadParticle->Update();
	//�f�X�t���O�̗������e���폜
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
		//�G�L�����̕`��
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
	randEnemyNmb = rand() % 2;
	//�G�L�����̐���
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(EnemyPos, input_, spriteCommon_, enemyModel_, enemyBulletModel_, enemyReticleModel_,randEnemyNmb);

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

			if (Collision::CircleCollision(posB, posA, 2.0f, 2.0f)) {
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

		if (Collision::RaySphere({ 0,0,0 }, posA, 3.0f, player->GetFarNear())) {
			if (input_->PushMouse(1)) {
				//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
				enemy->LockOnTrue();
			}
		}
	}

	if (clearNum <= clearCount) {
		//���e�ƃ{�X�̓����蔻��
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {

			//�G�L���������W
			posA = boss->GetWorldPosition();

			//���e�̍��W
			posB = bullet->GetWorldPosition();

			if (Collision::CircleCollision(posB, posA, 50.0f, 50.0f)) {
				//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
				boss->OnCollision();
				//���e�̏Փˎ��R�[���o�b�N���Ăяo��
				bullet->OnCollision();

			}
		}
		//�G�L���������W
		posA = boss->GetWorldPosition();

		Vector2 posR;
		//���e�̍��W
		posR = player->GetReticlePos();

		if (Collision::RaySphere({ 0,0,0 }, posA, 50.0f, player->GetFarNear())) {
			if (input_->PushMouse(1)) {
				//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
				boss->LockOnTrue();
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
	//�p�[�e�B�N���͈�
	for (int i = 0; i < 5; i++) {
		//X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
		const float rnd_pos = 5.0f;
		Vector3 pos = EnemyPos;
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//���x
		//X,Y,Z�S��[-0.05f,+0.05f]�Ń����_���ɕ��z
		const float rnd_vel = 0.0f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		const float rnd_acc = 0.0000f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//�ǉ�
		enemyDeadParticle->Add(30, pos, vel, acc, 0.0f, 20.0f, 1);
		enemyDeadParticle->Add(30, pos, vel, acc, 0.0f, 20.0f, 2);
		enemyDeadParticle->Update();
	}
}

void EnemyManager::BossDeadParticle(Vector3 EnemyPos)
{
	//�p�[�e�B�N���͈�
	for (int i = 0; i < 10; i++) {
		//X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
		const float rnd_pos = 5.0f;
		Vector3 pos = EnemyPos;
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//���x
		//X,Y,Z�S��[-0.05f,+0.05f]�Ń����_���ɕ��z
		const float rnd_vel = 0.0f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		const float rnd_acc = 0.0000f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//�ǉ�
		enemyDeadParticle->Add(40, pos, vel, acc, 0.0f, 50.0f, 1);
		enemyDeadParticle->Add(40, pos, vel, acc, 0.0f, 50.0f, 2);
		enemyDeadParticle->Update();
	}
}
