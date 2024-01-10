#pragma once
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "FbxLoader.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "LightData.h"

//ゲーム全体
class Framework
{
public://メンバ関数

	//実行
	void Run();


	virtual ~Framework() = default;

	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//毎フレーム処理
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	//終了チェック
	virtual bool IsEndRequst() { return endRequest_; }


	WinApp* GetWinApp() { return winApp; }
	DirectXCommon* GetDXCommon() { return dxCommon; }
	Input* GetInput() { return input; }

private:
	//ゲーム終了フラグ
	bool endRequest_ = false;
	//WinApp
	WinApp* winApp = nullptr;

	//DXcommon
	DirectXCommon* dxCommon = nullptr;
	//インプット
	Input* input = nullptr;
	//ImGui
	ImGuiManager* imGuiManager_ = nullptr;
	//FPS
	FPS* fps = new FPS;
	//メッセージ
	MSG msg{};
	//ライト
	LightGroup* light = nullptr;

	LightData* lightData = nullptr;
};