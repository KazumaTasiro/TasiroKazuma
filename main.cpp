#include "GameMain.h"


//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	GameMain* gameMain = nullptr;
	gameMain = new GameMain();

	//�Q�[���̏�����
	gameMain->Inirialize();


	//�Q�[�����[�v
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