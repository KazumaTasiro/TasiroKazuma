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
	///èâä˙âª
	///</summary>
	void Initialize();
	///<summary>
	///çXêV
	///</summary>

	void Update();

	void Move();


	//void Fire();

	Vector3 GetWorldPosition();
	///<summary>
	///ï`âÊ
	///</summary>
	void Draw();

private:

};
