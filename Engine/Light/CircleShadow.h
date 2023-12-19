#pragma once

#include<DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

/// <summary>
/// 丸影
/// </summary>
class CircleShadow {

private://エイリアス
	////Microsoft::WRL::を省略
	//template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	////DirecX::を省略
	//using Vector2 = DirectX::XMFLOAT2;
	//using Vector3 = DirectX::XMFLOAT3;
	//using Vector4 = DirectX::XMFLOAT4;
	//using Vector4 = DirectX::XMVECTOR;
	//using Matrix4 = DirectX::XMMATRIX;

public:
	//サブクラス
	struct ConstBufferData {
		Vector4 dir;
		Vector3 casterPos;
		float distanceCasterLight;
		Vector3 atten;
		float pad3;
		Vector2 factorAngleCos;
		unsigned int active;
		float pad4;
	};
	
public:
	//メンバ関数

	inline void SetDir(const Vector4& dirs) { this->dir = dirs.Normalization(); }
	inline const Vector4& GetDir() { return dir; }

	inline void SetCasterPos(const Vector3& casterpos) { this->casterPos = casterpos; }
	inline const Vector3& GetCasterPos() { return casterPos; }

	inline void SetDistanceCasterLight(float distanceCasterLights) { this->distanceCasterLight = distanceCasterLights; }
	inline float GetDistanceCasterLight() { return distanceCasterLight; }

	inline void SetAtten(const Vector3& attens) { this->atten = attens; }
	inline const Vector3& GetAtten() { return atten; }

	inline void SetFactorAngle(const Vector2& factorAngles) {
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngles.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngles.y));
	}

	inline const Vector2& GetFactorAngleCos() { return factorAngleCos; }
	inline void SetActive(bool actives) { this->active = actives; }
	inline bool IsActive() { return active; }

private:
	//方向
	Vector4 dir = { 1,0,0,0 };

	float distanceCasterLight = 100.0f;

	Vector3 casterPos = { 0,0,0 };

	Vector3 atten = { 0.5f,0.6f,0.0f };

	Vector2 factorAngleCos = { 0.2f,0.5f };

	bool active = false;

};