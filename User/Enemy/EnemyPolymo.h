#pragma once

class EnemyPolymo
{
public:
	EnemyPolymo();
	~EnemyPolymo();

	//初期化
	virtual void Initialize();

	//更新
	virtual void Update();

	//描画
	virtual void Draw();

private:
	
};