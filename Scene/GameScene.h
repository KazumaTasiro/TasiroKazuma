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
	void Initialize(WinApp* winApp,DirectXCommon* dxcomon, Input* input_);

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

	//�Q�[���t�F�[�Y
	enum class Phase {
		TITLE,//�^�C�g��
		GAME,//�{��
	};

private:
	Input* input = nullptr;

	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	ImGuiManager* ImGuiMan = nullptr;
	

	Audio* audio = nullptr;

	SpriteCommon* spriteCommon = nullptr;


	//OBJ���烂�f���f�[�^��ǂݍ���
	Model* model = nullptr;

	
	Sprite* sprite = nullptr;
	Sprite* sprite2 = nullptr;
	Sprite* sprite3 = nullptr;
	Object3d* object3d =nullptr;

	PostEffect* posteffect = nullptr;
	
	float f[2] = { 100,100 };

	//�t�F�[�Y
	Phase phase_ = Phase::TITLE;

};