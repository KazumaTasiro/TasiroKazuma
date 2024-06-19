#include "StageEditor.h"
#include "StageEditorScene.h"
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
	UpdateCSVfile();
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
	if ( ImGui::Button("DeleteEnemy") )
	{
		Clear();
	}
	ImGui::Separator();
	ImGui::InputText("CSVFileName",fileName,sizeof(fileName));
	saveEnemyCSV = ImGui::Button("saveCSV");
	LoadEnemyCSV = ImGui::Button("LoadCSV");
	ImGui::Separator();
	plusEnemy = ImGui::Button("Enemy");
	ImGui::Separator();
	if ( plusEnemy )
	{
		EnemyBox enemy;
		enemy.enemyObject = Object3d::Create();
		enemy.enemyObject->Initialize();
		enemy.enemyObject->SetModel(enemyModel);
		enemy.enemyObject->wtf.scale = { scaleNmb,scaleNmb ,scaleNmb };
		enemy.enemyObject->Update();
		enemy.enemyNmb = enemyNmbs;
		enemys.push_back(enemy);
		enemyNmbs++;
	}


	if ( saveEnemyCSV )
	{
		CreateSaveFile(fileName,false);

	}
	if ( LoadEnemyCSV )
	{
		ResetCSVfile();
		LoadCSVJudgment(fileName);

	}
	ImGui::SliderFloat("EnemysRot",&enemyRot,0,360);

	for ( int i = 0; i < enemys.size(); i++ )
	{
		std::string wait = { "EnemyWaitTimer" };
		wait.append(to_string(i));
		std::string pos = { "EnemyPos" };
		pos.append(to_string(i));

		ImGui::SliderInt(wait.c_str(),&enemys[ i ].waitTimer,0,200);
		ImGui::SliderFloat3(pos.c_str(),&enemys[ i ].enemyObject->wtf.position.x,-100,120);
		enemys[ i ].enemyObject->wtf.rotation.y = enemyRot * rot;
		std::string deleteButton= { "delete" };
		deleteButton.append(to_string(i));

		if ( ImGui::Button(deleteButton.c_str()) )
		{
			enemys.erase(enemys.begin() + i);;
		}
		ImGui::Separator();
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
			waitBox[ waitNmb ] = waitTime;
			waitNmb++;
			//コマンドループを抜ける
			break;
		}
		else if ( word.find("ENEMYS") == 0 )
		{
			getline(line_stream,word,',');

//待ち時間
			int32_t enemyNmb = atoi(word.c_str());

			//待機開始
			enemyNmbs = enemyNmb;
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

void StageEditor::CreateSaveFile(const std::string& fileNames,bool filePathTrue)
{
	std::string kDefaultTextureDirectoryPath = "Resources/EnemyCSVFile/";
	std::string CSVpath = ".csv";
	std::string fullPath;
	if ( filePathTrue == false )
	{
		fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	}
	else
	{
		fullPath = fileNames;

	}
	
	std::ofstream ofs(fullPath);

	ofs << "ENEMYS" << "," << enemyNmbs << "," << "," << std::endl;
	for ( int i = 0; i < enemys.size(); i++ )
	{
		ofs << "WAIT" << "," << enemys[ i ].waitTimer << "," << "," << std::endl;
		ofs << "POP" << "," <<static_cast<int> (enemys[ i ].enemyObject->wtf.position.x) << "," << static_cast< int > ( enemys[ i ].enemyObject->wtf.position.y ) << "," << static_cast< int > ( enemys[ i ].enemyObject->wtf.position.z ) << ","  << std::endl;
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
		Clear();
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
	enemy.enemyObject->wtf.scale = { scaleNmb,scaleNmb ,scaleNmb };
	enemy.enemyObject->wtf.position = enemyPos;
	enemy.enemyObject->Update();
	enemy.enemyNmb = enemyNmbs;
	enemy.waitTimer = waitBox[ waitNmb ];
	enemys.push_back(enemy);
	enemyNmbs++;

}

void StageEditor::Clear()
{
	enemys.clear();
	enemyNmbs = 0;
	waitNmb = 0;
}

void StageEditor::TxtUpdate()
{
	std::vector<std::string> CSVFileNames;
	TxtRoad("Resources/EnemyCSVFile",CSVFileNames);

	for ( int i = 0; i < CSVFileNames.size(); i++ )
	{
		CreateSaveFile(CSVFileNames[ i ],true);
	}
}

void StageEditor::TxtRoad(std::string folderPath,std::vector<std::string>& file_names)
{
	using namespace std::filesystem;
	directory_iterator iter(folderPath),end;
	std::error_code err;

	for ( ; iter != end && !err; iter.increment(err) )
	{
		const directory_entry entry = *iter;

		file_names.push_back(entry.path().string());
		printf("%s\n",file_names.back().c_str());
	}

	/* エラー処理 */
	if ( err )
	{
		std::cout << err.value() << std::endl;
		std::cout << err.message() << std::endl;
	}
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
