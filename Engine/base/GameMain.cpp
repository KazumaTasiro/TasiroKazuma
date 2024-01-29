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

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(Framework::GetDXCommon()->GetDevice());
	ParticleManager::StaticInitialize(Framework::GetDXCommon()->GetDevice(), Framework::GetDXCommon()->GetCommandList());
	ParticleLibrary::GetInstance()->Initialize();
	//DirectionalLight::StaticInitialize(Framework::GetDXCommon()->GetDevice());


	gameScene = new GameScene();
	gameScene->Initialize(Framework::GetDXCommon());

}

void GameMain::Finalize()
{
	gameScene->Finalize();
	FbxLoader::GetInstance()->Finalize();

	
	delete gameScene;
	delete posteffect;
	
	//基礎クラスの終了処理
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
