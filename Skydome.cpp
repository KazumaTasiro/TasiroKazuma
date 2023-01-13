#include "Skydome.h"

void Skydome::Initalize()
{
	model_ = Model::LoadFormOBJ("uv");
	//���[���h�ϊ��̏�����
	worldTransform_ = Object3d::Create();
	worldTransform_->SetScale({ 500,500,500 });
	worldTransform_->position = { 0,0,0 };
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
