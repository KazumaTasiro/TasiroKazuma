#pragma once
#include <Windows.h>

class WinApp {
public://静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static WinApp* GetInstance();

	void Initialize();

	HWND GetHwnd()const { return hwnd; }
	
	HINSTANCE GetHInstance()const { return w.hInstance; }

	//ウィンドウサイズ
	static const int window_width = 1280;
	static const int window_height = 720;

	void Finalize();

	bool ProcessMessage();

private:
	//ウィンドウハンドル
	HWND hwnd = nullptr;
	//ウィンドウクラスの設定
	WNDCLASSEX w{};
};