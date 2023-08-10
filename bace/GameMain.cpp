#include "GameMain.h"

GameMain::GameMain()
{
}

GameMain::~GameMain()
{
}

void GameMain::Inirialize()
{


	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//�|�C���^
	
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);



	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	

	//���͂̏�����
	input = new Input();
	input->Initalize(winApp);

	

	posteffect = new PostEffect();

	posteffect->Initialize(dxCommon->GetDevice(), input);

	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	gameScene = new GameScene();
	gameScene->Initialize(winApp, dxCommon, input);

}

void GameMain::Finalize()
{
	gameScene->Finalize();
	FbxLoader::GetInstance()->Finalize();

	//���͊J��
	delete input;
	delete gameScene;
	delete posteffect;
	delete fps;

	//DirectX���
	delete dxCommon;
	//WindowsAPI�̏I������
	winApp->Finalize();
	//WindowsAPI���
	delete winApp;
}

void GameMain::Update()
{
	//���b�Z�[�W������H

	//fps����
	fps->FpsControlBegin();

	//Windows�̃��b�Z�[�W����
	if (winApp->ProcessMessage()) {
		//�Q�[�����[�v�𔲂���
		endRequest_ = true;
	}



	//DirectX���t���[�������@��������
	input->Update();
	gameScene->Update();
}

void GameMain::Draw()
{
	posteffect->PreDrawScene(dxCommon->GetCommandList());

	gameScene->Draw();


	posteffect->PostDrawScene(dxCommon->GetCommandList());

	dxCommon->PreDraw();

	// 4.�`��R�}���h�����܂�

	posteffect->Draw(dxCommon->GetCommandList());


	dxCommon->PostDraw();
	// 5.���\�[�X�o���A��߂�

	//FPS�Œ�
	fps->FpsControlEnd();
}
