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
}

void StertCount::Update()
{
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
}

void StertCount::BossUpdate()
{
}
