#include "Skydome.h"

#include "Skydome.h"

void Skydome::Initalize()
{
	model_ = Model::LoadFormOBJ("uv");
	worldTransform_ = Object3d::Create();
	worldTransform_->wtf.scale= skydomeScale;
	worldTransform_->wtf.position = skydomePos;
	worldTransform_->SetModel(model_);

	worldTransform_->Update();
}

void Skydome::Update()
{

}

void Skydome::Draw()
{
	worldTransform_->Draw();
}