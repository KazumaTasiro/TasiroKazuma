#pragma once

#include "GameSceneState.h"

class GameClearScene :public GameSceneState
{
public:
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//終了処理
	void Finalize() override;

private:

};