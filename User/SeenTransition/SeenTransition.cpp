#include "SeenTransition.h"

SeenTransition::SeenTransition()
{
}

SeenTransition::~SeenTransition()
{

}
void SeenTransition::Initialize(SpriteCommon* spriteCommon_)
{
	assert(spriteCommon_);
	spriteCommon = spriteCommon_;

	winApp_ = winApp_->GetInstance();
	//黒いブラインド
	blind = new Sprite();
	blind->Initialize(spriteCommon,4);
	blind->SetPozition({ winApp_->window_width/2,winApp_->window_height/2 });
	blind->SetSize({ winApp_->window_width,winApp_->window_height });
	blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });

	//サカバンバスピスの上のやつ
	randBlindOverNmb = rand() % 3 + 7;
	randBlindUnderNmb = rand() % 3 + 10;
	blindFaceOver = new Sprite();
	blindFaceOver->Initialize(spriteCommon,randBlindOverNmb);
	blindFaceOver->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 - blindResetPos });
	blindFaceOver->SetSize({ winApp_->window_width,winApp_->window_height });

	blindFaceUnder = new Sprite();
	blindFaceUnder->Initialize(spriteCommon,randBlindUnderNmb);
	blindFaceUnder->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 + blindResetPos });
	blindFaceUnder->SetSize({ winApp_->window_width,winApp_->window_height });


}
void SeenTransition::OnSeenTrans()
{
	seenTransFlag = true;
}

void SeenTransition::Reset()
{

}

void SeenTransition::ResetBlindSprite()
{
	randBlindOverNmb = rand() % 3 + 7;
	randBlindUnderNmb = rand() % 3 + 10;
	blindFaceOver->SetTextureIndex(randBlindOverNmb);
	blindFaceUnder->SetTextureIndex(randBlindUnderNmb);
}



void SeenTransition::Update()
{
	if ( seenTransFlag )
	{
		seenNotEndFlag = true;
		blindTime--;
		if ( blindW < blindResetPos )
		{
			blindW += ( blindResetPos / blindTimeReset );
		}
		//blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });
		//blind->Update();
		blindFaceOver->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 +blindW - blindResetPos });
		blindFaceUnder->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 -blindW + blindResetPos });
		if ( blindW >= blindResetPos )
		{
			blindLoadTime--;
		}
		if ( blindLoadTime <= 0 )
		{
			seenTimeFlag = true;
			seenTransFlag = false;
			blindFlag = true;
		}
	}


	if ( blindFlag )
	{

		blindTime++;
		if ( blindTime >= 0 )
		{
			if ( blindW > 0 )
			{
				blindW -= ( blindResetPos / blindTimeReset );
			}
		}
		//blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });
		//blind->Update();
		blindFaceOver->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 +blindW - blindResetPos });
		blindFaceUnder->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 -blindW + blindResetPos });
		if ( blindW <= 0 )
		{
			blindLoadTime++;
		}
		if ( blindLoadTime > 10 )
		{
			blindFlag = false;
			seenTimeFlag = false;
			seenNotEndFlag = false;
			ResetBlindSprite();
		}
	}

}

void SeenTransition::Draw()
{
	blind->Draw();
	blindFaceUnder->Draw();
	blindFaceOver->Draw();
	
}
