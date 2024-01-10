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

	//黒いブラインド
	blind = new Sprite();
	blind->Initialize(spriteCommon,Nmb::four);
	blind->SetPozition({ WinApp::window_width/Nmb::two,WinApp::window_height/ Nmb::two });
	blind->SetSize({ WinApp::window_width,WinApp::window_height });
	blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });

	//サカバンバスピスの上のやつ
	blindFaceOver = new Sprite();
	blindFaceOver->Initialize(spriteCommon,randBlindOverNmb);
	blindFaceOver->SetPozition({ WinApp::window_width / Nmb::two,WinApp::window_height / Nmb::two - blindResetPos });
	blindFaceOver->SetSize({ WinApp::window_width,WinApp::window_height });

	blindFaceUnder = new Sprite();
	blindFaceUnder->Initialize(spriteCommon,randBlindUnderNmb);
	blindFaceUnder->SetPozition({ WinApp::window_width / Nmb::two,WinApp::window_height / Nmb::two + blindResetPos });
	blindFaceUnder->SetSize({ WinApp::window_width,WinApp::window_height });
	ResetBlindSprite();


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
	randBlindOverNmb = rand() % Nmb::three + Nmb::seven;
	randBlindUnderNmb = rand() % Nmb::three + Nmb::ten;
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
		blindFaceOver->SetPozition({ WinApp::window_width / Nmb::two,WinApp::window_height / Nmb::two +blindW - blindResetPos });
		blindFaceUnder->SetPozition({ WinApp::window_width / Nmb::two,WinApp::window_height / Nmb::two -blindW + blindResetPos });
		if ( blindW >= blindResetPos )
		{
			blindLoadTime--;
		}
		if ( blindLoadTime <= zeroNmb )
		{
			seenTimeFlag = true;
			seenTransFlag = false;
			blindFlag = true;
		}
	}


	if ( blindFlag )
	{

		blindTime++;
		if ( blindTime >= zeroNmb )
		{
			if ( blindW > zeroNmb )
			{
				blindW -= ( blindResetPos / blindTimeReset );
			}
		}
		//blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });
		//blind->Update();
		blindFaceOver->SetPozition({ WinApp::window_width / Nmb::two,WinApp::window_height / Nmb::two +blindW - blindResetPos });
		blindFaceUnder->SetPozition({ WinApp::window_width / Nmb::two,WinApp::window_height / Nmb::two -blindW + blindResetPos });
		if ( blindW <= zeroNmb )
		{
			blindLoadTime++;
		}
		if ( blindLoadTime > tenNmb )
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
