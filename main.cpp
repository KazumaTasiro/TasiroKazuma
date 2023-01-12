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



//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region ��ՃV�X�e���̏�����
	WinApp* winApp = nullptr;

	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//�|�C���^
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	MSG msg{};//���b�Z�[�W


	ImGuiManager* ImGuiMan = nullptr;
	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp,dxCommon);

	//�|�C���^
	Input* input = nullptr;

	//���͂̏�����
	input = new Input();
	input->Initalize(winApp);

	Audio* audio = nullptr;
	audio = new Audio();
	audio->Initialize();

	audio->LoadWave("se_amd06.wav");
	/*audio->PlayWave("se_amd06.wav", true,2.0f,1.0f);*/
	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	SpriteCommon* spriteCommon = nullptr;
	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	//OBJ���烂�f���f�[�^��ǂݍ���
	Model* model = Model::LoadFormOBJ("cube");

	Object3d* object3d = Object3d::Create();
	object3d->SetModel(model);

	object3d->Update();
#pragma endregion ��ՃV�X�e���̏�����


	//DIrectX������������������
#pragma region �ŏ��̃V�[���̏�����

	

	spriteCommon->LoadTexture(0, "meemu.jpg");
	spriteCommon->LoadTexture(1, "meemu.jpg");
	spriteCommon->LoadTexture(2, "wakka.jpg");

	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon,0);

	Sprite* sprite2 = new Sprite();

	sprite2->Initialize(spriteCommon,1);
	//sprite2->SetTextureIndex(1);
	sprite2->SetSize({ 100,100 });



	XMFLOAT2 position = sprite2->GetPosition();

	position.x += 100;

	position.y += 50;

	sprite2->SetPozition(position);

	XMFLOAT4 color = { 0,1,0,1 };

	sprite2->SetColor(color);

	sprite2->SetSize(XMFLOAT2{ 100.0f,50.0f });

	Sprite* sprite3 = new Sprite();

	sprite3->Initialize(spriteCommon, 2);
	//sprite3->SetTextureIndex(2);
	sprite3->SetSize({ 100,100 });
	sprite3->SetPozition({ 0,300 });
	sprite3->SetColor({ 1,1,1,1 });
	/*sprite2->SetIsFlipY(true);*/




	float f[2] = { 100,100 };
#pragma endregion �ŏ��̃V�[���̏�����

	//DIrectX���������������܂�
	// �`�揉��������
	// ���_�f�[�^�\����

	FPS* fps = new FPS;

	//�Q�[�����[�v
	while (true) {
#pragma region ��ՃV�X�e���̍X�V
		//���b�Z�[�W������H

		//fps����
		fps->FpsControlBegin();

		//Windows�̃��b�Z�[�W����
		if (winApp->ProcessMessage()) {
			//�Q�[�����[�v�𔲂���
			break;
		}

		char buf[10] = {};
		sprite2->SetPozition({ f[0],f[1] });

		//DirectX���t���[�������@��������
		input->Update();
		object3d->Update();

		//ImGui::SetWindowSize({ 500,100 },0);
		
		ImGuiMan->Bigin();

		ImGui::SetWindowSize({ 500,100 });
		ImGui::SliderFloat2("position", &f[0], 0.0f, 1280.0f,"%.1f");


		//�f���E�B���h�E�̕\��ON
		//ImGui::ShowDemoWindow();

		ImGuiMan->End();

		
#pragma endregion ��ՃV�X�e���̍X�V

#pragma region �ŏ��̃V�[���̍X�V

#pragma endregion �ŏ��̃V�[���̍X�V

		dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
		// 4.�`��R�}���h�����܂�
		spriteCommon->PreDraw();

		sprite->Draw();
		sprite2->Draw();
		sprite3->Draw();
#pragma endregion �ŏ��̃V�[���̕`��
		
		
		/*dxCommon->ClearDepthBuffer();*/

		Object3d::PreDraw(dxCommon->GetCommandList());

		object3d->Draw();

		Object3d::PostDraw();

		ImGuiMan->Draw();

		dxCommon->PostDraw();
		//// 5.���\�[�X�o���A��߂�

				//FPS�Œ�
		fps->FpsControlEnd();

	}
#pragma region �ŏ��̃V�[���̏I��


#pragma endregion �ŏ��̃V�[���̏I��
	ImGuiMan->Finalize();
	audio->Finalize();
#pragma region ��ՃV�X�e���̏I��
	delete audio;
	//ImGui�̊J��
	delete ImGuiMan;
	//���͊J��
	delete input;
	//3D�I�u�W�F�N�g�̉��
	delete object3d;
	//3D���f���J��
	delete model;

	//�X�v���C�g�̊J��
	delete spriteCommon;
	delete sprite2;
	//DirectX���
	delete dxCommon;
	//WindowsAPI�̏I������
	winApp->Finalize();
	//WindowsAPI���
	delete winApp;
#pragma endregion ��ՃV�X�e���̏I��
	return 0;
}