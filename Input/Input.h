#pragma once
#define DIRECTINPUT_VERSION  0x0800//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include <windows.h>
#include "WinApp.h"

using namespace Microsoft::WRL;

class Input {
public:
	//namespace�ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://�����o�[�֐�
	//������
	void Initalize(WinApp* winApp);
	//�X�V
	void Update();

	//�L�[�������������`�F�b�N
	bool PushKey(BYTE keyNunber);
	//�L�[�̃g���K�[���`�F�b�N
	bool TriggerKey(BYTE keyNunber);

	//�L�[�������������`�F�b�N
	//0�@���N���b�N
	//1�@�E�N���b�N
	//2�@�}�E�X�J�[�\���N���b�N
	bool PushMouse(int mouse_);
	//�L�[�̃g���K�[���`�F�b�N
	//0�@���N���b�N
	//1�@�E�N���b�N
	//2�@�}�E�X�J�[�\���N���b�N
	bool TriggerMouse(int mouse_);
	//�}�E�X�̃����[�X���`�F�b�N
	//0�@���N���b�N
	//1�@�E�N���b�N
	//2�@�}�E�X�J�[�\���N���b�N
	bool ReleaseMouse(int mouse_);
private://�����o�ϐ�
	//�L�[�{�[�h�̃f�o�C�X
	IDirectInputDevice8* keyboard = nullptr;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	//�}�E�X�̃f�o�C�X
	IDirectInputDevice8* mouse = nullptr;
	DIMOUSESTATE CurrentMouseState;		//!< �}�E�X�̌��݂̓��͏��
	DIMOUSESTATE PrevMouseState;			//!< �}�E�X�̈�t���[���O�̓��͏��


	//DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> directInput;
	//WindowsAPI
	WinApp* winApp_ = nullptr;
};