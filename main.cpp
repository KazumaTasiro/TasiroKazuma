#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "GameScene.h"
#include "PostEffect.h"



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



	//�|�C���^
	Input* input = nullptr;

	//���͂̏�����
	input = new Input();
	input->Initalize(winApp);

	PostEffect* posteffect = nullptr;

	posteffect = new PostEffect();

	posteffect->Initialize(dxCommon->GetDevice());
	
	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//�p�[�e�B�N���ÓI������
	ParticleManager::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	ParticleManager2::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	GameScene* gameScene = new GameScene();
	gameScene->Initialize(winApp, dxCommon, input);
	
#pragma endregion ��ՃV�X�e���̏�����


	//DIrectX������������������
#pragma region �ŏ��̃V�[���̏�����

	
	
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

		

		//DirectX���t���[�������@��������
		input->Update();
		gameScene->Update();

		//ImGui::SetWindowSize({ 500,100 },0);
		
		

		
#pragma endregion ��ՃV�X�e���̍X�V

#pragma region �ŏ��̃V�[���̍X�V

#pragma endregion �ŏ��̃V�[���̍X�V

		
		//posteffect->Draw(dxCommon_->GetCommandList());
		/*posteffect->Draw(dxCommon_->GetCommandList());*/
		//posteffect->PreDrawScene(dxCommon->GetCommandList());


		//

		//posteffect->PostDrawScene(dxCommon->GetCommandList());

		dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
		// 4.�`��R�}���h�����܂�
#pragma endregion �ŏ��̃V�[���̕`��
		
		

	/*	ImGuiMan->Draw();*/
		gameScene->Draw();
		
		//posteffect->Draw(dxCommon->GetCommandList());
		//

		dxCommon->PostDraw();
		//// 5.���\�[�X�o���A��߂�

				//FPS�Œ�
		fps->FpsControlEnd();

	}
#pragma region �ŏ��̃V�[���̏I��


#pragma endregion �ŏ��̃V�[���̏I��
	gameScene->Finalize();
#pragma region ��ՃV�X�e���̏I��

	//���͊J��
	delete input;
	delete gameScene;
	//DirectX���
	delete dxCommon;
	//WindowsAPI�̏I������
	winApp->Finalize();
	//WindowsAPI���
	delete winApp;
#pragma endregion ��ՃV�X�e���̏I��
	return 0;
}