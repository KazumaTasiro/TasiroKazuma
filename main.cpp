#include "Input/Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "SpriteCommon.h"
#include "Sprite.h"





//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region 基盤システムの初期化
	WinApp* winApp = nullptr;

	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//ポインタ
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	MSG msg{};//メッセージ

	//ポインタ
	Input* input = nullptr;

	//入力の初期化
	input = new Input();
	input->Initalize(winApp);

	SpriteCommon* spriteCommon = nullptr;
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

#pragma endregion 基盤システムの初期化


	//DIrectX初期化処理ここから
#pragma region 最初のシーンの初期化

	/*Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon);*/

	Sprite* sprite2 = new Sprite();
	sprite2->Initialize(spriteCommon);

	XMFLOAT2 position = sprite2->GetPosition();

	position.x += 100;

	position.y += 50;

	sprite2->SetPozition(position);

	XMFLOAT4 color = { 0,1,0,1 };

	sprite2->SetColor(color);

	sprite2->SetSize(XMFLOAT2{100.0f,50.0f});

	sprite2->SetIsFlipY(true);

	spriteCommon->LoadTexture(0, "meemu.jpg");
	spriteCommon->LoadTexture(1, "wakka.jpg");

	sprite2->SetTextureIndex(1);

#pragma endregion 最初のシーンの初期化
	
	//DIrectX初期化処理ここまで
	// 描画初期化処理
	// 頂点データ構造体
	
	FPS* fps = new FPS;

	//ゲームループ
	while (true) {
#pragma region 基盤システムの更新
		//メッセージがある？
		
		//fps制限
		fps->FpsControlBegin();

		//Windowsのメッセージ処理
		if (winApp->ProcessMessage()) {
			//ゲームループを抜ける
			break;
		}

		//DirectX舞フレーム処理　ここから
		input->Update();
		//sprite2->Update();
#pragma endregion 基盤システムの更新

#pragma region 最初のシーンの更新

#pragma endregion 最初のシーンの更新

		dxCommon->PreDraw();

#pragma region 最初のシーンの描画
		// 4.描画コマンドここまで

		/*sprite->Draw();*/
		sprite2->Draw();

#pragma endregion 最初のシーンの描画
		dxCommon->PostDraw();
		//// 5.リソースバリアを戻す

				//FPS固定
		fps->FpsControlEnd();

	}
#pragma region 最初のシーンの終了


#pragma endregion 最初のシーンの終了


#pragma region 基盤システムの終了
	//入力開放
	delete input;
	//スプライトの開放

	delete spriteCommon;
	delete sprite2;
	//DirectX解放
	delete dxCommon;
	//WindowsAPIの終了処理
	winApp->Finalize();
	//WindowsAPI解放
	delete winApp;
#pragma endregion 基盤システムの終了
	return 0;
}