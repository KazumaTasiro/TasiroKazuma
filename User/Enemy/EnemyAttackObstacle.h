#pragma once


class Enemy;
class EnemyAttackObstacle
{
public:
	EnemyAttackObstacle();
	~EnemyAttackObstacle();

/// <summary>
/// 初期化
/// </summary>
	void Initialize();

/// <summary>
/// 更新
/// </summary>
	void Update();

/// <summary>
/// 描画
/// </summary>
	void Draw();

/// <summary>
/// Enemyのセット
/// </summary>
	void SetEnemy(Enemy* enemy);

private:

	Enemy* enemy_ = nullptr;
};