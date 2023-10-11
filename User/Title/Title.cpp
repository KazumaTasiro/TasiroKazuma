#include "Title.h"

Titles::Titles()
{
}

Titles::~Titles()
{
}

void Titles::Initialize()
{
	//タイトルにいるサカバン
	titleBossModel = Model::LoadFormOBJ("Sakaban");
	titleBoss = new Object3d();
	titleBoss->Initialize();
	titleBoss->SetModel(titleBossModel);
	titleBoss->wtf.scale = { BossScale };
	titleBoss->wtf.position = { TitleBossPos };
	//タイトルロゴ
	titleLogoModel = Model::LoadFormOBJ("Title");
	titleLogo = new Object3d();
	titleLogo->Initialize();
	titleLogo->SetModel(titleLogoModel);
	titleLogo->wtf.scale = { LogoScale };
	titleLogo->wtf.position = { TitleLogoPos };
	//タイトルプレイヤー
	titlePlayerModel = Model::LoadFormOBJ("trakku");
	titlePlayer = new Object3d();
	titlePlayer->Initialize();
	titlePlayer->SetModel(titlePlayerModel);
	titlePlayer->wtf.position = { TitlePlayerPos };
	playerRotNum = TitleRotReset;
	titlePlayer->wtf.rotation.y = ( PI / 180 ) * playerRotNum;
}


void Titles::Update()
{
	LogoRotNum += LogoRotPlus;
	LogoRot.y = ( PI / 180 ) * LogoRotNum;
	titleLogo->wtf.rotation = LogoRot;
	if ( LogoRotNum >= 15 || LogoRotNum <= -15 )
	{
		LogoRotPlus = -LogoRotPlus;
	}

	if ( gravityStert )
	{
		titleBoss->wtf.position.y += upSpeed;
		upSpeed -= gravity;
		if ( titleBoss->wtf.position.y <= -posLimit + TitleBossPos.y )
		{
			upSpeed = defUpSpeed;
			titleBoss->wtf.position.y = TitleBossPos.y;
			gravityStert = false;
		}
	}
	else
	{
		gravityStertTime--;
		if ( gravityStertTime <= 0 )
		{
			gravityStert = true;
			gravityStertTime = defGravityStertTime;
		}
	}
	PlayerLimit();
	titleLogo->Update();
	titleBoss->Update();

}

void Titles::Draw()
{
	titleBoss->Draw();
	titleLogo->Draw();
	titlePlayer->Draw();
}

void Titles::Reset()
{
	titleBoss->wtf.position = { TitleBossPos };
	titleBoss->Update();
	titleLogo->wtf.position = { TitleLogoPos };
	titleLogo->Update();
	titlePlayer->wtf.position = { TitlePlayerPos };
	playerRotNum = TitleRotReset;
	titlePlayer->wtf.rotation.y = ( PI / 180 ) * playerRotNum;
	titlePlayer->Update();
	if ( posSpeed < 0 )
	{
		posSpeed = -posSpeed;
	}
	bossGoFlag = false;
}

void Titles::PlayerLimit()
{
	if ( titlePlayer->wtf.position.x > 1.5f )
	{
		if ( playerWallFlagX == false )
		{
			playerWallFlagY = false;
			playerRot.y = ( PI / 180 ) * playerRotNum;
			titlePlayer->wtf.rotation = playerRot;
			if ( playerRotNum >= 20 )
			{
				playerRotPlus = -playerRotPlus;

			}
			if ( playerRotNum <= -20 - playerRotPlus )
			{
				playerSpeed = -playerSpeed;
				titlePlayer->wtf.position.x += playerSpeed;
				playerWallFlagX = true;
			}
			playerRotNum += playerRotPlus;
		}
	}
	if ( titlePlayer->wtf.position.x < -1.5f )
	{
		if ( playerWallFlagY == false )
		{
			playerRot.y = ( PI / 180 ) * playerRotNum;
			titlePlayer->wtf.rotation = playerRot;
			playerWallFlagX = false;
			if ( playerRotNum <= -20 )
			{
				playerRotPlus = -playerRotPlus;
			}
			if ( playerRotNum >= 20 - playerRotPlus )
			{
				playerSpeed = -playerSpeed;
				titlePlayer->wtf.position.x += playerSpeed;
				playerWallFlagY = true;
			}
			playerRotNum += playerRotPlus;
		}
	}
	titlePlayer->wtf.position.x += playerSpeed;
	titlePlayer->Update();
}

void Titles::GameStertTrans()
{
	if ( bossGoFlag )
	{
		titleBoss->wtf.position.z += bossNear;
		titlePlayer->wtf.position.z += bossNear;
	}
}

