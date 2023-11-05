#pragma once


#include <DirectXMath.h>

class Vector3
{
public:


	float x;
	float y;
	float z;

public:


	Vector3();
	Vector3(float x, float y, float z);


	float length() const;
	Vector3& nomalize();
	float dot(const Vector3& v) const;
	Vector3 cross(const Vector3& v) const;
	static const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);
	static const Vector3 slerp(Vector3& stert,Vector3& end, float t);

	static Vector3 Vector3Zero();
	static bool Vector3Equal(Vector3 v1,Vector3 v2);
	bool Vector3IsInfinite();

	// 球面線形補間関数
	// out   : 補間ベクトル（出力）
	// start : 開始ベクトル
	// end : 終了ベクトル
	// t : 補間値（0～1）
	Vector3 SphereLinear(Vector3& out,Vector3& start,Vector3& end,float t);

	Vector3 operator+() const;
	Vector3 operator-() const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	//Vector3& operator=(const XMFLOAT3& v);
	//XMFLOAT3& operator=(const Vector3& v);
};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

