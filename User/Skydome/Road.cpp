#include "Road.h"

void Road::Initialize()
{
	ModelManager::GetInstance()->LoadModel("worldRoad");
	ModelManager::GetInstance()->LoadModel("worldBillding");
	roadModel = ModelManager::GetInstance()->FindObjModel("worldRoad");
	BillModel = ModelManager::GetInstance()->FindObjModel("worldBillding");

	for ( int i = 0; i < roadCount; i++ )
	{
		road_[ i ] = new Object3d();
		road_[ i ]->Initialize();
		road_[ i ]->SetModel(roadModel);
		road_[ i ]->wtf.position = { pos.x ,pos.y,pos.z + ( posTrans * i ) };
		road_[ i ]->wtf.scale = { RoasdSize };
		road_[ i ]->ShadowUse();
		Bill_[ i ] = new Object3d();
		Bill_[ i ]->Initialize();
		Bill_[ i ]->SetModel(BillModel);
		Bill_[ i ]->wtf.position = { pos.x ,pos.y,pos.z + ( posTrans * i ) };
		Bill_[ i ]->wtf.scale = { RoasdSize };



	}

}

void Road::BeforeUpdate()
{
	for ( int i = 0; i < roadCount; i++ )
	{
		road_[ i ]->wtf.position.z -= beforSpeed;
		Bill_[ i ]->wtf.position.z -= beforSpeed;
		if ( road_[ i ]->wtf.position.z < posLimit )
		{
			road_[ i ]->wtf.position.z = posMove;
		}
		if ( Bill_[ i ]->wtf.position.z < posLimit )
		{
			Bill_[ i ]->wtf.position.z = posMove;
		}
		Bill_[ i ]->Update();
		road_[ i ]->Update();
	}
}

void Road::Update()
{
	for ( int i = 0; i < roadCount; i++ )
	{
		road_[ i ]->wtf.position.z -= speed;
		Bill_[ i ]->wtf.position.z -= speed;
		if ( road_[ i ]->wtf.position.z < posLimit )
		{
			road_[ i ]->wtf.position.z = posMove;
		}
		if ( Bill_[ i ]->wtf.position.z < posLimit )
		{
			Bill_[ i ]->wtf.position.z = posMove;
		}
		Bill_[ i ]->Update();
		road_[ i ]->Update();
	}
}

void Road::roadUpdate()
{
	for ( int i = 0; i < roadCount; i++ )
	{
		Bill_[ i ]->Update();
		road_[ i ]->Update();
	}
}

void Road::Draw()
{
	for ( int i = 0; i < roadCount; i++ )
	{
		road_[ i ]->Draw();
		Bill_[ i ]->Draw();
	}
}

void Road::Reset()
{
	for ( int i = 0; i < roadCount; i++ )
	{
		road_[ i ]->wtf.position = { pos.x ,pos.y,pos.z + ( posTrans * i ) };
		road_[ i ]->wtf.scale = { RoasdSize };
		road_[ i ]->Update();
		Bill_[ i ]->wtf.position = { pos.x ,pos.y,pos.z + ( posTrans * i ) };
		Bill_[ i ]->wtf.scale = { RoasdSize };
		Bill_[ i ]->Update();
	}
}
