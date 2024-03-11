#include "StageEditor.h"
StageEditor* StageEditor::stageEditor = nullptr;

void StageEditor::Initialize()
{
	ModelManager::GetInstance()->LoadModel("Sakaban");
	enemyModel = ModelManager::GetInstance()->FindObjModel("Sakaban");

}

void StageEditor::Update()
{
	for ( int i = 0; i < enemys.size(); i++ )
	{
		enemys[ i ].enemyObject->Update();
	}
}

void StageEditor::Draw()
{
	for ( int i = 0; i < enemys.size(); i++ )
	{
		enemys[ i ].enemyObject->Draw();
	}
}

void StageEditor::DrawImgui()
{
	ImGui::InputText("CSVFileName",fileName,sizeof(fileName));
	plusEnemy = ImGui::Button("Enemy");
	if ( plusEnemy )
	{
		EnemyBox enemy;
		enemy.enemyObject = Object3d::Create();
		enemy.enemyObject->Initialize();
		enemy.enemyObject->SetModel(enemyModel);
		enemy.enemyObject->Update();
		enemy.enemyNmb = enemyNmbs;
		enemys.push_back(enemy);
		enemyNmbs++;
	}
	
	saveEnemyCSV = ImGui::Button("saveCSV");
	if ( saveEnemyCSV )
	{
		CreateSaveFile(fileName);

	}
	for ( int i = 0; i < enemys.size(); i++ )
	{
		std::string wait = { "EnemyWaitTimer" };
		wait.append(to_string(i));
		std::string pos = { "EnemyPos" };
		pos.append(to_string(i));
		std::string scale = { "EnemyScale" };
		scale.append(to_string(i));

		ImGui::SliderInt(wait.c_str(),&enemys[ i ].waitTimer,0,200);
		ImGui::SliderFloat3(pos.c_str(),&enemys[ i ].enemyObject->wtf.position.x,-100,100);
		ImGui::SliderFloat3(scale.c_str(),&enemys[ i ].enemyObject->wtf.scale.x,1,50);

	}

}

void StageEditor::LoadCSVfile(const std::string& fileNames)
{
	//ファイルを開く
	std::string kDefaultTextureDirectoryPath = "Resources/EnemyCSVFile/";
	std::string CSVpath = ".csv";
	std::ifstream file;
	std::string fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	file.open(fullPath);
	assert(file.is_open());

	//ファイルを内容を文字列ストリームにコピー
	enemyCSV << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void StageEditor::UpdateCSVfile()
{
	Clear();
	std::string line;
	//コマンド実行ループ
	while ( getline(enemyCSV,line) )
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream,word,',');

		//"//"から始まる行はコメント
		if ( word.find("//") == 0 )
		{
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if ( word.find("POP") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());
			//y座標
			getline(line_stream,word,',');
			float y = ( float ) std::atof(word.c_str());
			//z座標
			getline(line_stream,word,',');
			float z = ( float ) std::atof(word.c_str());
			//敵を発生させる
			CSVEnemys(Vector3(x,y,z));
		}

		//WAITコマンド
		else if ( word.find("WAIT") == 0 )
		{
			getline(line_stream,word,',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			enemys[ waitNmb ].waitTimer = waitTime;
			waitNmb++;
			//コマンドループを抜ける
			break;
		}
		else if ( word.find("WAIT") == 0 )
		{
			getline(line_stream,word,',');

//待ち時間
			int32_t enemyNmb = atoi(word.c_str());

			//待機開始
			enemys[ waitNmb ].enemyNmb = enemyNmb;
			break;
		}

	}
}

void StageEditor::ResetCSVfile()
{
	enemyCSV.str("");
	enemyCSV.clear(std::stringstream::goodbit);
	//LoadEnemyPopData();
}

void StageEditor::CreateSaveFile(const std::string& fileNames)
{
	std::string kDefaultTextureDirectoryPath = "Resources/EnemyCSVFile/";
	std::string CSVpath = ".csv";
	std::string fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	std::ofstream ofs(fullPath);

	ofs << "ENEMYS" << "," << enemyNmbs << "," << "," << std::endl;
	for ( int i = 0; i < enemys.size(); i++ )
	{
		ofs << "WAIT" << "," << enemys[ i ].waitTimer << "," << "," << std::endl;
		ofs << "POP" << "," << enemys[ i ].enemyObject->wtf.position.x << "," << enemys[ i ].enemyObject->wtf.position.y << "," << enemys[ i ].enemyObject->wtf.position.z << std::endl;
		ofs << "SCALE" << "," << enemys[ i ].enemyObject->wtf.scale.x << "," << enemys[ i ].enemyObject->wtf.scale.y << "," << enemys[ i ].enemyObject->wtf.scale.z << std::endl;
	}

}


void StageEditor::LoadCSVJudgment(const std::string& fileNames)
{
	std::string kDefaultTextureDirectoryPath = "Resources/EnemyCSVFile/";
	std::string CSVpath = ".csv";
	std::string fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	bool isExists = std::filesystem::exists(fullPath);
	if ( isExists )
	{
		LoadCSVfile(fileNames);
		UpdateCSVfile();
	}
}

void StageEditor::CSVEnemys(Vector3 enemyPos)
{
	EnemyBox enemy;
	enemy.enemyObject = Object3d::Create();
	enemy.enemyObject->Initialize();
	enemy.enemyObject->SetModel(enemyModel);
	enemy.enemyObject->wtf.position = enemyPos;
	enemy.enemyObject->Update();
	enemy.enemyNmb = enemyNmbs;
	enemys.push_back(enemy);
	enemyNmbs++;
}

void StageEditor::Clear()
{
	enemys.clear();
	enemyNmbs = 0;
	waitNmb = 0;
}

StageEditor::~StageEditor()
{

}

StageEditor* StageEditor::GetInstance()
{
	if ( stageEditor == nullptr )
	{
		stageEditor = new StageEditor();
	}
	return stageEditor;
}

void StageEditor::StaticInitialize()
{

}