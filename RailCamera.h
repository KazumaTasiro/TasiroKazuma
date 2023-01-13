#pragma once
#include "Object3d.h"
#include"Input/input.h"

///<summary>
/// 初期化
///</summary>
class RailCamera {
public:
	///<summary>
	/// 初期化
	///</summary>
	void Initalize();

	///<summary>
	/// 更新
	///</summary>
	void Update();

	XMFLOAT3 MatVector(XMMATRIX matrix4, XMFLOAT3 vector3);


	void Reset();

private:
	//ワールド変換データ
	Object3d* worldTransform_=nullptr;

	//キー入力
	Input* input_ = nullptr;

};
