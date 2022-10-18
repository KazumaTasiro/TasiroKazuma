#include "Input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

void Input::Initalize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	////DirectInput�̏�����
	//IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
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
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	HRESULT result;
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	//�O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));

	

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
