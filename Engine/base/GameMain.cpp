#include "GameMain.h"

GameMain::GameMain()
{
}

GameMain::~GameMain()
{
}


void GameMain::Initialize()
{

	Framework::Initialize();
	posteffect = new PostEffect();

	posteffect->Initialize(Framework::GetDXCommon()->GetDevice(), Framework::GetInput());

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(Framework::GetDXCommon()->GetDevice());
	ParticleManager::StaticInitialize(Framework::GetDXCommon()->GetDevice(), Framework::GetDXCommon()->GetCommandList());
	ParticleLibrary::GetInstance()->Initialize();
	SpriteCommon::GetInstance()->Initialize(Framework::GetDXCommon());
	SpriteInitialize();
	ParticleInitialize();
	ModelLoad();
	StageEditor::GetInstance()->Initialize();
	SeenTransition::GetInstance()->Initialize();
	GameSceneState* scene = new TitleScene();
	SceneManager::GetInstance()->SetNextScene(scene);
	//DirectionalLight::StaticInitialize(Framework::GetDXCommon()->GetDevice());


	//gameScene = new GameScene();
	//gameScene->Initialize(Framework::GetDXCommon());

}

void GameMain::Finalize()
{
	SceneManager::GetInstance()->Update();
	//gameScene->Finalize();
	FbxLoader::GetInstance()->Finalize();

	
	delete gameScene;
	delete posteffect;
	
	//基礎クラスの終了処理
	Framework::Finalize();
}

void GameMain::Update()
{
	Framework::Update();
	SceneManager::GetInstance()->Update();
	//gameScene->Update();
}

void GameMain::Draw()
{
	posteffect->PreDrawScene(Framework::GetDXCommon()->GetCommandList());

	Object3d::PreDraw(Framework::GetDXCommon()->GetCommandList());
	SceneManager::GetInstance()->ObjectDraw();
	Object3d::PostDraw();
	//gameScene->Draw();

	posteffect->PostDrawScene(Framework::GetDXCommon()->GetCommandList());

	Framework::GetDXCommon()->PreDraw();

	posteffect->Draw(Framework::GetDXCommon()->GetCommandList());

	SceneManager::GetInstance()->SpriteDraw();
	Framework::GetDXCommon()->PostDraw();


}

void GameMain::SpriteInitialize()
{
	SpriteCommon::GetInstance()->LoadTexture(0,"Reticle.png");
	SpriteCommon::GetInstance()->LoadTexture(1,"ReticleLock.png");
	SpriteCommon::GetInstance()->LoadTexture(2,"EnemyLock.png");
	SpriteCommon::GetInstance()->LoadTexture(3,"stert.png");
	SpriteCommon::GetInstance()->LoadTexture(4,"GameBlind.png");
	SpriteCommon::GetInstance()->LoadTexture(5,"GameClear.png");
	SpriteCommon::GetInstance()->LoadTexture(6,"GameOver.png");
	SpriteCommon::GetInstance()->LoadTexture(7,"GameBlindFaceOver1.png");
	SpriteCommon::GetInstance()->LoadTexture(8,"GameBlindFaceOver2.png");
	SpriteCommon::GetInstance()->LoadTexture(9,"GameBlindFaceOver3.png");
	SpriteCommon::GetInstance()->LoadTexture(10,"GameBlindFaceUnder1.png");
	SpriteCommon::GetInstance()->LoadTexture(11,"GameBlindFaceUnder2.png");
	SpriteCommon::GetInstance()->LoadTexture(12,"GameBlindFaceUnder3.png");
	SpriteCommon::GetInstance()->LoadTexture(13,"Time3.png");
	SpriteCommon::GetInstance()->LoadTexture(14,"Time2.png");
	SpriteCommon::GetInstance()->LoadTexture(15,"Time1.png");
	SpriteCommon::GetInstance()->LoadTexture(16,"TimeGo.png");
	SpriteCommon::GetInstance()->LoadTexture(17,"playerHPNone.png");
	SpriteCommon::GetInstance()->LoadTexture(18,"playerHPMax.png");
	SpriteCommon::GetInstance()->LoadTexture(19,"Operation.png");
	SpriteCommon::GetInstance()->LoadTexture(20,"CameraMove.png");
	SpriteCommon::GetInstance()->LoadTexture(21,"CengeCameraE.png");
	SpriteCommon::GetInstance()->LoadTexture(22,"CengeCmaraQ.png");
	SpriteCommon::GetInstance()->LoadTexture(23,"damageEf.png");
	SpriteCommon::GetInstance()->LoadTexture(24,"MoveTuto.png");
}

void GameMain::ParticleInitialize()
{
	ParticleLibrary::GetInstance()->ObjectInitialize();

	ParticleLibrary::GetInstance()->ParticleDataSave(0,"RotParticleL");
	//ParticleLibrary::GetInstance()->ParticleUpdate(0);
	ParticleLibrary::GetInstance()->ParticleDataSave(1,"RotParticleR");
	//ParticleLibrary::GetInstance()->ParticleUpdate(1);
	ParticleLibrary::GetInstance()->ParticleDataSave(2,"ring");
	//ParticleLibrary::GetInstance()->ParticleUpdate(2);
}

void GameMain::ModelLoad()
{
	ModelManager::GetInstance()->LoadModel("Sakaban");
	ModelManager::GetInstance()->LoadModel("SakabanTakkle");
	ModelManager::GetInstance()->LoadModel("SakabanObstacle");
}
