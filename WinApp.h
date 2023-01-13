#pragma once
#include <Windows.h>

class WinApp {
public://�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:

	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static WinApp* GetInstance();

	void Initialize();

	HWND GetHwnd()const { return hwnd; }
	
	HINSTANCE GetHInstance()const { return w.hInstance; }

	//�E�B���h�E�T�C�Y
	static const int window_width = 1280;
	static const int window_height = 720;

	void Finalize();

	bool ProcessMessage();

private:
	//�E�B���h�E�n���h��
	HWND hwnd = nullptr;
	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
};