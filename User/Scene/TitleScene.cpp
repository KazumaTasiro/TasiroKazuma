#include "TitleScene.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	camera_ = std::make_unique<Camera>(WinApp::window_width,WinApp::window_height);
	camera_->SetEye(cameraTitle);
	camera_->Update();

	lightGroupNon = LightGroup::Create();

	Object3d::SetLightNon(lightGroupNon);
	Object3d::SetCamera(camera_.get());
	ParticleManager::SetCamera(camera_.get());
	skydome_ = std::make_unique <Skydome>();
	skydome_->Initalize();

	road_ = std::make_unique <Road>();
	road_->Initialize();

	stert_ = std::make_unique <Sprite>();
	stert_->Initialize(3);
	stert_->SetPozition({ WinApp::GetInstance()->window_width / 2.0f, WinApp::GetInstance()->window_height / 2.0f + gamestertUp });

	title_ = std::make_unique< Titles>();
	title_->Initialize();



	ImGuiMan_ = ImGuiManager::GetInstance();

}

void TitleScene::Update()
{
#ifdef _DEBUG
	if ( Input::GetInstance()->PushKey(DIK_P) )
	{
		GameSceneState* state = new StageEditorScene();
		SceneManager::GetInstance()->SetNextScene(state);
	}
#endif
	ImGuiMan_->Bigin();
#ifdef _PARTICLELIBRARY

	ImGui::SliderFloat3("cameraEye",&cameraImgui.x,-30,30);

	camera_->SetEye(cameraImgui);
	Vector3 CameraMove = { 0,0,1.0f };
	Vector3 cameraTage = cameraImgui + CameraMove;
	camera_->SetTarget(cameraTage);
	camera_->Update();
	ParticleLibrary::GetInstance()->ObjectUpdate();
	ParticleLibrary::GetInstance()->DrawImgui();
	ParticleLibrary::GetInstance()->Update();

#endif
	ImGuiMan_->End();

#ifndef _PARTICLELIBRARY

	skydome_->Update();
	road_->BeforeUpdate();
	title_->Update();
	SeenTransition::GetInstance()->Update();
	//player_->Update();
	if ( SeenTransition::GetInstance()->ReturnSeenNotEnd() == false )
	{
		if ( Input::GetInstance()->TriggerMouse(0) )
		{
			SeenTransition::GetInstance()->OnSeenTrans();
			seenFlag = true;
		}
	}
	if ( seenFlag )
	{
		if ( SeenTransition::GetInstance()->ReturnSeenTrans() )
		{

			
			GameSceneState* state = new GamePlayScene();
			SceneManager::GetInstance()->SetNextScene(state);
			seenFlag = false;
		}
	}
#endif
}

void TitleScene::ObjectDraw()
{
#ifdef _PARTICLELIBRARY
//skydome_->Draw();
//road_->Draw();
	ParticleLibrary::GetInstance()->ObjectDraw();
	ParticleLibrary::GetInstance()->PDraw();
#endif // _PARTICLELIBRARY
#ifndef _PARTICLELIBRARY
	skydome_->Draw();
	road_->Draw();
	/*player_->Draw();*/
	title_->Draw();
#endif // _PARTICLELIBRARY
}

void TitleScene::SpriteDraw()
{
	ImGuiMan_->Draw();
#ifndef _PARTICLELIBRARY
	stert_->Draw();
	SeenTransition::GetInstance()->Draw();
#endif // _PARTICLELIBRARY
}

void TitleScene::Finalize()
{
}
