#pragma once
#include"DirectXCommon.h"

//スプライト共通部分
class SpriteCommon{
public:
	//初期化
	void Initialize(DirectXCommon* dxcommon);
private:
	DirectXCommon dxcommon_;

};