#pragma once
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "GameScene.h"
#include "PostEffect.h"
#include "FbxLoader.h"

//�Q�[���S��
class GameMain
{
public:
	GameMain();
	~GameMain();

	//����������
	void Inirialize();

	//�I��
	void Finalize();

	//���t���[���X�V()
	void Update();

	//�`��
	void Draw();

	//�I���t���O�̃`�F�b�N
	bool isEndRequest() { return endRequest_; }

private:
	//WinApp
	WinApp* winApp = nullptr;
	//�Q�[���I���t���O
	bool endRequest_ = false;
	//DXcommon
	DirectXCommon* dxCommon = nullptr;
	//�C���v�b�g
	Input* input = nullptr;
	//�|�X�g�G�t�F�N�g
	PostEffect* posteffect = nullptr;
	//FPS
	FPS* fps = new FPS;
	//GameScene
	GameScene* gameScene = nullptr;
	MSG msg{};//���b�Z�[�W
};