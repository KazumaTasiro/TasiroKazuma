#include "Road.h"

void Road::Initialize()
{
	roadModel = Model::LoadFormOBJ("worldRoad");
	BillModel = Model::LoadFormOBJ("worldBillding");

	for (int i = 0; i < roadCount; i++)
	{
		road_[i] = new Object3d();
		road_[i]->Initialize();
		road_[i]->SetModel(roadModel);
		road_[i]->wtf.position = { 0,-2.5f,-50.0f + (50 * i) };
		road_[i]->wtf.scale = { RoasdSize };
		Bill_[i] = new Object3d();
		Bill_[i]->Initialize();
		Bill_[i]->SetModel(BillModel);
		Bill_[i]->wtf.position = { 0,-2.5f,-50.0f + (50 * i) };
		Bill_[i]->wtf.scale = { RoasdSize };

	}

}

void Road::BeforeUpdate()
{
	for (int i = 0; i < roadCount; i++) {
		road_[i]->wtf.position.z -= beforSpeed;
		Bill_[i]->wtf.position.z -= beforSpeed;
		if (road_[i]->wtf.position.z < -150) {
			road_[i]->wtf.position.z = -100.0f + (49.0f * roadCount);
		}
		if (Bill_[i]->wtf.position.z < -150) {
			Bill_[i]->wtf.position.z = -100.0f + (49.0f * roadCount);
		}
		Bill_[i]->Update();
		road_[i]->Update();
	}
}

void Road::Update()
{
	for (int i = 0; i < roadCount; i++) {
		road_[i]->wtf.position.z -= speed;
		Bill_[i]->wtf.position.z -= speed;
		if (road_[i]->wtf.position.z < -150) {
			road_[i]->wtf.position.z = -100.0f+(49.0f * roadCount);
		}
		if (Bill_[i]->wtf.position.z < -150) {
			Bill_[i]->wtf.position.z = -100.0f + (49.0f * roadCount);
		}
		Bill_[i]->Update();
		road_[i]->Update();
	}
}

void Road::Draw()
{
	for (int i = 0; i < roadCount; i++) {
		road_[i]->Draw();
		Bill_[i]->Draw();
	}
}

void Road::Reset()
{
	for (int i = 0; i < roadCount; i++) {
		road_[i]->wtf.position = { 0,-2.5f,-50.0f + (50 * i) };
		road_[i]->wtf.scale = { RoasdSize };
		road_[i]->Update();
		Bill_[i]->wtf.position = { 0,-2.5f,-50.0f + (50 * i) };
		Bill_[i]->wtf.scale = { RoasdSize };
		Bill_[i]->Update();
	}
}
