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
	titlePlayer->wtf.rotation.y = rot * playerRotNum;
}


void Titles::Update()
{
	LogoRotNum += LogoRotPlus;
	LogoRot.y = rot * LogoRotNum;
	titleLogo->wtf.rotation = LogoRot;
	if ( LogoRotNum >= LogoRotMax || LogoRotNum <= -LogoRotMax )
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
		if ( gravityStertTime <= gravityEndTime )
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
	titlePlayer->wtf.rotation.y = rot * playerRotNum;
	titlePlayer->Update();
	if ( playerSpeed < 0 )
	{
		playerSpeed = -playerSpeed;
	}
	if ( playerRotPlus < 0 )
	{
		playerRotPlus = -playerRotPlus;
	}
	playerWallFlagY = false;
	playerWallFlagX = false;
	bossGoFlag = false;
}

void Titles::PlayerLimit()
{
	if ( titlePlayer->wtf.position.x > playerMoveRimit )
	{
		if ( playerWallFlagX == false )
		{
			playerWallFlagY = false;
			playerRot.y = rot * playerRotNum;
			titlePlayer->wtf.rotation = playerRot;
			if ( playerRotNum >= playerRotConst )
			{
				playerRotPlus = -playerRotPlus;

			}
			if ( playerRotNum <= -playerRotConst - playerRotPlus )
			{
				playerSpeed = -playerSpeed;
				titlePlayer->wtf.position.x += playerSpeed;
				playerWallFlagX = true;
			}
			playerRotNum += playerRotPlus;
		}
	}
	if ( titlePlayer->wtf.position.x < -playerMoveRimit )
	{
		if ( playerWallFlagY == false )
		{
			playerRot.y = rot * playerRotNum;
			titlePlayer->wtf.rotation = playerRot;
			playerWallFlagX = false;
			if ( playerRotNum <= -playerRotConst )
			{
				playerRotPlus = -playerRotPlus;
			}
			if ( playerRotNum >= playerRotConst - playerRotPlus )
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

