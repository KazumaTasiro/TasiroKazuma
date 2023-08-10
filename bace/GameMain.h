#pragma once
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "GameScene.h"
#include "PostEffect.h"
#include "FbxLoader.h"

//ゲーム全体
class GameMain
{
public:
	GameMain();
	~GameMain();

	//初期化処理
	void Inirialize();

	//終了
	void Finalize();

	//毎フレーム更新()
	void Update();

	//描画
	void Draw();

	//終了フラグのチェック
	bool isEndRequest() { return endRequest_; }

private:
	//WinApp
	WinApp* winApp = nullptr;
	//ゲーム終了フラグ
	bool endRequest_ = false;
	//DXcommon
	DirectXCommon* dxCommon = nullptr;
	//インプット
	Input* input = nullptr;
	//ポストエフェクト
	PostEffect* posteffect = nullptr;
	//FPS
	FPS* fps = new FPS;
	//GameScene
	GameScene* gameScene = nullptr;
	MSG msg{};//メッセージ
};