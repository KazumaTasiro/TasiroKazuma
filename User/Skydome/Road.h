#pragma once
#include "Object3d.h"
#include "Model.h"
#include "LightData.h"
#include <cassert>

///<summary>
///ボスの生成と行動
///</summary>
class Road {
public:
	///<summary>
	////初期化
	///</summary>
	void Initialize();
	///<summary>
	////初期化更新
	///</summary>
	void BeforeUpdate();
	///<summary>
	////更新
	///</summary>
	void Update();
	///<summary>
	////更新
	///</summary>
	void roadUpdate();
	///<summary>
	////描画
	///</summary>
	void Draw();
	///<summary>
	////リセット
	///</summary>
	void Reset();
private:
	int roadCount = 7;

	Object3d* road_[7] = { nullptr };
	Model* roadModel = nullptr;
	Object3d* Bill_[7] = { nullptr };
	Model* BillModel = nullptr;
	float speed = 1.0f;
	float beforSpeed = 0.2f;

	Vector3 RoasdSize = { 6,2,2 };
	Vector3 pos= { 0,-2.5f,-50.0f };
	int posTrans = 50;

	float posMove = -100.0f + ( 49.0f * roadCount );
	float posLimit = -150.0f;
};