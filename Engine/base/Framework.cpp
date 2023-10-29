#include "Framework.h"

void Framework::Run()
{
	//ゲームの初期化
	Initialize();
	while (true)
	{
		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if (IsEndRequst()) {
			break;
		}
		//描画
		Draw();
	}
	//ゲームの終了
	Finalize();
}

void Framework::Initialize()
{
	//WindowsAPIの初期化
	winApp = WinApp::GetInstance();
	winApp->Initialize();

	//ポインタ

	dxCommon = new DirectXCommon();
	dxCommon->Initialize();

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//入力の初期化
	input =  Input::GetInstance();
	input->Initalize();
}

void Framework::Finalize()
{
	//入力開放
	delete fps;

	//DirectX解放
	delete dxCommon;
	//WindowsAPIの終了処理
	winApp->Finalize();
	//WindowsAPI解放
}

void Framework::Update()
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

	//FPS固定
	fps->FpsControlEnd();
}
