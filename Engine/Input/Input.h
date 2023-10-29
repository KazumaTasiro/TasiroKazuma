#pragma once
#define DIRECTINPUT_VERSION  0x0800//DirectInputのバージョン指定
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include <windows.h>
#include "WinApp.h"


using namespace Microsoft::WRL;

class Input {
public:
	//namespace省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバー関数
	//シングルトンインスタンス
	static Input* GetInstance();

	//初期化
	void Initalize();
	//更新
	void Update();

	//キーを押したかをチェック
	bool PushKey(BYTE keyNunber);
	//キーのトリガーをチェック
	bool TriggerKey(BYTE keyNunber);

	//キーを押したかをチェック
	//0　左クリック
	//1　右クリック
	//2　マウスカーソルクリック
	bool PushMouse(int mouse_);
	//キーのトリガーをチェック
	//0　左クリック
	//1　右クリック
	//2　マウスカーソルクリック
	bool TriggerMouse(int mouse_);
	//マウスのリリースをチェック
	//0　左クリック
	//1　右クリック
	//2　マウスカーソルクリック
	bool ReleaseMouse(int mouse_);
private://メンバ変数

	Input() = default;
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;

private:
	static Input* Input_;
private:
	//キーボードのデバイス
	IDirectInputDevice8* keyboard = nullptr;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	//マウスのデバイス
	IDirectInputDevice8* mouse = nullptr;
	DIMOUSESTATE CurrentMouseState;		//!< マウスの現在の入力情報
	DIMOUSESTATE PrevMouseState;			//!< マウスの一フレーム前の入力情報


	//DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;
	//WindowsAPI
	WinApp* winApp_ = nullptr;
};
