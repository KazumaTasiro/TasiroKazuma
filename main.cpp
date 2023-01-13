#include "Input/Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "Audio.h"
#include"GameScene.h"


//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region ��ՃV�X�e���̏�����

	//�|�C���^�錾
	WinApp* winApp_ = nullptr;
	winApp_ = new WinApp;


	DirectXCommon* dxCommon_ = nullptr;
	dxCommon_ = new DirectXCommon;

	//winApp������
	winApp_->Initialize();

	MSG msg = {};
	//DirectX�����������@��������
	dxCommon_->Initialize(winApp_);

	Input* input_ = nullptr;
	input_ = new Input;
	input_->Initalize(winApp_);

	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);

	SpriteCommon* spriteCommon = nullptr;
	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

	ImGuiManager* ImGuiMan = nullptr;
	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp_, dxCommon_);
	Audio* audio = nullptr;
	audio = new Audio();
	audio->Initialize();

//	//OBJ���烂�f���f�[�^��ǂݍ���
//	Model* model = Model::LoadFormOBJ("cube");
//
//	Object3d* object3d = Object3d::Create();
//	object3d->SetModel(model);
//	object3d->SetRotetion({ 30,30,30 });
//
//	object3d->Update();
//#pragma endregion ��ՃV�X�e���̏�����
//
//
//	//DIrectX������������������
//#pragma region �ŏ��̃V�[���̏�����
//
//	/*Sprite* sprite = new Sprite();
//	sprite->Initialize(spriteCommon);*/
//
//	spriteCommon->LoadTexture(0, "meemu.jpg");
//	spriteCommon->LoadTexture(1, "wakka.jpg");
//
//	Sprite* sprite2 = new Sprite();
//
//	sprite2->Initialize(spriteCommon, 1);
//	sprite2->SetTextureIndex(1);
//	sprite2->SetSize({ 100,100 });
//
//	XMFLOAT2 position = sprite2->GetPosition();
//	XMFLOAT2 posE = { 0,20 };
//
//	//object3d->SetEye({posE.x,posE.y,-50 });
//	//object3d->SetTarget({ posE.x,posE.y,0 });
//	position.x += 100;
//
//	position.y += 50;
//
//	sprite2->SetPozition(position);
//
//	XMFLOAT4 color = { 0,1,0,1 };
//
//	sprite2->SetColor(color);
//
//	sprite2->SetSize(XMFLOAT2{ 100.0f,50.0f });

	/*sprite2->SetIsFlipY(true);*/




	float f[2] = { 100,100 };
#pragma endregion �ŏ��̃V�[���̏�����

	//DIrectX���������������܂�
	// �`�揉��������
	// ���_�f�[�^�\����

	FPS* fps = new FPS;


	GameScene* gameScene = nullptr;
	/* �Q�[���V�[���̏�����*/
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon_,input_);
	//�Q�[�����[�v
	while (true) {
#pragma region ��ՃV�X�e���̍X�V
		//���b�Z�[�W������H

		//fps����
		fps->FpsControlBegin();

		//Windows�̃��b�Z�[�W����
		if (winApp_->ProcessMessage()) {
			//�Q�[�����[�v�𔲂���
			break;
		}

		char buf[10] = {};
		//sprite2->SetPozition({ f[0],f[1] });

		//DirectX���t���[�������@��������
		input_->Update();
		gameScene->Update();
		//object3d->Update();

		//ImGui::SetWindowSize({ 500,100 },0);

		ImGuiMan->Bigin();

		ImGui::SetWindowSize({ 500,100 });
		ImGui::SliderFloat2("position", &f[0], 0.0f, 1280.0f, "%.1f");
		//�f���E�B���h�E�̕\��ON
		//ImGui::ShowDemoWindow();

		ImGuiMan->End();


#pragma endregion ��ՃV�X�e���̍X�V

#pragma region �ŏ��̃V�[���̍X�V

#pragma endregion �ŏ��̃V�[���̍X�V

		dxCommon_->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
		// 4.�`��R�}���h�����܂�

		/*sprite->Draw();*/
		//sprite2->Draw();

#pragma endregion �ŏ��̃V�[���̕`��

		gameScene->Draw();
		/*dxCommon->ClearDepthBuffer();*/

		//Object3d::PreDraw(dxCommon->GetCommandList());

		//object3d->Draw();

		//Object3d::PostDraw();

		//ImGuiMan->Draw();

		dxCommon_->PostDraw();
		//// 5.���\�[�X�o���A��߂�

				//FPS�Œ�
		fps->FpsControlEnd();

	}
#pragma region �ŏ��̃V�[���̏I��


#pragma endregion �ŏ��̃V�[���̏I��

	audio->Finalize();
	ImGuiMan->Finalize();

#pragma region ��ՃV�X�e���̏I��
	//Audio�̉��
	//delete audio;
	//ImGui�̊J��
	delete ImGuiMan;
	//���͊J��
	delete input_;
	////3D�I�u�W�F�N�g�̉��
	//delete object3d;
	////3D���f���J��
	//delete model;

	//�X�v���C�g�̊J��
	delete spriteCommon;
	//delete sprite2;
	//DirectX���
	delete dxCommon_;
	//WindowsAPI�̏I������
	winApp_->Finalize();
	//WindowsAPI���
	delete winApp_;
#pragma endregion ��ՃV�X�e���̏I��
	return 0;
}