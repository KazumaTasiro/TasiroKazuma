#pragma once
#include "Sprite.h"
#include "SpriteCommon.h"


///<summary>
///画面推移の生成と動き
///</summary>
class SeenTransition
{
public:
	SeenTransition();
	~SeenTransition();
	///<summary>
	////初期化
	///</summary>
	void Initialize(SpriteCommon* spriteCommon_);
	///<summary>
	////更新
	///</summary>
	void Update();
	///<summary>
	////描画
	///</summary>
	void Draw();
	///<summary>
	////ブラインド開始
	///</summary>
	void OnSeenTrans();
	///<summary>
	////リセット
	///</summary>
	void Reset();
	///<summary>
	////ブラインド終わり
	///</summary>
	bool ReturnSeenTrans() {
		return seenTimeFlag;
	}
	///<summary>
	////ブラインド終わり
	///</summary>
	bool ReturnSeenNotEnd() {
		return seenNotEndFlag;
	}
	///<summary>
	////画像のランダムリセット
	///</summary>
	void ResetBlindSprite();

private:
	enum Nmb
	{
		zero,
		one,
		two,
		three,
		four,
		five,
		six,
		seven,
		eight,
		nine,
		ten,

	};
private://ブラインド
	bool seenTransFlag = false;
	bool seenTimeFlag = false;
	bool seenNotEndFlag = false;


	Sprite* blind;
	Sprite* blindFaceOver;
	Sprite* blindFaceUnder;

	SpriteCommon* spriteCommon = nullptr;

	bool blindFlag = false;
	int blindTime = 30;
	int blindTimeReset = 30;
	float blindW = 0;
	int blindLoadTime = 10;
	float blindResetPos = 720.0f;

	int randBlindOverNmb = 0;
	int randBlindUnderNmb = 0;

	int zeroNmb = 0;
	int tenNmb = 10;
};