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
#include "Player.h"
#include "Skydome.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "Road.h"
#include "ParticleManager.h"
#include "Title.h"

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

	void ClearSpriteUpdate();

	void GameOverSpriteUpdate();



	////�e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }

	void PhaseReset();
	void TitleReset();




private:
	enum Scene
	{
		Title,
		Game,
		Boss,
		GameOver,
		GameClear
	};
	Scene scene;
private:
	///���L����
	Player* player_ = nullptr;

	//std::list<std::unique_ptr<Enemy>> enemy_;

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

	Road* road = nullptr;

	Skydome* skydome = nullptr;

	EnemyManager* enemyManager = nullptr;


	Sprite* stert = nullptr;
	Sprite* gameClear = nullptr;
	Sprite* gameOver = nullptr;
	Vector2 spriteEnd;

	bool playMove = true;

	float playPos = 0;

	bool TitleEnd = false;

	ParticleManager* ParticleMana;

	Sprite* blind;

	bool blindFlag = false;
	int blindTime = 30;
	int blindTimeReset = 30;
	float blindW = 0;

	Vector3 cameraTitle = { 0,0,10 };
	Vector3 cameraGame = { 0,0,-10 };

	Titles* title = nullptr;
};