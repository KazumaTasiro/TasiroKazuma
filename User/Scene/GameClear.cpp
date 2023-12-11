#include "GameClear.h"

GameClearScene::GameClearScene()
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize(SpriteCommon* spriteCommon)
{
	assert(spriteCommon);

	spriteCommon_ = spriteCommon;

	bossSter = Object3d::Create();
	bossSterModel = Model::LoadFormOBJ("SakabanFantom");
	bossSter->SetModel(bossSterModel);
	bossSter->wtf.position = bossPos;
	bossSter->wtf.scale = bossScale;
	bossSter->wtf.rotation = bossRot;
	bossSter->wtf.matWorld.m[ 3 ][ 3 ] = wCenge;

	clear_ = new Sprite();
	clear_->Initialize(spriteCommon_,five);
	clear_->SetPozition({ WinApp::window_width / 2,( WinApp::window_height / 2 ) - gameOverUp });
	clear_->SetColor({ clear_->GetColor().x,clear_->GetColor().y,clear_->GetColor().z,wCenge });

}

void GameClearScene::Draw()
{
	bossSter->Draw();
}

void GameClearScene::SpriteDraw()
{
	clear_->Draw();
}

void GameClearScene::Update()
{
	CameraUpdate();
	BossSpriteUpdate();
	camera_->Update();
	bossSter->Update();
}

void GameClearScene::BossSpriteUpdate()
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

void GameClearScene::CameraUpdate()
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

void GameClearScene::Reset()
{
	cameraRotEnd = false;
	clearEnd = false;
	cameraPIPuls = zero;
	wCenge = zero;
	clear_->SetColor({ clear_->GetColor().x,clear_->GetColor().y,clear_->GetColor().z,wCenge });
}
