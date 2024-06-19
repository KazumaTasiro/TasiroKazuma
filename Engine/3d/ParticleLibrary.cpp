#include "ParticleLibrary.h"

ParticleLibrary* ParticleLibrary::particleLibrary = nullptr;

ParticleLibrary* ParticleLibrary::GetInstance()
{
	if ( particleLibrary == nullptr )
	{
		particleLibrary = new ParticleLibrary();
	}
	return particleLibrary;
}

void ParticleLibrary::StaticInitialize()
{

}

void ParticleLibrary::Initialize()
{
	particle_ = new ParticleManager();
	particleData = new ParticleData();
	particleClear = new ParticleData();
	particle_->Initialize();
	particle_->LoadTexture("standard.png");


}

void ParticleLibrary::ObjectInitialize()
{
	ModelManager::GetInstance()->LoadModel("trakku");
	playerModel = ModelManager::GetInstance()->FindObjModel("trakku");
	player = Object3d::Create();
	player->Initialize();
	player->SetModel(playerModel);
}

void ParticleLibrary::ObjectUpdate()
{
	player->Update();
}

void ParticleLibrary::ObjectDraw()
{
	if ( objectDrawSwich )
	{
		player->Draw();
	}
}

void ParticleLibrary::PDraw()
{
	particle_->Draw();
}

void ParticleLibrary::Update()
{
	Vector3 particlePoss = particleData->particlePos;

	if ( particleData->randomParticlePosX )
	{

		particlePoss.x += RandNmber(particleData->randomParticlePos.x);
	}
	if ( particleData->randomParticlePosY )
	{

		particlePoss.y += RandNmber(particleData->randomParticlePos.y);
	}
	if ( particleData->randomParticlePosZ )
	{

		particlePoss.z += RandNmber(particleData->randomParticlePos.z);
	}


	if ( particleData->endPoint )
	{
		if ( particleData->randomParticleSpeedX )
		{
			particleData->particleEndPointSpeed = RandNmber(particleData->particleRandomSpeedX);
		}
		particleData->endPointPos = ( particleData->particleEndPos - particlePoss );
		particleData->endPointPos.nomalize();
		particleData->endPointPos *= particleData->particleEndPointSpeed;

	}
	else
	{

		if ( particleData->randomParticleSpeedX )
		{

			particleData->particleSpeed.x = RandNmber(particleData->particleRandomSpeedX);
		}
		if ( particleData->randomParticleSpeedY )
		{

			particleData->particleSpeed.y = RandNmber(particleData->particleRandomSpeedY);
		}
		if ( particleData->randomParticleSpeedZ )
		{

			particleData->particleSpeed.z = RandNmber(particleData->particleRandomSpeedZ);
		}
	}

	if ( particleData->randomParticleStertScale )
	{

		particleData->particleStertScale = RandNmber(particleData->particleRandomStertScale);
		if ( particleData->particleStertScale < 0 )
		{
			particleData->particleStertScale = -particleData->particleStertScale;
		}
	}
	if ( particleData->randomParticleEndScale )
	{

		particleData->particleEndScale = RandNmber(particleData->particleRandomEndScale);
		if ( particleData->particleEndScale < 0 )
		{
			particleData->particleEndScale = -particleData->particleEndScale;
		}
	}

	if ( particleData->randomParticleStertColor )
	{
		Vector3 randomColor = { static_cast< float > ( rand() % 100 ),static_cast< float > ( rand() % 100 ) ,static_cast< float > ( rand() % 100 ) };
		randomColor.x /= 100;
		randomColor.y /= 100;
		randomColor.z /= 100;
		particleData->particleStertColor = { randomColor.x,randomColor.y,randomColor.z,1 };
	}
	if ( particleData->randomParticleEndColor )
	{
		Vector3 randomColor = { static_cast< float > ( rand() % 100 ),static_cast< float > ( rand() % 100 ) ,static_cast< float > ( rand() % 100 ) };
		randomColor.x /= 100;
		randomColor.y /= 100;
		randomColor.z /= 100;
		particleData->particleEndColor = { randomColor.x,randomColor.y,randomColor.z,1 };

	}

	for ( int i = 0; i < particleData->particleNmber; i++ )
	{
		if ( particleData->endPoint )
		{
			particle_->Add(particleData->particleLife,particlePoss,particleData->particleEndPos,particleData->endPointPos,particleData->particleStertScale,particleData->particleEndScale,particleData->easingNmb,particleData->particleStertColor,particleData->particleEndColor);
		}
		else
		{
			particle_->Add(particleData->particleLife,particlePoss,particleData->particleEndPos,particleData->particleSpeed,particleData->particleStertScale,particleData->particleEndScale,particleData->easingNmb,particleData->particleStertColor,particleData->particleEndColor);
		}
	}
	particle_->Update();
}

void ParticleLibrary::DrawImgui()
{
	ImGui::InputText("ObjectFileName",objectFileName,sizeof(objectFileName));

	//OVJをロードする
	loadObject = ImGui::Button("loadObject");
	if ( loadObject )
	{
		playerModel = ModelManager::GetInstance()->FindObjModel(objectFileName);
		player->SetModel(playerModel);
	}
	//Objの表示切替
	ImGui::Checkbox("ObjectDraw",&objectDrawSwich);
	//objの角度
	ImGui::SliderFloat("playerRot",&playerRot,-playerRotLimit,playerRotLimit);
	player->wtf.rotation.y = playerRot * rot;
	player->Update();

	ImGui::InputText("CSVFileName",fileName,sizeof(fileName));
	loadParticle = ImGui::Button("loadParticle");

	saveParticle = ImGui::Button("saveParticle");
	if ( ImGui::Button("ResetParticle") )
	{
		ResetCSVfile();
		ResetParticleData();
		ResetCSVfile();
	}
	if ( saveParticle )
	{
		CreateSaveFile(fileName,false);

	}
	if ( loadParticle )
	{
		ResetCSVfile();
		LoadCSVJudgment(fileName,false);
		ResetCSVfile();

	}

	ImGui::InputText("TextureFileName",particleData->texFileName,sizeof(particleData->texFileName));
	loadTexSwich = ImGui::Button("loadTexture");
	if ( loadTexSwich )
	{
		LoadTexJudgment(particleData->texFileName);
	}
	ImGui::SliderInt("particleNmber",&particleData->particleNmber,0,70);
	ImGui::SliderFloat3("particlePos",&particleData->particlePos.x,-5.0f,5.0f);
	ImGui::Checkbox("randomParticlePosX",&particleData->randomParticlePosX);
	ImGui::Checkbox("randomParticlePosY",&particleData->randomParticlePosY);
	ImGui::Checkbox("randomParticlePosZ",&particleData->randomParticlePosZ);
	ImGui::SliderFloat3("particlerandomPosX",&particleData->randomParticlePos.x,-5.0f,5.0f);

	//エンドポイント関係
	ImGui::Checkbox("endPoint",&particleData->endPoint);
	ImGui::SliderFloat("particleEndPointSpeed",&particleData->particleEndPointSpeed,0,3.0f);
	ImGui::Checkbox("randomParticleSpeed",&particleData->randomParticleSpeedX);
	ImGui::SliderFloat("particleRandomSpeed",&particleData->particleRandomSpeedX,-3.0f,3.0f);
	ImGui::SliderFloat3("particleEndPos",&particleData->particleEndPos.x,-5.0f,5.0f);
	ImGui::SliderFloat3("particleSpeed",&particleData->particleSpeed.x,-0.1f,0.1f);
	ImGui::Checkbox("randomParticleSpeedX",&particleData->randomParticleSpeedX);
	ImGui::Checkbox("randomParticleSpeedY",&particleData->randomParticleSpeedY);
	ImGui::Checkbox("randomParticleSpeedZ",&particleData->randomParticleSpeedZ);
	ImGui::SliderFloat("particleRandomSpeedX",&particleData->particleRandomSpeedX,-0.1f,0.1f);
	ImGui::SliderFloat("particleRandomSpeedY",&particleData->particleRandomSpeedY,-0.1f,0.1f);
	ImGui::SliderFloat("particleRandomSpeedZ",&particleData->particleRandomSpeedZ,-0.1f,0.1f);


	//パーティクルのサイズ
	ImGui::SliderFloat("particleStertScale",&particleData->particleStertScale,0,50.0f);

	ImGui::Checkbox("randomParticleStertScale",&particleData->randomParticleStertScale);
	ImGui::SliderFloat("particleRandomStertScale",&particleData->particleRandomStertScale,0,5.0f);


	ImGui::SliderFloat("particleEndScale",&particleData->particleEndScale,0,50.0f);

	ImGui::Checkbox("randomParticleEndScale",&particleData->randomParticleEndScale);
	ImGui::SliderFloat("particleRandomEndScale",&particleData->particleRandomEndScale,0,5.0f);

	ImGui::SliderInt("particleLife",&particleData->particleLife,0,30);
	ImGui::SliderInt("easingNmb",&particleData->easingNmb,0,2);


	ImGui::ColorEdit4("particleStertCollor",&particleData->particleStertColor.x);
	ImGui::ColorEdit4("particleCollor",&particleData->particleEndColor.x);

	ImGui::Checkbox("randomParticleStertColor",&particleData->randomParticleStertColor);
	ImGui::Checkbox("randomParticleEndColor",&particleData->randomParticleEndColor);


	//particle->SetColor(particleColor);

}

void ParticleLibrary::LoadCSVfile(const std::string& fileNames)
{
		//ファイルを開く
	std::string kDefaultTextureDirectoryPath = "Resources/ParticleCSVFile/";
	std::string CSVpath = ".csv";
	std::ifstream file;
	std::string fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	file.open(fullPath);
	assert(file.is_open());

	//ファイルを内容を文字列ストリームにコピー
	particleCSV << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void ParticleLibrary::UpdateCSVfile(ParticleManager* particlemana)
{
	//1行分の文字列を入れる変数
	std::string line;
	//初期化
	ResetParticleData();

	//コマンド実行ループ
	while ( getline(particleCSV,line) )
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

		//ファイル名
		if ( word.find("File") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			std::string readFileName = word;

			std::string kDefaultTextureDirectoryPath = "Resources/";
			std::string PngPath = ".png";
			std::string JpgPath = ".jpg";
			std::string texPath = readFileName + PngPath;
			std::string fullPath = kDefaultTextureDirectoryPath + texPath;
			bool isExists;
			isExists = std::filesystem::exists(fullPath);
			if ( isExists )
			{
				particlemana->LoadTexture(texPath);
			}
			texPath = readFileName + JpgPath;
			fullPath = kDefaultTextureDirectoryPath + texPath;
			isExists = std::filesystem::exists(fullPath);
			if ( isExists )
			{
				particlemana->LoadTexture(texPath);
			}

			//particle->LoadTexture(readFileName);
		}
		if ( word.find("particleNmb") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			int x = ( int ) std::atof(word.c_str());


			particleData->particleNmber = x;
		}

		//POPコマンド
		if ( word.find("Pos") == 0 )
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

			particleData->particlePos = { x,y,z };
		}
		if ( word.find("RandomPosButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());
			//y座標
			getline(line_stream,word,',');
			bool y = ( bool ) std::atof(word.c_str());
			//z座標
			getline(line_stream,word,',');
			bool z = ( bool ) std::atof(word.c_str());

			particleData->randomParticlePosX = x;
			particleData->randomParticlePosY = y;
			particleData->randomParticlePosZ = z;
		}
		if ( word.find("RandomPos") == 0 )
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

			particleData->randomParticlePos = { x,y,z };
		}
		if ( word.find("EndPosButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			particleData->endPoint = x;
		}
		if ( word.find("EndPos") == 0 )
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

			particleData->particleEndPos = { x,y,z };
		}
		if ( word.find("EndPointSpeed") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());


			particleData->particleEndPointSpeed = x;
		}

		if ( word.find("RandomSpeedButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());
			//y座標
			getline(line_stream,word,',');
			bool y = ( bool ) std::atof(word.c_str());
			//z座標
			getline(line_stream,word,',');
			bool z = ( bool ) std::atof(word.c_str());

			particleData->randomParticleSpeedX = x;
			particleData->randomParticleSpeedY = y;
			particleData->randomParticleSpeedZ = z;
		}
		if ( word.find("Speed") == 0 )
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

			particleData->particleSpeed = { x,y,z };
		}
		if ( word.find("StertScale") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());

			particleData->particleStertScale = x;
		}
		if ( word.find("RandomStertScaleButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			particleData->randomParticleStertScale = x;
		}
		if ( word.find("RandomStertScale") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());

			particleData->particleRandomStertScale = x;
		}

		if ( word.find("EndScale") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());

			particleData->particleEndScale = x;
		}
		if ( word.find("RandomEndScaleButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			particleData->randomParticleEndScale = x;
		}
		if ( word.find("RandomEndScale") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());

			particleData->particleRandomEndScale = x;
		}
		if ( word.find("Life") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			int x = ( int ) std::atof(word.c_str());

			particleData->particleLife = x;
		}
		if ( word.find("Easing") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			int x = ( int ) std::atof(word.c_str());

			particleData->easingNmb = x;
		}
		if ( word.find("StertColor") == 0 )
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
			getline(line_stream,word,',');
			float w = ( float ) std::atof(word.c_str());

			particleData->particleStertColor = { x,y,z ,w };
		}
		if ( word.find("EndColor") == 0 )
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
			getline(line_stream,word,',');
			float w = ( float ) std::atof(word.c_str());

			particleData->particleEndColor = { x,y,z ,w };
		}
		if ( word.find("RandomStertColorButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			particleData->randomParticleStertColor = x;
		}
		if ( word.find("RandomEndColorButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			particleData->randomParticleEndColor = x;
		}


	}
}

void ParticleLibrary::ResetCSVfile()
{
	particleCSV.str("");
	particleCSV.clear(std::stringstream::goodbit);
	//LoadEnemyPopData();
}

void ParticleLibrary::CreateSaveFile(const std::string& fileNames,bool filePathTrue)
{
	std::string kDefaultTextureDirectoryPath = "Resources/ParticleCSVFile/";
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

	ofs << "File" << "," << particleData->texFileName << "," << std::endl;
	ofs << "particleNmb" << "," << particleData->particleNmber << "," << std::endl;
	ofs << "Pos" << "," << particleData->particlePos.x << "," << particleData->particlePos.y << "," << particleData->particlePos.z << "," << std::endl;
	ofs << "RandomPosButton" << "," << particleData->randomParticlePosX << "," << particleData->randomParticlePosY << "," << particleData->randomParticlePosZ << "," << std::endl;
	ofs << "RandomPos" << "," << particleData->randomParticlePos.x << "," << particleData->randomParticlePos.y << "," << particleData->randomParticlePos.z << "," << std::endl;
	ofs << "EndPosButton" << "," << particleData->endPoint << "," << std::endl;
	ofs << "EndPos" << "," << particleData->particleEndPos.x << "," << particleData->particleEndPos.y << "," << particleData->particleEndPos.z << "," << std::endl;
	ofs << "EndPointSpeed" << "," << particleData->particleEndPointSpeed << "," << std::endl;
	ofs << "RandomSpeedButton" << "," << particleData->randomParticleSpeedX << "," << particleData->randomParticleSpeedY << "," << particleData->randomParticleSpeedZ << "," << std::endl;
	ofs << "Speed" << "," << particleData->particleSpeed.x << "," << particleData->particleSpeed.y << "," << particleData->particleSpeed.z << "," << std::endl;
	ofs << "RandomSpeed" << "," << particleData->particleRandomSpeedX << "," << particleData->particleRandomSpeedY << "," << particleData->particleRandomSpeedZ << "," << std::endl;
	ofs << "StertScale" << "," << particleData->particleStertScale << "," << std::endl;
	ofs << "RandomStertScaleButton" << "," << particleData->randomParticleStertScale << "," << std::endl;
	ofs << "RandomStertScale" << "," << particleData->particleRandomStertScale << "," << std::endl;
	ofs << "EndScale" << "," << particleData->particleEndScale << "," << std::endl;
	ofs << "RandomEndScaleButton" << "," << particleData->randomParticleEndScale << "," << std::endl;
	ofs << "RandomEndScale" << "," << particleData->particleRandomEndScale << "," << std::endl;
	ofs << "Life" << "," << particleData->particleLife << "," << std::endl;
	ofs << "Easing" << "," << particleData->easingNmb << "," << std::endl;
	ofs << "StertColor" << "," << particleData->particleStertColor.x << "," << particleData->particleStertColor.y << "," << particleData->particleStertColor.z << "," << particleData->particleStertColor.w << "," << std::endl;
	ofs << "EndColor" << "," << particleData->particleEndColor.x << "," << particleData->particleEndColor.y << "," << particleData->particleEndColor.z << "," << particleData->particleEndColor.w << "," << std::endl;
	ofs << "RandomStertColorButton" << "," << particleData->randomParticleStertColor << "," << std::endl;
	ofs << "RandomEndColorButton" << "," << particleData->randomParticleEndColor << "," << std::endl;
}

void ParticleLibrary::LoadTexJudgment(const std::string& fileNames)
{
	std::string kDefaultTextureDirectoryPath = "Resources/";
	std::string PngPath = ".png";
	std::string JpgPath = ".jpg";
	std::string texPath = fileNames + PngPath;
	std::string fullPath = kDefaultTextureDirectoryPath + texPath;
	bool isExists = std::filesystem::exists(fullPath);
	if ( isExists )
	{
		particle_->LoadTexture(texPath);
	}
	texPath = fileName + JpgPath;
	fullPath = kDefaultTextureDirectoryPath + texPath;
	isExists = std::filesystem::exists(fullPath);
	if ( isExists )
	{
		particle_->LoadTexture(texPath);
	}

}

void ParticleLibrary::LoadCSVJudgment(const std::string& fileNames,bool filePathTrue)
{
	std::string kDefaultTextureDirectoryPath = "Resources/ParticleCSVFile/";
	std::string CSVpath = ".csv";
	std::string fullPath;
	if ( filePathTrue )
	{
		fullPath = fileNames ;
	}
	else
	{
		fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	}

	bool isExists = std::filesystem::exists(fullPath);
	if ( isExists )
	{
		LoadCSVfile(fileNames);
		UpdateCSVfile(particle_);
	}


}

float ParticleLibrary::RandNmber(float randNmb)
{
	float Nmb = ( float ) rand() / RAND_MAX * randNmb - randNmb / 2.0f;
	return Nmb;

}

void ParticleLibrary::ParticleDataSave(uint32_t nmb,const std::string& fileNames)
{

	if ( particleBox[ nmb ].particleUse == nullptr )
	{
		particleBox[ nmb ].particleUse = new ParticleManager();
		particleBox[ nmb ].particleUse->Initialize();
	}
	ResetCSVfile();
	LoadCSVfile(fileNames);
	UpdateCSVfile(particleBox[ nmb ].particleUse);

	
	particleBox[ nmb ].pData.easingNmb = particleData->easingNmb;
	particleBox[ nmb ].pData.particlePos = particleData->particlePos;
	particleBox[ nmb ].pData.randomParticlePos = particleData->randomParticlePos;
	//終点
	particleBox[ nmb ].pData.particleEndPos = particleData->particleEndPos;
	particleBox[ nmb ].pData.endPointPos = particleData->endPointPos;
	//大きさ
	//最初
	particleBox[ nmb ].pData.particleStertScale = particleData->particleStertScale;
	particleBox[ nmb ].pData.particleRandomStertScale = particleData->particleRandomStertScale;
	//最後
	particleBox[ nmb ].pData.particleEndScale = particleData->particleEndScale;
	particleBox[ nmb ].pData.particleRandomEndScale = particleData->particleRandomEndScale;

	//速度
	particleBox[ nmb ].pData.particleSpeed = particleData->particleSpeed;
	particleBox[ nmb ].pData.particleEndPointSpeed = particleData->particleEndPointSpeed;
	particleBox[ nmb ].pData.particleRandomSpeed = particleData->particleRandomSpeed;
	particleBox[ nmb ].pData.particleRandomSpeedX = particleData->particleRandomSpeedX;
	particleBox[ nmb ].pData.particleRandomSpeedY = particleData->particleRandomSpeedY;
	particleBox[ nmb ].pData.particleRandomSpeedZ = particleData->particleRandomSpeedZ;

	//色
	particleBox[ nmb ].pData.particleStertColor = particleData->particleStertColor;
	particleBox[ nmb ].pData.particleEndColor = particleData->particleEndColor;

	//数
	particleBox[ nmb ].pData.particleNmber = particleData->particleNmber;
	//パーティクルのライフ
	particleBox[ nmb ].pData.particleLife = particleData->particleLife;
	//イージングのナンバー
	particleBox[ nmb ].pData.easingNmb = particleData->easingNmb;

	for ( int i = 0; i < 30; i++ )
	{
		particleBox[ nmb ].pData.texFileName[ i ] = particleData->texFileName[ i ];
	}
	//パーティクルのスイッチ
	particleBox[ nmb ].pData.randomParticleStertColor = particleData->randomParticleStertColor;
	particleBox[ nmb ].pData.randomParticleEndColor = particleData->randomParticleEndColor;
	particleBox[ nmb ].pData.randomParticleSize = particleData->randomParticleSize;
	particleBox[ nmb ].pData.endPoint = particleData->endPoint;
	//ランダムpos

	particleBox[ nmb ].pData.randomParticlePosX = particleData->randomParticlePosX;
	particleBox[ nmb ].pData.randomParticlePosY = particleData->randomParticlePosY;
	particleBox[ nmb ].pData.randomParticlePosZ = particleData->randomParticlePosZ;
	//ランダムspeed
	particleBox[ nmb ].pData.randomParticleSpeedX = particleData->randomParticleSpeedX;
	particleBox[ nmb ].pData.randomParticleSpeedY = particleData->randomParticleSpeedY;
	particleBox[ nmb ].pData.randomParticleSpeedZ = particleData->randomParticleSpeedZ;
	//ランダムscale
	particleBox[ nmb ].pData.randomParticleStertScale = particleData->randomParticleStertScale;
	particleBox[ nmb ].pData.randomParticleEndScale = particleData->randomParticleEndScale;

	ResetParticleData();
}

void ParticleLibrary::AddParticle(uint32_t nmb,Vector3 ObjectPos)
{
	Vector3 particlePoss = particleData->particlePos;

	if ( particleBox[ nmb ].pData.randomParticlePosX )
	{

		particlePoss.x += RandNmber(particleBox[ nmb ].pData.randomParticlePos.x);
	}
	if ( particleBox[ nmb ].pData.randomParticlePosY )
	{

		particlePoss.y += RandNmber(particleBox[ nmb ].pData.randomParticlePos.y);
	}
	if ( particleBox[ nmb ].pData.randomParticlePosZ )
	{

		particlePoss.z += RandNmber(particleBox[ nmb ].pData.randomParticlePos.z);
	}


	if ( particleBox[ nmb ].pData.endPoint )
	{
		if ( particleBox[ nmb ].pData.randomParticleSpeedX )
		{
			particleBox[ nmb ].pData.particleEndPointSpeed = RandNmber(particleBox[ nmb ].pData.particleRandomSpeedX);
		}
		particleBox[ nmb ].pData.endPointPos = ( particleBox[ nmb ].pData.particleEndPos - particlePoss );
		particleBox[ nmb ].pData.endPointPos.nomalize();
		particleBox[ nmb ].pData.endPointPos *= particleBox[ nmb ].pData.particleEndPointSpeed;

	}
	else
	{

		if ( particleBox[ nmb ].pData.randomParticleSpeedX )
		{

			particleBox[ nmb ].pData.particleSpeed.x = RandNmber(particleBox[ nmb ].pData.particleRandomSpeedX);
		}
		if ( particleBox[ nmb ].pData.randomParticleSpeedY )
		{

			particleBox[ nmb ].pData.particleSpeed.y = RandNmber(particleBox[ nmb ].pData.particleRandomSpeedY);
		}
		if ( particleBox[ nmb ].pData.randomParticleSpeedZ )
		{

			particleBox[ nmb ].pData.particleSpeed.z = RandNmber(particleBox[ nmb ].pData.particleRandomSpeedZ);
		}
	}

	if ( particleBox[ nmb ].pData.randomParticleStertScale )
	{

		particleBox[ nmb ].pData.particleStertScale = RandNmber(particleBox[ nmb ].pData.particleRandomStertScale);
		if ( particleBox[ nmb ].pData.particleStertScale < 0 )
		{
			particleBox[ nmb ].pData.particleStertScale = -particleBox[ nmb ].pData.particleStertScale;
		}
	}
	if ( particleBox[ nmb ].pData.randomParticleEndScale )
	{

		particleBox[ nmb ].pData.particleEndScale = RandNmber(particleBox[ nmb ].pData.particleRandomEndScale);
		if ( particleBox[ nmb ].pData.particleEndScale < 0 )
		{
			particleBox[ nmb ].pData.particleEndScale = -particleBox[ nmb ].pData.particleEndScale;
		}
	}

	if ( particleBox[ nmb ].pData.randomParticleStertColor )
	{
		Vector3 randomColor = { static_cast< float > ( rand() % 100 ),static_cast< float > ( rand() % 100 ) ,static_cast< float > ( rand() % 100 ) };
		randomColor.x /= 100;
		randomColor.y /= 100;
		randomColor.z /= 100;
		particleBox[ nmb ].pData.particleStertColor = { randomColor.x,randomColor.y,randomColor.z,particleBox[ nmb ].pData.particleStertColor.w };
	}
	if ( particleBox[ nmb ].pData.randomParticleEndColor )
	{
		Vector3 randomColor = { static_cast< float > ( rand() % 100 ),static_cast< float > ( rand() % 100 ) ,static_cast< float > ( rand() % 100 ) };
		randomColor.x /= 100;
		randomColor.y /= 100;
		randomColor.z /= 100;
		particleBox[ nmb ].pData.particleEndColor = { randomColor.x,randomColor.y,randomColor.z,particleBox[ nmb ].pData.particleEndColor.w };

	}
	for ( int i = 0; i < particleBox[ nmb ].pData.particleNmber; i++ )
	{
		if ( particleBox[ nmb ].pData.endPoint )
		{
			particleBox[ nmb ].particleUse->Add(particleBox[ nmb ].pData.particleLife,particleBox[ nmb ].pData.particlePos+ObjectPos,particleBox[ nmb ].pData.particleEndPos,particleBox[ nmb ].pData.endPointPos,particleBox[ nmb ].pData.particleStertScale,particleBox[ nmb ].pData.particleEndScale,particleBox[ nmb ].pData.easingNmb,particleBox[ nmb ].pData.particleStertColor,particleBox[ nmb ].pData.particleEndColor);
		}
		else
		{
			particleBox[ nmb ].particleUse->Add(particleBox[ nmb ].pData.particleLife,particleBox[ nmb ].pData.particlePos+ ObjectPos,particleBox[ nmb ].pData.particleEndPos,particleBox[ nmb ].pData.particleSpeed,particleBox[ nmb ].pData.particleStertScale,particleBox[ nmb ].pData.particleEndScale,particleBox[ nmb ].pData.easingNmb,particleBox[ nmb ].pData.particleStertColor,particleBox[ nmb ].pData.particleEndColor);
		}
	}
	
}

void ParticleLibrary::ParticleUpdate(uint32_t nmb)
{
	particleBox[ nmb ].particleUse->Update();
}

void ParticleLibrary::ParticleDraw(uint32_t nmb)
{
	particleBox[ nmb ].particleUse->Draw();
}

void ParticleLibrary::Finalize()
{
	for ( int i = 0; i < particleBoxSaves; i++ )
	{
		if ( particleBox[ i ].particleUse != nullptr )
		{
			delete particleBox[ i ].particleUse;
		}
	}
	delete particle_;
	delete particleLibrary;
}

void ParticleLibrary::TxtUpdate()
{
	std::vector<std::string> CSVFileNames;
	TxtRoad("Resources/ParticleCSVFile",CSVFileNames);

	for ( int i = 0; i < CSVFileNames.size(); i++ )
	{
		ResetCSVfile();
		LoadCSVJudgment(CSVFileNames[ i ],true);
		ResetCSVfile();
		CreateSaveFile(CSVFileNames[ i ],true);
	}

}

void ParticleLibrary::TxtRoad(std::string folderPath,std::vector<std::string>& file_names)
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

void ParticleLibrary::ResetParticleData()
{
	particleData->particlePos=particleClear->particlePos;
	particleData->randomParticlePos=particleClear->randomParticlePos;
	particleData->particleEndPos=particleClear->particleEndPos;
	particleData->endPointPos=particleClear->endPointPos;
	particleData->particleStertScale=particleClear->particleStertScale;;
	particleData->particleRandomStertScale= particleClear->particleRandomStertScale;
	particleData->particleEndScale= particleClear->particleEndScale;
	particleData->particleRandomEndScale= particleClear->particleRandomEndScale;
	particleData->particleSpeed= particleClear->particleSpeed;
	particleData->particleEndPointSpeed= particleClear->particleEndPointSpeed;
	particleData->particleRandomSpeed= particleClear->particleRandomSpeed;
	particleData->particleRandomSpeedX= particleClear->particleRandomSpeedX;
	particleData->particleRandomSpeedY= particleClear->particleRandomSpeedY;
	particleData->particleRandomSpeedZ= particleClear->particleRandomSpeedZ;
	particleData->particleStertColor= particleClear->particleStertColor;
	particleData->particleEndColor= particleClear->particleEndColor;
	particleData->particleNmber= particleClear->particleNmber;
	particleData->particleLife= particleClear->particleLife;
	particleData->easingNmb= particleClear->easingNmb;
	for ( int i = 0; i < 30; i++ )
	{
		particleData->texFileName[i ] = particleClear->texFileName[i];
	}

	particleData->randomParticleStertColor= particleClear->randomParticleStertColor;
	particleData->randomParticleEndColor= particleClear->randomParticleEndColor;
	particleData->randomParticleSize= particleClear->randomParticleSize;
	particleData->endPoint= particleClear->endPoint;
	particleData->randomParticlePosX= particleClear->randomParticlePosX;
	particleData->randomParticlePosY= particleClear->randomParticlePosY;
	particleData->randomParticlePosZ= particleClear->randomParticlePosZ;
	particleData->randomParticleSpeedX= particleClear->randomParticleSpeedX;
	particleData->randomParticleSpeedY= particleClear->randomParticleSpeedY;
	particleData->randomParticleSpeedZ= particleClear->randomParticleSpeedZ;
	particleData->randomParticleStertScale= particleClear->randomParticleStertScale;
	particleData->randomParticleEndScale= particleClear->randomParticleEndScale;
}

ParticleLibrary::~ParticleLibrary()
{

}
