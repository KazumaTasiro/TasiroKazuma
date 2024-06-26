#pragma once
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
#include <vector>
#include <memory>

class StageEditor
{
public:

	//シングルトン
	static StageEditor* GetInstance();

	void StaticInitialize();

	void Initialize();

	void Update();

	void Draw();

	void DrawImgui();

	void LoadCSVfile(const std::string& fileNames);

	void UpdateCSVfile();

	void ResetCSVfile();

	void CreateSaveFile(const std::string& fileNames,bool filePathTrue);


	void LoadCSVJudgment(const std::string& fileNames);

	void CSVEnemys(Vector3 enemyPos);

	void Clear();

	void TxtUpdate();

	void TxtRoad(std::string folderPath,std::vector<std::string>& file_names);
private:
	StageEditor() = default;
	~StageEditor();
	StageEditor(const StageEditor&) = delete;
	const StageEditor& operator=(const StageEditor&) = delete;

	static StageEditor* stageEditor;
private:

	float PI = 3.141592f;

	struct EnemyBox
	{
		Object3d* enemyObject;
		int  waitTimer = 0;
		int  enemyNmb = 0;
	};
private:
	char fileName[ 30 ] = { "" };
	//敵の最大数
	Model* enemyModel;

	std::stringstream enemyCSV;

	std::vector<EnemyBox> enemys;

	uint32_t enemyNmbs = 0;

	bool plusEnemy = false;
	bool saveEnemyCSV = false;

	uint32_t waitNmb;

	float scaleNmb = 10.0f;

	float enemyScale = 1;

	float enemyRot = 180.0f;
	float rot = ( PI / 180 );
};