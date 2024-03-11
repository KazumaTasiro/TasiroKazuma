#include "GameClear.h"
#include "GameClearScene.h"

GameClear::GameClear()
{
}

GameClear::~GameClear()
{
}

void GameClear::Initialize()
{

	spriteCommon_ = SpriteCommon::GetInstance();

	bossSter = Object3d::Create();
	bossSterModel = Model::LoadFormOBJ("SakabanFantom");
	bossSter->SetModel(bossSterModel);
	bossSter->wtf.position = bossPos;
	bossSter->wtf.scale = bossScale;
	bossSter->wtf.rotation = bossRot;
	bossSter->wtf.matWorld.m[ 3 ][ 3 ] = wCenge;

	clear_ = new Sprite();
	clear_->Initialize(five);
	clear_->SetPozition({ WinApp::window_width / 2.0f,( WinApp::window_height / 2.0f ) - gameOverUp });
	clear_->SetColor({ clear_->GetColor().x,clear_->GetColor().y,clear_->GetColor().z,wCenge });

}

void GameClear::Draw()
{
	bossSter->Draw();
}

void GameClear::SpriteDraw()
{
	clear_->Draw();
}



void GameClear::Update()
{
	CameraUpdate();
	BossSpriteUpdate();
	camera_->Update();
	bossSter->Update();
}

void GameClear::BossSpriteUpdate()
{
	if ( cameraRotEnd == true )
	{
		if ( wCenge < 1.0f )
		{
			wCenge += wCengePlus;
			clear_->SetColor({ clear_->GetColor().x,clear_->GetColor().y,clear_->GetColor().z,wCenge });
		}
		else if(wCenge>=1.0f)
		{
			clearEnd = true;
		}
		
	}
	clear_->Update();

}

void GameClear::CameraUpdate()
{
	if ( cameraPIPuls < 10 )
	{
		cameraPIPuls += cameraPI;
	}
	else
	{
		cameraRotEnd = true;
	}
	playerRot.y = cameraPIPuls;
	camera_->SetTarget({ playerRot });
}

void GameClear::Reset()
{
	cameraRotEnd = false;
	clearEnd = false;
	cameraPIPuls = zero;
	wCenge = zero;
	clear_->SetColor({ clear_->GetColor().x,clear_->GetColor().y,clear_->GetColor().z,wCenge });
}
