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
	//開始時間やで
	double time_;
	//最初の位置やで
	Vector3 stert_;
	//最後の位置やで
	Vector3 end_;
	double endTime_;


};