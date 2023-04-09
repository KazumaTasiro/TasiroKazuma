#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "PostEffect.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include"ParticleManager.h"
#include"ParticleManager2.h"

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

	void EffectAdd();

private:
	Input* input = nullptr;

	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	ImGuiManager* ImGuiMan = nullptr;
	
	Camera* camera = nullptr;
	Transform camWtf;

	Audio* audio = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	Sprite* sprite = nullptr;
	Sprite* sprite2 = nullptr;

	Object3d* obj = nullptr;
	Object3d* obj2 = nullptr;

	//OBJ���烂�f���f�[�^��ǂݍ���
	Model* model = nullptr;
	Model* model2 = nullptr;

	//�p�[�e�B�N���N���X�̏����� 
	ParticleManager* particleManager = nullptr;
	ParticleManager2* particleManager2 = nullptr;
};