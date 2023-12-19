#pragma once

#include <DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

/// <summary>
/// スポットライト
/// </summary>
class SpotLight {
private://エイリアス
	//Microsoft::WRL::を省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	////DirecX::を省略
	//using Vector2 = DirectX::XMFLOAT2;
	//using Vector3 = DirectX::XMFLOAT3;
	//using Vector4 = DirectX::XMFLOAT4;
	//using Vector4 = DirectX::XMVECTOR;
	//using Matrix4 = DirectX::XMMATRIX;


public://サブクラス
	struct ConstBufferData
	{
		Vector4 lightv;
		Vector3 lightpos;
		float pad1;
		Vector3 lightcolor;
		float pad2;
		Vector3 lightatten;
		float pad3;
		Vector2 lightfactoranglecos;
		unsigned int active;
		float pad4;
		
	};

public:
	//メンバ関数

	inline void SetLightDir(const Vector4& lightdirs) { this->lightdir =lightdirs.Normalization(); }
	inline const Vector4& GetLightDir() { return lightdir; }
	inline void SetLightPos(const Vector3& lightPos) { this->lightpos = lightPos; }
	inline const Vector3& GetLightPos() { return lightpos; }
	inline void SetLightColor(const Vector3& lightColor) { this->lightcolor = lightColor; }
	inline const Vector3& GetLightColor() { return lightcolor; }
	inline void SetLightAtten(const Vector3& lightAttens) { this->lightAtten = lightAttens; }
	inline const Vector3& GetLightAtten() { return lightAtten; }
	inline void SetLightFactorAngle(const Vector2& lightFactorAngles) {
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngles.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngles.y));
	}
	inline const Vector2& GetLightFactorAngleCos() { return lightFactorAngleCos; }
	inline void SetActive(bool actives) { this->active = actives; }
	inline bool IsActive() { return active; }


private:

	//ライトの方向
	Vector4 lightdir = { 1,0,0,0 };
	//ライト座標
	Vector3 lightpos = { 0,0,0 };
	//ライトの色
	Vector3 lightcolor = { 1,1,1 };
	//ライトの距離減衰係数
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	//ライト減衰角度
	Vector2 lightFactorAngleCos = { 0.5f,0.2f };
	//有効フラグ
	bool active = false;


};