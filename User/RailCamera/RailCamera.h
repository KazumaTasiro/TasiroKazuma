#pragma once
#include "Camera.h"
#include "Object3d.h"
#include "SplinePosition.h"
#include "Input.h"

class Player;
class RailCamera
{
public:
	RailCamera();
	~RailCamera();

	void Initialize();

	void Update();

	void SetCamera(Camera* camera) {
		camera_ = camera;
	}
	void SetPlayer(Player* player) {
		player_ = player;
	}
	void Reset();


private:
	enum CameraPos
	{
		Back,
		Right,
		Front,
		Left
	};
	enum MovePos
	{
		BackRight,
		FrontRight,
		FrontLeft,
		BackLeft
	};
	Camera* camera_ = nullptr;
	Player* player_ = nullptr;
	Object3d* cameraPos[ 4 ] = { nullptr };
	Object3d* cameraMovePos[ 4 ] = { nullptr };


	Vector3 cameraBack = { 0,0,-10 };
	Vector3 cameraRight = { 10,0,0 };
	Vector3 cameraLeft = { -10,0,0 };
	Vector3 cameraFront = { 0,0,10 };

	Vector3 backLeft = { -10,0,-10 };
	Vector3 backRight = { 10,0,-10 };
	Vector3 frontLeft = { -10,0,10 };
	Vector3 frontRight = { 10,0,10 };

	SplinePosition* splineRight[ 4 ] = { nullptr };
	SplinePosition* splineLeft[ 4 ] = { nullptr };

	Input* input_ = nullptr;

	float splineTime = 0.1f;

	int nowPos = Back;
	int nowPosPlus = Right;
	int nowPosMinus = Left;

	bool cameraMoveFlag = false;
	bool cameraMoveFlagR = false;
	bool cameraMoveFlagL = false;
};