#pragma once
#include "Vector3.h"

class Easing
{
public:
	Easing();
	~Easing();
	Vector3 easeInVector3(double time,Vector3 stert,Vector3 end ,double endTime);
	double easeIn(double time, double stert, double end, double endTime);
	double easeInCubic(double x);
private:
	//�J�n���Ԃ��
	double time_;
	//�ŏ��̈ʒu���
	Vector3 stert_;
	//�Ō�̈ʒu���
	Vector3 end_;
	double endTime_;


};