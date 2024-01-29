#include "EnemyObstacleBullet.h"

EnemyObstacleBullet::EnemyObstacleBullet()
{
}

EnemyObstacleBullet::~EnemyObstacleBullet()
{
}

void EnemyObstacleBullet::Initialize(Model* model,Model* alertRoadModel,const Vector3& position)
{
	assert(model);

	model_ = model;

	AlertRoadModel_ = alertRoadModel;


	//ワールドトランスフォームの初期化
	worldTransform_ = Object3d::Create();
	worldTransformAlertRoad_ = Object3d::Create();

	//引数で受け取った初期座標をセット
	worldTransform_->wtf.position = ( position );
	worldTransformAlertRoad_->wtf.position = worldTransform_->wtf.position;

	//モデルをセットする
	worldTransform_->SetModel(model_);

	worldTransform_->wtf.scale = worldSc;
}

void EnemyObstacleBullet::Update()
{
	GravityUpdate();
	RoadUpdate();
	worldTransform_->Update();
	worldTransformAlertRoad_->wtf.position = worldTransform_->wtf.position;
	worldTransformAlertRoad_->Update();
}

void EnemyObstacleBullet::Draw()
{
	worldTransform_->Draw();
	if ( hitRoad )
	{
		worldTransformAlertRoad_->Draw();
	}
}

void EnemyObstacleBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyObstacleBullet::GetWorldPosition()
{
	return worldTransform_->wtf.position;
}

void EnemyObstacleBullet::GravityUpdate()
{
	if ( hitRoad == false )
	{
		speedGra += gravity;
		worldTransform_->wtf.position.y -= speedGra;
		if ( worldTransform_->wtf.position.y < roadPos )
		{
			hitRoad = true;
			worldTransform_->wtf.position.y = roadPos;
		}
	}
}

void EnemyObstacleBullet::RoadUpdate()
{
	if ( hitRoad )
	{
		worldTransform_->wtf.position.z -= speed;
		if ( worldTransform_->wtf.position.z <= -15 )
		{
			isDead_ = true;
		}
	}
}
