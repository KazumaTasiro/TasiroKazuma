#pragma once
#include "ParticleManager.h"
#include "Vector3.h"
#include "ImGuiManager.h"
#include <memory>
#include <list>
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include "Object3d.h"
#include "ModelManager.h"



class ParticleLibrary
{
public:

	//シングルトン
	static ParticleLibrary* GetInstance();

	void StaticInitialize();

	void Initialize();

	void ObjectInitialize();

	void ObjectUpdate();

	void ObjectDraw();

	//パーティクルマネージャーのDraw
	void PDraw();

	void Update();

	void DrawImgui();

	void LoadCSVfile(const std::string& fileNames);

	void UpdateCSVfile(ParticleManager* particlemana);

	void ResetCSVfile();

	void CreateSaveFile(const std::string& fileNames);

	void LoadTexJudgment(const std::string& fileNames);

	void LoadCSVJudgment(const std::string& fileNames);

	float RandNmber(float randNmb);

	//パーティクルの情報を保存する
	void ParticleDataSave(uint32_t nmb,const std::string& fileNames);

	//パーティクルの生成
	void AddParticle(uint32_t nmb ,Vector3 ObjectPos);

	//保存したパーティクルの更新
	void ParticleUpdate(uint32_t nmb);

	void ParticleDraw(uint32_t nmb);

	void Finalize();
private:
	ParticleLibrary() = default;
	~ParticleLibrary();
	ParticleLibrary(const ParticleLibrary&) = delete;
	const ParticleLibrary& operator=(const ParticleLibrary&) = delete;

	static ParticleLibrary* particleLibrary;

public:


	struct ParticleData
	{
	//パーティクルの基本情報
	//位置
		Vector3 particlePos = { 0,0,0 };
		Vector3 randomParticlePos{ 0,0,0 };
		//終点
		Vector3 particleEndPos = { 0,0,0 };
		Vector3 endPointPos = { 0,0,0 };
		//大きさ
		//最初
		float particleStertScale = 1;
		float particleRandomStertScale = 1;
		//最後
		float particleEndScale = 1;
		float particleRandomEndScale = 1;

		//速度
		Vector3 particleSpeed = { 0,0,0 };
		float particleEndPointSpeed = 1.0f;
		Vector3 particleRandomSpeed = { 0,0,0 };
		float particleRandomSpeedX = 0;
		float particleRandomSpeedY = 0;
		float particleRandomSpeedZ = 0;

		//色
		Vector4 particleStertColor = { 1,1,1,1 };
		Vector4 particleEndColor = { 1,1,1,1 };

		//数
		int particleNmber = 0;
		//パーティクルのライフ
		int particleLife = 10;
		//イージングのナンバー
		int easingNmb = 0;

		char texFileName[ 30 ] = { "standard" };

		//パーティクルのスイッチ
		bool randomParticleStertColor = false;
		bool randomParticleEndColor = false;
		bool randomParticleSize = false;
		bool endPoint = false;
		//ランダムpos

		bool randomParticlePosX;
		bool randomParticlePosY;
		bool randomParticlePosZ;
		//ランダムspeed
		bool randomParticleSpeedX;
		bool randomParticleSpeedY;
		bool randomParticleSpeedZ;
		//ランダムscale
		bool randomParticleStertScale = false;
		bool randomParticleEndScale = false;
	};
	struct ParticleBox
	{
		ParticleManager* particleUse = nullptr;
		ParticleData pData;
	};

private:
	float PI = 3.141592f;

	//パーティクルの基本情報
	//位置
	Vector3 particlePos = { 0,0,0 };
	Vector3 randomParticlePos{ 0,0,0 };

	//終点
	Vector3 particleEndPos = { 0,0,0 };
	Vector3 endPointPos = { 0,0,0 };
	//大きさ
	//最初
	float particleStertScale = 1;
	float particleRandomStertScale = 1;
	//最後
	float particleEndScale = 1;
	float particleRandomEndScale = 1;

	//速度
	Vector3 particleSpeed = { 0,0,0 };
	float particleEndPointSpeed = 1.0f;
	Vector3 particleRandomSpeed = { 0,0,0 };
	float particleRandomSpeedX = 0;
	float particleRandomSpeedY = 0;
	float particleRandomSpeedZ = 0;

	//色
	Vector4 particleStertColor = { 1,1,1,1 };
	Vector4 particleEndColor = { 1,1,1,1 };

	//数
	int particleNmber = 0;
	//パーティクルのライフ
	int particleLife = 10;
	//イージングのナンバー
	int easingNmb = 0;

	char fileName[ 30 ] = { "" };
	char texFileName[ 30 ] = { "standard" };

	char objectFileName[ 30 ] = { "" };
	char defaltObjectFileName[ 30 ] = { "trakku" };
	bool loadObject = false;

	//パーティクルのスイッチ
	bool randomParticleStertColor = false;
	bool randomParticleEndColor = false;
	bool randomParticleSize = false;
	bool loadParticle = false;
	bool saveParticle = false;
	bool endPoint = false;
	//ランダムpos

	bool randomParticlePosX;
	bool randomParticlePosY;
	bool randomParticlePosZ;
	//ランダムspeed
	bool randomParticleSpeedX;
	bool randomParticleSpeedY;
	bool randomParticleSpeedZ;
	//ランダムscale
	bool randomParticleStertScale = false;
	bool randomParticleEndScale = false;

	//画像のロードスイッチ
	bool loadTexSwich = false;

	ParticleManager* particle_;

public:
	

	static const int particleBoxSaves = 30;
	ParticleBox particleBox[ particleBoxSaves ];
	//パーティクル情報取得コマンド
	std::stringstream particleCSV;
private:

	Object3d* player;
	Model* playerModel;

	float playerRot = 0;
	float playerRotLimit = 10.0f;
	float rot = ( PI / 180 );

};