#include "Framework.h"
#include "GameMain.h"


//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Framework* game = nullptr;
	game = new GameMain();

	game->Run();
	delete game;

	return 0;
}