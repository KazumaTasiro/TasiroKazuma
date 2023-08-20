#include "Framework.h"

void Framework::Run()
{
	//�Q�[���̏�����
	Initialize();
	while (true)
	{
		//���t���[���X�V
		Update();
		//�I�����N�G�X�g�������甲����
		if (IsEndRequst()) {
			break;
		}
		//�`��
		Draw();
	}
	//�Q�[���̏I��
	Finalize();

}

void Framework::Initialize()
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
}

void Framework::Finalize()
{
	//���͊J��
	delete input;
	delete fps;

	//DirectX���
	delete dxCommon;
	//WindowsAPI�̏I������
	winApp->Finalize();
	//WindowsAPI���
	delete winApp;
}

void Framework::Update()
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

	//FPS�Œ�
	fps->FpsControlEnd();
}
