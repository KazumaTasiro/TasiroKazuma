#include "RailCamera.h"
#include "Player.h"

RailCamera::RailCamera()
{
}

RailCamera::~RailCamera()
{
}

void RailCamera::Initialize()
{
	input_ = Input::GetInstance();

	for ( int i = 0; i < 4; i++ )
	{
		cameraPos[ i ] = Object3d::Create();
		cameraPos[ i ]->Initialize();
		cameraMovePos[ i ] = Object3d::Create();
		cameraMovePos[ i ]->Initialize();

	}


	cameraPos[ Back ]->wtf.position = cameraBack;
	cameraPos[ Left ]->wtf.position = cameraLeft;
	cameraPos[ Front ]->wtf.position = cameraFront;
	cameraPos[ Right ]->wtf.position = cameraRight;

	cameraMovePos[ BackRight ]->wtf.position = backRight;
	cameraMovePos[ FrontRight ]->wtf.position = frontRight;
	cameraMovePos[ FrontLeft ]->wtf.position = frontLeft;
	cameraMovePos[ BackLeft ]->wtf.position = backLeft;

	splineRight[ BackRight ] = new SplinePosition(cameraBack,backRight,cameraRight);
	splineRight[ FrontRight ] = new SplinePosition(cameraRight,frontRight,cameraFront);
	splineRight[ FrontLeft ] = new SplinePosition(cameraFront,frontLeft,cameraLeft);
	splineRight[ BackLeft ] = new SplinePosition(cameraLeft,backLeft,cameraBack);
	
	splineLeft[ BackRight ] = new SplinePosition(cameraRight,backRight,cameraBack);
	splineLeft[ FrontRight ] = new SplinePosition(cameraFront,frontRight,cameraRight);
	splineLeft[ FrontLeft ] = new SplinePosition(cameraLeft,frontLeft,cameraFront);
	splineLeft[ BackLeft ] = new SplinePosition(cameraBack,backLeft,cameraLeft);

	for ( int i = 0; i < 4; i++ )
	{
		splineLeft[ i ]->Update(splineTime);
		splineRight[ i ]->Update(splineTime);
	}

	for ( int i = 0; i < 4; i++ )
	{
		splineRight[ i ]->Reset();
		splineLeft[ i ]->Reset();
	}

}

void RailCamera::Update()
{
	if ( cameraMoveFlag == false )
	{
		if ( input_->TriggerKey(DIK_Q) )
		{
			nowPos += 1;
			nowPosPlus += 1;
			nowPosMinus += 1;
			if ( nowPos > 3 )
			{
				nowPos = Back;
			}
			else if ( nowPosPlus > 3 )
			{
				nowPosPlus = Back;
			}
			else if ( nowPosMinus > 3 )
			{
				nowPosMinus = Back;
			}

			cameraMoveFlag = true;
			cameraMoveFlagR = true;
		}

		if ( input_->TriggerKey(DIK_E) )
		{
			nowPos -= 1;
			nowPosPlus -= 1;
			nowPosMinus -= 1;
			if ( nowPos < 0 )
			{
				nowPos = Left;
			}
			else if ( nowPosPlus < 0 )
			{
				nowPosPlus = Left;
			}
			else if ( nowPosMinus < 0 )
			{
				nowPosMinus = Left;
			}
			cameraMoveFlag = true;
			cameraMoveFlagL = true;
		}
	}
	if ( cameraMoveFlag )
	{

		if ( nowPos == 0 )
		{
			if ( cameraMoveFlagR )
			{
				camera_->SetEye(splineRight[ BackLeft ]->NowPos);
			}
			if ( cameraMoveFlagL )
			{
				camera_->SetEye(splineLeft[ BackRight ]->NowPos);
			}
			if ( camera_->GetEye().x == cameraBack.x && camera_->GetEye().z == cameraBack.z )
			{
				cameraMoveFlag = false;
				cameraMoveFlagL = false;
				cameraMoveFlagR = false;
			}

		}
		else if ( nowPos == 1 )
		{
			if ( cameraMoveFlagR )
			{
				camera_->SetEye(splineRight[ BackRight ]->NowPos);
			}
			if ( cameraMoveFlagL )
			{
				camera_->SetEye(splineLeft[ FrontRight ]->NowPos);
			}
			if ( camera_->GetEye().x == cameraRight.x && camera_->GetEye().z == cameraRight.z )
			{
				cameraMoveFlag = false;
				cameraMoveFlagL = false;
				cameraMoveFlagR = false;
			}
		}
		else if ( nowPos == 2 )
		{
			if ( cameraMoveFlagR )
			{
				camera_->SetEye(splineRight[ FrontRight ]->NowPos);
			}
			if ( cameraMoveFlagL )
			{
				camera_->SetEye(splineLeft[ FrontLeft ]->NowPos);
			}
			if ( camera_->GetEye().x == cameraFront.x && camera_->GetEye().z == cameraFront.z )
			{
				cameraMoveFlag = false;
				cameraMoveFlagL = false;
				cameraMoveFlagR = false;
			}
		}
		else if ( nowPos == 3 )
		{
			if ( cameraMoveFlagR )
			{
				camera_->SetEye(splineRight[ FrontLeft ]->NowPos);
			}
			if ( cameraMoveFlagL )
			{
				camera_->SetEye(splineLeft[ BackLeft ]->NowPos);
			}
			if ( camera_->GetEye().x == cameraLeft.x && camera_->GetEye().z == cameraLeft.z )
			{
				cameraMoveFlag = false;
				cameraMoveFlagL = false;
				cameraMoveFlagR = false;
			}
		}
		for ( int i = 0; i < 4; i++ )
		{
			splineLeft[ i ]->Update(splineTime);
			splineRight[ i ]->Update(splineTime);
		}
	}
	else
	{
		for ( int i = 0; i < 4; i++ )
		{
			splineRight[ i ]->Reset();
			splineLeft[ i ]->Reset();
			splineLeft[ i ]->Update(splineTime);
			splineRight[ i ]->Update(splineTime);
		}

	}
	for ( int i = 0; i < 4; i++ )
	{
		cameraPos[ i ]->Update();
	}
}

void RailCamera::Reset()
{
	for ( int i = 0; i < 4; i++ )
	{
		splineLeft[ i ]->Reset();
	}
	nowPos = Back;
	cameraMoveFlag = false;
	cameraMoveFlagL = false;
	cameraMoveFlagR = false;
	camera_->SetEye(cameraBack);
}
