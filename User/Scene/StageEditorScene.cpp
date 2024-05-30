#include "StageEditorScene.h"

void StageEditorScene::Initialize()
{
	camera_ = std::make_unique<Camera>(WinApp::window_width,WinApp::window_height);
	camera_->SetEye(cameraGame);
	camera_->Update();

	Object3d::SetCamera(camera_.get());

	skydome_ = std::make_unique <Skydome>();
	skydome_->Initalize();

	road_ = std::make_unique <Road>();
	road_->Initialize();

	railCamera = std::make_unique < RailCamera>();
	railCamera->SetCamera(camera_.get());
	//railCamera->SetPlayer(player_.get());
	railCamera->Initialize();
	railCamera->Update();

	ImGuiMan_ = ImGuiManager::GetInstance();
	road_->Update();
}

void StageEditorScene::Update()
{
	ImGuiMan_->Bigin();

	skydome_->Update();

	camera_->SetEye(cameraGame);
	camera_->Update();
	StageEditor::GetInstance()->Update();
	StageEditor::GetInstance()->DrawImgui();
	railCamera->Update();


	ImGuiMan_->End();
}

void StageEditorScene::ObjectDraw()
{
	skydome_->Draw();
	road_->Draw();
	StageEditor::GetInstance()->Draw();
}

void StageEditorScene::SpriteDraw()
{
	ImGuiMan_->Draw();
}

void StageEditorScene::Finalize()
{
}
