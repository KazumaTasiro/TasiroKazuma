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



class ParticleLibrary
{
public:

	//シングルトン
	static ParticleLibrary* GetInstance();

	void StaticInitialize();

	void Initialize();

	void Draw();

	void Update();

	void DrawImgui();

	void LoadCSVfile(const std::string& fileNames);

	void UpdateCSVfile();

	void ResetCSVfile();

	void CreateSaveFile(const std::string& fileNames);

	void LoadTexJudgment(const std::string& fileNames);

	void LoadCSVJudgment(const std::string& fileNames);

	float RandNmber(float randNmb);

	void ParticleDataSave(uint32_t nmb,const std::string& fileNames);

	void AddParticle(uint32_t nmb);

	void ParticleUpdate(uint32_t nmb);
private:
	ParticleLibrary() = default;
	~ParticleLibrary();
	ParticleLibrary(const ParticleLibrary&) = delete;
	const ParticleLibrary& operator=(const ParticleLibrary&) = delete;

	static ParticleLibrary* particleLibrary;

private:
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

		char texFileName[ 30 ] = { "LockOnParticle" };

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
	char texFileName[ 30 ] = { "LockOnParticle" };

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

	ParticleManager* particle;

public:
	ParticleBox particleBox[ 30 ];

	//パーティクル情報取得コマンド
	std::stringstream particleCSV;
};