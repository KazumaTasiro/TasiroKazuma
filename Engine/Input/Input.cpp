#include "Input.h"


#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

void Input::Initalize(WinApp* winApp)
{
	HRESULT result;
	//借りてきたWinAppのインスタンスを記録
	winApp_ = winApp;

	////DirectInputの初期化
	//IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//排他的制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//マウスデバイスの生成
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//排他的制御レベルのセット
	result = mouse->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));



}

void Input::Update()
{
	HRESULT result;
	//キーボード情報の取得開始
	keyboard->Acquire();

	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));


	// 更新前に最新マウス情報を保存する
	PrevMouseState = CurrentMouseState;
	// 最新のマウスの状態を更新
	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &CurrentMouseState);
	if (FAILED(result))
	{
		mouse->Acquire();
		result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &CurrentMouseState);
	}

	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNunber)
{
	//指定キーを押していればtrueを返す
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
	// 左クリックされているか判定
	if (CurrentMouseState.rgbButtons[mouse_] & (0x80))
	{
		// 左クリック中
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
