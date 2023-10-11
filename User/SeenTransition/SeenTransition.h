#pragma once
#include "Sprite.h"
#include "SpriteCommon.h"

class SeenTransition
{
public:
	SeenTransition();
	~SeenTransition();

	void Initialize(SpriteCommon* spriteCommon_);

	void Update();

	void Draw();

	void OnSeenTrans();

	void Reset();

	bool ReturnSeenTrans() {
		return seenTimeFlag;
	}

private:
	bool seenTransFlag = false;
	bool seenTimeFlag = false;


	Sprite* blind;
	Sprite* blindFaceOver;
	Sprite* blindFaceUnder;

	SpriteCommon* spriteCommon = nullptr;

	bool blindFlag = false;
	int blindTime = 30;
	int blindTimeReset = 30;
	float blindW = 0;

	int randBlindOverNmb = 0;
	int randBlindUnderNmb = 0;
};