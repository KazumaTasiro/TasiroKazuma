#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>

class Titles
{
public:
	Titles();
	~Titles();

	void Initialize();

	void Update();

	void Draw();

	void Reset();

private:

	Object3d* titleBoss = nullptr;
	Model* titleBossModel = nullptr;
	Vector3 BossScale = { 30,30,30 };
	Vector3 TitleBossPos = { 0,4, -50 };
	float gravity = 0.5f;
	float defUpSpeed = 3.0f;
	float upSpeed = 3.0f;
	float posLimit = 2.0f;
	float posSpeed = 0.1f;
	bool gravityStert = true;
	int defGravityStertTime = 3;
	int gravityStertTime = 3;
};
