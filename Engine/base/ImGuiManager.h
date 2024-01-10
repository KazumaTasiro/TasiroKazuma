#pragma once
#include <imgui.h>
#include "WinApp.h"
#include <d3dx12.h>
#include "DirectXCommon.h"


//ImGUIの管理
class ImGuiManager
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxcommon);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Bigin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	void Draw();

	void MySaveFunction();

	static ImGuiManager* GetInstance();
private:

	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(const ImGuiManager&) = delete;
	const ImGuiManager& operator=(const ImGuiManager&) = delete;
private:
	static ImGuiManager* ImGuiManager_;
private:
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxcommon_ = nullptr;

	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

};