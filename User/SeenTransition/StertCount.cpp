#include "StertCount.h"


StertCount::StertCount()
{
}

StertCount::~StertCount()
{

}

void StertCount::Initialize()
{
	
	bossStertModel = Model::LoadFormOBJ("Sakaban");
	bossStert = new Object3d();
	bossStert->Initialize();
	bossStert->SetModel(bossStertModel);
	bossStert->wtf.scale = { BossScale };
	bossStert->wtf.position = { StertBossPos };
	camera_->SetTarget(bossStert->GetWorldPosition());
	easing = new Easing();
}

void StertCount::Update()
{
	time += 1;
	CameraUpdate();
}

void StertCount::Draw()
{
	if ( camera_->GetEye().z<=-300 )
	{
		bossStert->Draw();
	}
}

void StertCount::CameraUpdate()
{
	CameraEasing();
	camera_->Update();
}

void StertCount::BossUpdate()
{
}

void StertCount::CameraEasing()
{
	camera_->SetEye({	camera_->GetEye().x,camera_->GetEye().y,
						easing->easeOut(time,camera_->GetEye().z,
						bossStert->wtf.position.z - stertBossFrontCamera,endTime)
		});
}

void StertCount::Reset()
{
	time = 0;
}
