#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>


///<summary>
///天球
///</summary>
class Skydome {
public:
	///<summary>
	///初期化
	///</summary>
	void Initalize();

	///<summary>
	///更新
	///</summary>
	void Update();

	///<summary>
	///描画
	///</summary>
	void Draw();


private:
	//ワールド変換データ
	Object3d* worldTransform_ = nullptr;
	//モデル
	Model* model_ = nullptr;

};