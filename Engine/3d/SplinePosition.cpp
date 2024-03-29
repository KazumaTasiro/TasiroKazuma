#include "SplinePosition.h"

SplinePosition::SplinePosition(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end)
{
	std::vector<Vector3> points_{ Start, Start, p1, p2, end, end };
	this->points = points_;
}


SplinePosition::SplinePosition(Vector3& Start,Vector3& p1,Vector3& end)
{
	std::vector<Vector3> points_{ Start, Start, p1, p1, end, end };
	this->points = points_;
}

SplinePosition::SplinePosition(const std::vector<Vector3>& points)
{
	this->points = points;
}

SplinePosition::~SplinePosition() {}

void SplinePosition::Update(float& time)
{
	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < points.size() - 3) {

			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
		}
	}
	NowPos = SplinePositionUpdate(points, startIndex, timeRate_);
}

void SplinePosition::Reset()
{
	startIndex = 1;
	timeRate_ = 0.0f;
}

Vector3 SplinePosition::SplinePositionUpdate(const std::vector<Vector3>& points_, size_t& startIndex_, float& t)
{
	size_t n = points_.size() - 2;

	if (startIndex_ > n)return points_[n];
	if (startIndex_ < 1)return points_[1];

	Vector3 p0 = points_[startIndex_ - 1];
	Vector3 p1 = points_[startIndex_];
	Vector3 p2 = points_[startIndex_ + 1];
	Vector3 p3 = points_[startIndex_ + 2];

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) + (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}
