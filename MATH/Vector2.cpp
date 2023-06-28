#include "Vector2.h"
#include <cmath>

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

float Vector2::length()const {
	return sqrt(x * x + y * y);
}

Vector2& Vector2::nomalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

float Vector2::dot(const Vector2& v) const {
	return (this->x * v.x) + (this->y * v.y);
}

float Vector2::cross(const Vector2& v)const {
	return (this->x * v.y) - (this->y * v.x);
}

Vector2 Vector2::operator+()const {
	return *this;
}

Vector2 Vector2::operator-()const {
	Vector2 temp = { 0,0 };
	temp.x = -(*this).x;
	temp.y = -(*this).y;
	return temp;         ////
}

Vector2& Vector2::operator+=(const Vector2& v) {
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v) {
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s) {
	this->x *= s;
	this->y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s) {
	this->x /= s;
	this->y /= s;
	return *this;
}

//Vector2D�@�N���X�ɑ����Ȃ��֐��Q
//�񍀉��Z�q
const Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	temp.x += v2.x;
	temp.y += v2.y;
	return temp;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	temp.x -= v2.x;
	temp.y -= v2.y;
	return temp;
}

const Vector2 operator*(const Vector2& v, float s) {
	Vector2 temp(v);
	temp.x = temp.x * s;
	temp.y = temp.y * s;
	return temp;
}

const Vector2 operator*(float s, const Vector2& v) {
	Vector2 temp = { 0,0};
	temp.x = s * v.x;
	temp.y = s * v.y;
	return temp;
}

const Vector2 operator/(const Vector2& v, float s) {
	Vector2 temp(v);
	return temp /= s;
}