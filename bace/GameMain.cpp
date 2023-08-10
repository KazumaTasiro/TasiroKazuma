#include "GameMain.h"

GameMain::GameMain()
{
}

GameMain::~GameMain()
{
}

void GameMain::Inirialize()
{


	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//ポインタ
	
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);



	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	

	//入力の初期化
	input = new Input();
	input->Initalize(winApp);

	

	posteffect = new PostEffect();

	posteffect->Initialize(dxCommon->GetDevice(), input);

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	gameScene = new GameScene();
	gameScene->Initialize(winApp, dxCommon, input);

}

void GameMain::Finalize()
{
	gameScene->Finalize();
	FbxLoader::GetInstance()->Finalize();

	//入力開放
	delete input;
	delete gameScene;
	delete posteffect;
	delete fps;

	//DirectX解放
	delete dxCommon;
	//WindowsAPIの終了処理
	winApp->Finalize();
	//WindowsAPI解放
	delete winApp;
}

void GameMain::Update()
{
	//メッセージがある？

	//fps制限
	fps->FpsControlBegin();

	//Windowsのメッセージ処理
	if (winApp->ProcessMessage()) {
		//ゲームループを抜ける
		endRequest_ = true;
	}



	//DirectX毎フレーム処理　ここから
	input->Update();
	gameScene->Update();
}

void GameMain::Draw()
{
	posteffect->PreDrawScene(dxCommon->GetCommandList());

	gameScene->Draw();


	posteffect->PostDrawScene(dxCommon->GetCommandList());

	dxCommon->PreDraw();

	// 4.描画コマンドここまで

	posteffect->Draw(dxCommon->GetCommandList());


	dxCommon->PostDraw();
	// 5.リソースバリアを戻す

	//FPS固定
	fps->FpsControlEnd();
}
