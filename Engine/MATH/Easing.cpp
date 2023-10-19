#include "Easing.h"

Easing::Easing()
{
}

Easing::~Easing()
{
}

Vector3 Easing::easeInVector3(double time, Vector3 stert, Vector3 end, double endTime)
{
	time_ = time / endTime;
	double v = easeInCubic(time);
	Vector3 ret = (end *static_cast<float> (v) + stert);
	return ret;
}


double Easing::easeIn(double time, double stert, double end, double endTime)
{
	time_ = time / endTime;
	double v = easeInCubic(time_);
	double ret = end * static_cast<float> (v) + stert;
	return ret;
}

double Easing::easeInCubic(double x)
{
	return x * x * x;
}

double Easing::easeOut(double time,double stert,double end,double endTime)
{
	time_ = time / endTime;
	double v = easeOutCubic(time_);
	double ret = end * static_cast< float > ( v ) + stert;
	return ret;
}

double Easing::easeOutCubic(double x)
{
	if ( time_ < 0.5 )
	{
		return x * x * x * x;
	}
	else
	{
		return (1 - ( pow(-2 * x + 2,3) / 2 ));
	}
	
}
