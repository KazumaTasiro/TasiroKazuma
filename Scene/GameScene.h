#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Model.h"
#include "Input.h"
#include "PostEffect.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "FbxLoader.h"
#include "Object3dFbx.h"
#include "Camera.h"
#include "Enemy.h"
#include "Player.h"
#include "Skydome.h"


class GameScene {
public:
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxcomon, Input* input_);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void CheckAllCollisions();

	///// <summary>
	///// �Փ˔���Ɖ���
	///// </summary>
	//void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void UpdateEnemyPopCommands();

	void ExistenceEnemy(const Vector3& EnemyPos);

	////�e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }

	void PhaseReset();

	void EnemyPopComandReset();


private:
	///���L����
	Player* player_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemy_;

	Input* input = nullptr;

	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	ImGuiManager* ImGuiMan = nullptr;
	

	Audio* audio = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	Camera* camera = nullptr;

	//OBJ���烂�f���f�[�^��ǂݍ���
	Model* model = nullptr;

	FbxModel* model1 = nullptr;
	Object3dFbx* object1 = nullptr;

	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	Skydome* skydome = nullptr;
	bool waitflag = false;
	int waitTimer = 0;
};