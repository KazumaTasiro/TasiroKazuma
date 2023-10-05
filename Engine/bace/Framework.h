#pragma once
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "FbxLoader.h"

//�Q�[���S��
class Framework
{
public://�����o�֐�

	//���s
	void Run();

	virtual ~Framework() = default;

	//������
	virtual void Initialize();

	//�I��
	virtual void Finalize();

	//���t���[������
	virtual void Update();

	//�`��
	virtual void Draw() = 0;

	//�I���`�F�b�N
	virtual bool IsEndRequst() { return endRequest_; }


	WinApp* GetWinApp() { return winApp; }
	DirectXCommon* GetDXCommon() { return dxCommon; }
	Input* GetInput() { return input; }

private:
	//�Q�[���I���t���O
	bool endRequest_ = false;
	//WinApp
	WinApp* winApp = nullptr;

	//DXcommon
	DirectXCommon* dxCommon = nullptr;
	//�C���v�b�g
	Input* input = nullptr;
	//FPS
	FPS* fps = new FPS;
	//���b�Z�[�W
	MSG msg{};
};