#include "GameOver.h"

GameOver::GameOver()
{
}

GameOver::~GameOver()
{
}

void GameOver::Initialize()
{
	playerDeadParticle = new ParticleManager();
	playerDeadParticle->Initialize();
	playerDeadParticle->LoadTexture("smoke.png");
	playerGameOver = Object3d::Create();
	playerGameOverModel = Model::LoadFormOBJ("trakku");
	playerGameOver->Initialize();
	playerGameOver->SetModel(playerGameOverModel);
	playerGameOver->wtf.position = playerResetPos;
	playerGameOver->wtf.rotation = playerRot;

}

void GameOver::Update()
{
	camera_->SetEye(cameraPos_);
	camera_->SetTarget({ playerPos.x,0,playerPos.z });
	playerGameOver->Update();
	PlayerDeadParticle();
	//playerDeadParticle->Draw();
	camera_->Update();
}

void GameOver::Draw()
{
	playerGameOver->Draw();
}

void GameOver::DrawParticle()
{
	playerDeadParticle->Draw();
}

void GameOver::PlayerDeadParticle()
{
	//パーティクル範囲
	for ( int i = 0; i < 20; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = rnd_posS;
		Vector3 pos = playerPos;
		pos.x += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += 2.0f;
		pos.z += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		const float rnd_vel = rnd_velS;
		const float rnd_velY = rnd_velYS;
		Vector3 vel{};
		vel.x = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = ( float ) rand() / RAND_MAX * rnd_velY - rnd_velY / 2.0f;
		if ( vel.y < 0 )
		{
			vel.y = -vel.y;
		}
		vel.z = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = rnd_accS;
		Vector3 acc{};
		acc.x = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		//const float rnd_pos = 1.0f;
		//Vector3 pos = playerGameOver->wtf.position;
		//pos.x += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//pos.y += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//pos.z += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//追加
		playerDeadParticle->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,one);
		playerDeadParticle->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,two);
		playerDeadParticle->Update();
	}
}
