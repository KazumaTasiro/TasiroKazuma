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

	Time3->Initialize(spriteCommon_,Tex::threeSp);
	Time2->Initialize(spriteCommon_,Tex::twoSp);
	Time1->Initialize(spriteCommon_,Tex::oneSp);
	TimeGo->Initialize(spriteCommon_,Tex::goSp);
	Time3->SetPozition({ WinApp::window_width / GoNmb::two,WinApp::window_height / GoNmb::two });
	Time2->SetPozition({ WinApp::window_width / GoNmb::two,WinApp::window_height / GoNmb::two });
	Time1->SetPozition({ WinApp::window_width / GoNmb::two,WinApp::window_height / GoNmb::two });
	TimeGo->SetPozition({ WinApp::window_width / GoNmb::two,WinApp::window_height / GoNmb::two });
	//bossStertModel = Model::LoadFormOBJ("Sakaban");
	//bossStert = new Object3d();
	//bossStert->Initialize();
	//bossStert->SetModel(bossStertModel);
	//bossStert->wtf.scale = { BossScale };
	//bossStert->wtf.position = { StertBossPos };
	camera_->SetTarget(PlayerPos);
	cameraPosTime = 1.0f / oneSecondTime;
	//easing = new Easing();
	CameraUpdate();
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
			if ( nowCount == GoNmb::zero )
			{
				Time3->Draw();
			}
			else if ( nowCount == GoNmb::one )
			{
				Time2->Draw();
			}
			else if ( nowCount == GoNmb::two )
			{
				Time1->Draw();
			}
		}
		if ( time > endTime - ( CengeSeenTIme * GoNmb::three ) )
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
		if ( nowCount == GoNmb::zero )
		{
			float cameraMoveAmount = RightCameraAfterPos.z - RightCameraPos.z;
			cameraPos;
			cameraPos += cameraPosTime * cameraMoveAmount;

			camera_->SetEye({ RightCameraPos.x, RightCameraPos.y,RightCameraPos.z + cameraPos });
		}
		else if ( nowCount == GoNmb::one )
		{
			float cameraMoveAmount = LeftCameraAfterPos.z - LeftCameraPos.z;
			cameraPos2 += cameraPosTime * cameraMoveAmount;

			camera_->SetEye({ LeftCameraPos.x, LeftCameraPos.y,LeftCameraPos.z + cameraPos2 });

		}
		else if ( nowCount == GoNmb::two )
		{
			goStert = true;
			float cameraMoveAmount = CenterCameraAfterPos.z - CenterCameraPos.z;
			cameraPos3 += cameraPosTime * cameraMoveAmount;

			camera_->SetEye({ CenterCameraAfterPos.x, CenterCameraAfterPos.y,CenterCameraPos.z + cameraPos3 });
		}
		else
		{

		}
	}
}

void StertCount::Reset()
{
	time = GoNmb::zero;
	cameraPos3 = GoNmb::zero;
	cameraPos = GoNmb::zero;
	cameraPos2 = GoNmb::zero;
	cameraStertEnd = false;
	goStert = false;
}
