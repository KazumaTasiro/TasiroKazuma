#pragma once

#include <DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class PointLight
{
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
		Vector3 lightpos;
		float pad1;

		Vector3 lightcolor;
		float pad2;

		Vector3 lightatten;
		unsigned int active;
	};

public:

	inline void SetLightPos(const Vector3& lightPos) {
		this->lightpos = lightPos;
	}
	inline const Vector3& GetLightPos() { return lightpos; }
	inline void SetLightColor(const Vector3& lightColor) {
		this->lightcolor = lightColor;
	}
	inline const Vector3& GetLightColor() { return lightcolor; }
	inline void SetLightAtten(const Vector3& lightAttens) {
		this->lightAtten = lightAttens;
	}
	inline const Vector3& GetLightAtten() { return lightAtten; }
	inline void SetActive(bool actives) { this->active = actives; }
	inline bool IsActive() { return active; }



private:

	Vector3 lightpos = { 0,0,0 };
	Vector3 lightcolor = { 1,1,1 };
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	bool active = false;

};