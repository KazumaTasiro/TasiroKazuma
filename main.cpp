#include "GameMain.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	GameMain* gameMain = nullptr;
	gameMain = new GameMain();

	//ゲームの初期化
	gameMain->Inirialize();


	//ゲームループ
	while (true) {

		gameMain->Update();
		if (gameMain->isEndRequest() == true) {
			break;
		}

		gameMain->Draw();
	}

	gameMain->Finalize();
	delete gameMain;

	return 0;
}