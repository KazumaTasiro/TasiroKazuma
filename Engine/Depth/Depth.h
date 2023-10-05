#pragma once
#include "DirectXCommon.h"
#include "WinApp.h"

class Depth
{
public:
	Depth();
	~Depth();

	void Initialize(WinApp* winApp,DirectXCommon* dxCommon);

private:
	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

};

