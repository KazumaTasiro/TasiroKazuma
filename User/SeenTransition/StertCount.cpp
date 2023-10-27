#include "StertCount.h"



StertCount::StertCount()
{

}


StertCount::~StertCount()
{

}

void StertCount::Initialize(SpriteCommon* spriteCommon)
{
	assert(spriteCommon);
	spriteCommon_ = spriteCommon;
	Time3 = new Sprite();
	Time2 = new Sprite();
	Time1 = new Sprite();
	TimeGo = new Sprite();

	Time3->Initialize(spriteCommon_,13);
	Time2->Initialize(spriteCommon_,14);
	Time1->Initialize(spriteCommon_,15);
	TimeGo->Initialize(spriteCommon_,16);
	Time3->SetPozition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	Time2->SetPozition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	Time1->SetPozition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	TimeGo->SetPozition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	//bossStertModel = Model::LoadFormOBJ("Sakaban");
	//bossStert = new Object3d();
	//bossStert->Initialize();
	//bossStert->SetModel(bossStertModel);
	//bossStert->wtf.scale = { BossScale };
	//bossStert->wtf.position = { StertBossPos };
	camera_->SetTarget(PlayerPos);
	cameraPosTime = 1.0f / oneSecondTime;
	//easing = new Easing();
}

void StertCount::Update()
{
	time += 1;
	CameraUpdate();
}

void StertCount::Draw()
{
	if ( time < endTime )
	{
		if ( time >= CengeSeenTIme )
		{
			if ( nowCount == 0 )
			{
				Time3->Draw();
			}
			else if ( nowCount == 1 )
			{
				Time2->Draw();
			}
			else if ( nowCount == 2 )
			{
				Time1->Draw();
			}
		}
		if ( time > endTime - (CengeSeenTIme*4) )
		{
			TimeGo->Draw();
		}
	}
}

void StertCount::CameraUpdate()
{
	//CameraEasing();
	CameraMove();
	camera_->Update();
}

void StertCount::BossUpdate()
{
}

void StertCount::CameraEasing()
{
	//camera_->SetEye({	camera_->GetEye().x,camera_->GetEye().y,
	//					easing->easeOut(time,camera_->GetEye().z,
	//					bossStert->wtf.position.z - stertBossFrontCamera,endTime)
	//	});
}

void StertCount::CameraMove()
{

	if ( time < endTime )
	{
		time++;
	}
	else
	{
		cameraStertEnd = true;
	}
	if ( time >= CengeSeenTIme )
	{
		nowCount = static_cast< int >( ( time - CengeSeenTIme ) / oneSecondTime );
		if ( nowCount == 0 )
		{
			float cameraMoveAmount = RightCameraAfterPos.z - RightCameraPos.z;
			cameraPos;
			cameraPos += cameraPosTime * cameraMoveAmount;

			camera_->SetEye({ RightCameraPos.x, RightCameraPos.y,RightCameraPos.z + cameraPos });
		}
		else if ( nowCount == 1 )
		{
			float cameraMoveAmount = LeftCameraAfterPos.z - LeftCameraPos.z;
			cameraPos2 += cameraPosTime * cameraMoveAmount;

			camera_->SetEye({ LeftCameraPos.x, LeftCameraPos.y,LeftCameraPos.z + cameraPos2 });

		}
		else if ( nowCount == 2 )
		{
			float cameraMoveAmount = CenterCameraAfterPos.z - CenterCameraPos.z;
			cameraPos3 += cameraPosTime * cameraMoveAmount;

			camera_->SetEye({ CenterCameraAfterPos.x, CenterCameraAfterPos.y,CenterCameraPos.z + cameraPos3 });
		}
	}
}

void StertCount::Reset()
{
	time = 0;
	cameraPos3 = 0;
	cameraPos = 0;
	cameraPos2 = 0;
	cameraPos3 = false;
}
