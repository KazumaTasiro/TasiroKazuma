#include "Skydome.h"

#include "Skydome.h"

void Skydome::Initalize()
{
	ModelManager::GetInstance()->LoadModel("uv");
	model_ = ModelManager::GetInstance()->FindObjModel("uv");
	worldTransform_ = Object3d::Create();
	worldTransform_->wtf.scale= skydomeScale;
	worldTransform_->wtf.position = skydomePos;
	worldTransform_->SetModel(model_);

	worldTransform_->Update();
}

void Skydome::Update()
{
	worldTransform_->Update();
}

void Skydome::Draw()
{
	//worldTransform_->wtf.position += skydomePos2;
	//worldTransform_->wtf.scale = skydomeScale;
	//worldTransform_->Update();
	worldTransform_->Draw();
}