#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>

class Road {
public:

	void Initialize();

	void BeforeUpdate();

	void Update();

	void Draw();

	void Reset();
private:
	int roadCount = 7;

	Object3d* road_[7] = { nullptr };
	Model* roadModel = nullptr;
	float speed = 1.0f;
	float beforSpeed = 0.2f;

};