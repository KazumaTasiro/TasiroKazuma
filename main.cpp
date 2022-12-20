#include "Input/Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "SpriteCommon.h"
#include "Sprite.h"





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

	SpriteCommon* spriteCommon = nullptr;
	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

#pragma endregion ��ՃV�X�e���̏�����


	//DIrectX������������������
#pragma region �ŏ��̃V�[���̏�����

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
		//sprite2->Update();
#pragma endregion ��ՃV�X�e���̍X�V

#pragma region �ŏ��̃V�[���̍X�V

#pragma endregion �ŏ��̃V�[���̍X�V

		dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
		// 4.�`��R�}���h�����܂�

		/*sprite->Draw();*/
		sprite2->Draw();

#pragma endregion �ŏ��̃V�[���̕`��
		dxCommon->PostDraw();
		//// 5.���\�[�X�o���A��߂�

				//FPS�Œ�
		fps->FpsControlEnd();

	}
#pragma region �ŏ��̃V�[���̏I��


#pragma endregion �ŏ��̃V�[���̏I��


#pragma region ��ՃV�X�e���̏I��
	//���͊J��
	delete input;
	//�X�v���C�g�̊J��

	delete spriteCommon;
	delete sprite2;
	//DirectX���
	delete dxCommon;
	//WindowsAPI�̏I������
	winApp->Finalize();
	//WindowsAPI���
	delete winApp;
#pragma endregion ��ՃV�X�e���̏I��
	return 0;
}