#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "ModelManager.h"

///<summary>
///タイトルのオブジェクトの生成と動き
///</summary>
class Titles
{
public:
	Titles();
	~Titles();

	///<summary>
	///初期化
	///</summary>
	void Initialize();
	///<summary>
	///更新
	///</summary>
	void Update();
	///<summary>
	///描画
	///</summary>
	void Draw();
	///<summary>
	///リセット
	///</summary>
	void Reset();

	//プレイヤーの行動制限
	void PlayerLimit();
	///<summary>
	///ゲーム移行するときに使う関数
	///</summary>
	void GameStertTrans();

private://タイトルとその他

	float PI = 3.141592f;


	//ロゴ
	Object3d* titleLogo = nullptr;
	Model* titleLogoModel = nullptr;
	//ロゴスケール
	Vector3 LogoScale = { 1.5f,1.5f,1.5f };
	//ロゴポジション
	Vector3 TitleLogoPos = { 0,4, -10 };
	Vector3 LogoResetPos = { 0,4,-10 };
	Vector3 LogoRot = { 0,( PI / 180 ) * 1,0 };

	float LogoRotNum = 0;
	float LogoRotMax = 15;
	float LogoRotPlus = 0.5f;

	float rot = ( PI / 180 );

private://プレイヤー

	//タイトルのプレイヤー
	Object3d* titlePlayer = nullptr;
	Model* titlePlayerModel = nullptr;
	//プレイヤーのスケール
	Vector3 PlayerScale = { 1.5f,1.5f,1.5f };
	//プレイヤーのポジション
	Vector3 TitlePlayerPos = { 0,-2.5f, 0 };
	Vector3 TitlePlayerResetPos = { 0,-2.5f, 0 };
	//移動時のプレイヤーの回転
	float TitleRotReset = 20;
	Vector3 playerRot = { 0,( PI / 180 ) * 1,0 };
	float playerRotNum = 0;
	float playerRotConst = 20;
	float playerRotPlus = 2;
	//プレイヤーのポジション限界
	float playerMoveRimit = 1.5f;
	float playerSpeed = 0.1f;
	//どちらの壁に触れたか
	bool playerWallFlagX = false;
	bool playerWallFlagY = false;

private://ボス


	//タイトルのボス
	Object3d* titleBoss = nullptr;
	Model* titleBossModel = nullptr;
	//ボススケール
	Vector3 BossScale = { 20,20,20 };
	//ボスポジション
	Vector3 TitleBossPos = { 0,4, -50 };
	Vector3 TitleResetBossPos = { 0,4,-50 };

	//ボスの重力関係
	float gravity = 0.1f;
	float defUpSpeed = 1.0f;
	float upSpeed = 1.0f;
	float posLimit = 1.0f;
	float posSpeed = 0.1f;
	bool gravityStert = true;
	int defGravityStertTime = 3;
	int gravityStertTime = 3;
	int gravityEndTime = 0;
	float bossNear = 1.0f;

	//ゲームスタートフラグ
	bool bossGoFlag = false;


};
