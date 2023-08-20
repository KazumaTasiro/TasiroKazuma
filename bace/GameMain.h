#pragma once

#include "GameScene.h"
#include "PostEffect.h"
#include "Framework.h"

//ゲーム全体
class GameMain:public Framework
{
public:
	GameMain();
	~GameMain();

	//初期化処理
	void Initialize() override;

	//終了
	void Finalize() override;

	//毎フレーム更新()
	void Update() override;

	//描画
	void Draw() override;


private:
	
	//ポストエフェクト
	PostEffect* posteffect = nullptr;
	
	//GameScene
	GameScene* gameScene = nullptr;

};