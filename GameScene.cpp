#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	audio = new Audio();
	audio->Initialize();



	//SoundData soundData1 = SoundLoadWave("Resources/se_amd06.wav");
	audio->LoadWave("se_amd06.wav");
	audio->PlayWave("se_amd06.wav");
	//SoundPlayWave(xAudio2.Get(), soundData1);
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
}
