#include "GameMain.h"

GameMain::GameMain()
{
}

GameMain::~GameMain()
{
}

void GameMain::Initialize()
{

	Framework::Initialize();
	posteffect = new PostEffect();

	posteffect->Initialize(Framework::GetDXCommon()->GetDevice(), Framework::GetInput());

	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(Framework::GetDXCommon()->GetDevice(), WinApp::window_width, WinApp::window_height);
	ParticleManager::StaticInitialize(Framework::GetDXCommon()->GetDevice(), Framework::GetDXCommon()->GetCommandList());

	gameScene = new GameScene();
	gameScene->Initialize(Framework::GetWinApp(), Framework::GetDXCommon(), Framework::GetInput());

}

void GameMain::Finalize()
{
	gameScene->Finalize();
	FbxLoader::GetInstance()->Finalize();

	
	delete gameScene;
	delete posteffect;
	
	//��b�N���X�̏I������
	Framework::Finalize();
}

void GameMain::Update()
{
	Framework::Update();
	gameScene->Update();
}

void GameMain::Draw()
{
	posteffect->PreDrawScene(Framework::GetDXCommon()->GetCommandList());

	gameScene->Draw();

	posteffect->PostDrawScene(Framework::GetDXCommon()->GetCommandList());

	Framework::GetDXCommon()->PreDraw();

	posteffect->Draw(Framework::GetDXCommon()->GetCommandList());

	Framework::GetDXCommon()->PostDraw();


}
