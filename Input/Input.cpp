#include "Input.h"


#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

void Input::Initalize(WinApp* winApp)
{
	HRESULT result;
	//�؂�Ă���WinApp�̃C���X�^���X���L�^
	winApp_ = winApp;

	////DirectInput�̏�����
	//IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//�L�[�{�[�h�f�o�C�X�̐���
	
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//�r���I���䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//�}�E�X�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));
	//���̓f�[�^�`���̃Z�b�g
	result = mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//�r���I���䃌�x���̃Z�b�g
	result = mouse->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));



}

void Input::Update()
{
	HRESULT result;
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	//�O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));


	// �X�V�O�ɍŐV�}�E�X����ۑ�����
	PrevMouseState = CurrentMouseState;
	// �ŐV�̃}�E�X�̏�Ԃ��X�V
	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &CurrentMouseState);
	if (FAILED(result))
	{
		mouse->Acquire();
		result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &CurrentMouseState);
	}

	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNunber)
{
	//�w��L�[�������Ă����true��Ԃ�
	if (key[keyNunber]) {
		return true;
	}

	return false;
}

bool Input::TriggerKey(BYTE keyNunber)
{
	if (!keyPre[keyNunber] && key[keyNunber]) {
		return true;
	}

	return false;
}

bool Input::PushMouse(int mouse_)
{
	// ���N���b�N����Ă��邩����
	if (CurrentMouseState.rgbButtons[mouse_] & (0x80))
	{
		// ���N���b�N��
		return true;
	}

	return false;
}

bool Input::TriggerMouse(int mouse_)
{
	if (!(PrevMouseState.rgbButtons[mouse_] & (0x80)) &&
		CurrentMouseState.rgbButtons[mouse_] & (0x80))
	{
		return true;
	}
	

	return false;
}

bool Input::ReleaseMouse(int mouse_)
{
	if (PrevMouseState.rgbButtons[mouse_] & (0x80) &&
		!(CurrentMouseState.rgbButtons[mouse_] & (0x80)))
	{
		return true;
	}

	return false;
}
