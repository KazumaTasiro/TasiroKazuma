#pragma once
#define DIRECTINPUT_VERSION  0x0800//DirectInputのバージョン指定
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include <windows.h>
#include "../WinApp.h"

using namespace Microsoft::WRL;

class Input {
public:
	//namespace省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバー関数
	//初期化
	void Initalize(WinApp* winApp);
	//更新
	void Update();

	//キーを押したかをチェック
	bool PushKey(BYTE keyNunber);
	//キーのトリガーをチェック
	bool TriggerKey(BYTE keyNunber);
private://メンバ変数
	//キーボードのデバイス
	IDirectInputDevice8* keyboard = nullptr;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;
	//WindowsAPI
	WinApp* winApp_ = nullptr;
};