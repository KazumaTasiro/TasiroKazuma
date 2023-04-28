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
#include "JsonLoader.h"

struct LevelData;

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

	LevelData* levelData = nullptr;

	Model* modelBox = nullptr;
	Model* modelSphere = nullptr;

	std::map<std::string, Model*> models;
	std::vector<Object3d*> objects;

};