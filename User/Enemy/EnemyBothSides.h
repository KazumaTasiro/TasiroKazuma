#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include <memory>
#include <list>
#include "Vector3.h"
#include "LockOnBullet.h"
#include "Sprite.h"
#include "Collision.h"
#include "SplinePosition.h"
#include "EnemyBullet.h"
#include "ParticleManager.h"

class Player;
class EnemyBothSides
{
public:
	EnemyBothSides();
	~EnemyBothSides();
	///<summary>
	///初期化
	///</summary>
	/// EnemyNmb 敵の行動の種類
	/// EnemyRootNmb 敵の行動ルート　0:左 1:右
	void Initialize(Vector3 EnemyPos,SpriteCommon* sptriteCommon,Model* model,Model* enemyBulletModel,Model* enemyReticleModel,int EnemyNmb,int EnemyRootNmb);
	///<summary>
	///更新
	///</summary>

	void Update(Player* player);
	///<summary>
	////敵の動き
	///</summary>
	void Move();

	///<summary>
	////敵の間の発射
	///</summary>
	void Fire();
	///<summary>
	////敵の座標
	///</summary>
	Vector3 GetWorldPosition();
	///<summary>
	///描画
	///</summary>
	void Draw();
	///<summary>
	///UI描画
	///</summary>
	void DrawUI();

	///<summary>
	///描画
	///</summary>
	void ParticleDraw();

private:

};