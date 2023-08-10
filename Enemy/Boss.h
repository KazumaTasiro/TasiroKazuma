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

class Boss
{
public:
	Boss();
	~Boss();

	///<summary>
	///初期化
	///</summary>
	void Initialize();
	///<summary>
	///更新
	///</summary>

	void Update();

	void Move();


	//void Fire();

	Vector3 GetWorldPosition();
	///<summary>
	///描画
	///</summary>
	void Draw();

private:

};
