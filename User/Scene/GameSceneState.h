#pragma once

class GameScene;

class GameSceneState
{
public:
	virtual void Initialize(GameScene* pGameScene) = 0;
	virtual void Update(GameScene* pGameScene) = 0;
	virtual void Draw(GameScene* pGameScene) = 0;
	virtual void showState(GameScene* pGameScene) = 0;


private:

};
