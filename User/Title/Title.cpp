#include "Title.h"

Titles::Titles()
{
}

Titles::~Titles()
{
}

void Titles::Initialize()
{
	titleBossModel = Model::LoadFormOBJ("Sakaban");
	titleBoss = new Object3d();
	titleBoss->Initialize();
	titleBoss->SetModel(titleBossModel);
	titleBoss->wtf.scale = { BossScale };
	titleBoss->wtf.position = { TitleBossPos };
}

void Titles::Update()
{
	if (gravityStert) {
		titleBoss->wtf.position.y += upSpeed;
		upSpeed -= gravity;
		if (titleBoss->wtf.position.y <= -posLimit + TitleBossPos.y) {
			upSpeed = defUpSpeed;
			titleBoss->wtf.position.y = TitleBossPos.y;
			gravityStert = false;
		}
	}
	else
	{
		gravityStertTime--;
		if (gravityStertTime <= 0) {
			gravityStert = true;
			gravityStertTime = defGravityStertTime;
		}
	}

	titleBoss->Update();
}

void Titles::Draw()
{
	titleBoss->Draw();
}

void Titles::Reset()
{
	titleBoss->wtf.position = { TitleBossPos };
	titleBoss->Update();
	if (posSpeed < 0) {
		posSpeed = -posSpeed;
	}
}
