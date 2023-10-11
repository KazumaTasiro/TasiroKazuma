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
	blind->Initialize(spriteCommon,4);
	blind->SetPozition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	blind->SetSize({ WinApp::window_width,WinApp::window_height });
	blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });

	////サカバンバスピスの上のやつ
	//randBlindOverNmb = rand() % 3 + 7;
	//randBlindUnderNmb = rand() % 3 + 10;
	//blindFaceOver = new Sprite();
	//blindFaceOver->Initialize(spriteCommon,randBlindOverNmb);
	//blindFaceOver->SetPozition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	//blindFaceOver->SetSize({ WinApp::window_width,WinApp::window_height });

	//blindFaceUnder = new Sprite();
	//blindFaceUnder->Initialize(spriteCommon,randBlindUnderNmb);
	//blindFaceUnder->SetPozition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	//blindFaceUnder->SetSize({ WinApp::window_width,WinApp::window_height });


}
void SeenTransition::OnSeenTrans()
{
	seenTransFlag = true;
}

void SeenTransition::Reset()
{
}



void SeenTransition::Update()
{
	if ( seenTransFlag )
	{
		blindTime--;
		if ( blindW < 1 )
		{
			blindW += ( 1.0f / blindTimeReset );
		}
		blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });
		blind->Update();
		if ( blindTime <= -10 )
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
				blindW -= ( 1.0f / blindTimeReset );
			}
		}
		blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });
		blind->Update();
		if ( blindTime >= blindTimeReset )
		{
			blindFlag = false;
		}
	}

}

void SeenTransition::Draw()
{
	blind->Draw();
}
