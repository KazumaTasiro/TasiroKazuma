#pragma once
#include <memory>
#include <list>
#include <cassert>
#include "EnemyBullet.h"
#include "Model.h"

class Enemy;
class Player;
class EnemyBulletAttack
{
public:
	//EnemyBulletAttack();
	//~EnemyBulletAttack();


/// <summary>
/// 初期化
/// </summary>
	void Initialize(Model* bulletModel);


/// <summary>
/// 更新
/// </summary>
	void Update(Enemy* enemy);



/// <summary>
/// 描画
/// </summary>
	void Draw();

	void Move();

	void Attck();

	void OnColl();

private:
		//敵の弾モデル
	Model* enemyBulletModel_ = nullptr;

	Enemy* enemy_ = nullptr;

	Player* player_ = nullptr;
	float playerWidth = 1.5f;
	float enemyWidth_ = 1.5f;

	float move = 0.1f;

	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;


	Vector3 velocity_;
	float verocitySpeed = 3.0f;
};