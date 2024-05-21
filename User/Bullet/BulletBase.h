#pragma once
#include "Object3d.h"
#include "Model.h"

class BulletBase
{
public:
	virtual void Initialize();

	virtual void Update();

	virtual void Draw();
protected:
	//ワールド変換データ
	Object3d* worldTransform_;
	//モデル
	Model* model_ = nullptr;

};