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
	player->Update();
}

void ParticleLibrary::ObjectUpdate()
{
	player->Update();
}

void ParticleLibrary::ObjectDraw()
{
	player->Draw();
}

void ParticleLibrary::PDraw()
{
	particle_->Draw();
}

void ParticleLibrary::Update()
{
	Vector3 particlePoss = particlePos;

	if ( randomParticlePosX )
	{

		particlePoss.x += RandNmber(randomParticlePos.x);
	}
	if ( randomParticlePosY )
	{

		particlePoss.y += RandNmber(randomParticlePos.y);
	}
	if ( randomParticlePosZ )
	{

		particlePoss.z += RandNmber(randomParticlePos.z);
	}


	if ( endPoint )
	{
		if ( randomParticleSpeedX )
		{
			particleEndPointSpeed = RandNmber(particleRandomSpeedX);
		}
		endPointPos = ( particleEndPos - particlePoss );
		endPointPos.nomalize();
		endPointPos *= particleEndPointSpeed;

	}
	else
	{

		if ( randomParticleSpeedX )
		{

			particleSpeed.x = RandNmber(particleRandomSpeedX);
		}
		if ( randomParticleSpeedY )
		{

			particleSpeed.y = RandNmber(particleRandomSpeedY);
		}
		if ( randomParticleSpeedZ )
		{

			particleSpeed.z = RandNmber(particleRandomSpeedZ);
		}
	}

	if ( randomParticleStertScale )
	{

		particleStertScale = RandNmber(particleRandomStertScale);
		if ( particleStertScale < 0 )
		{
			particleStertScale = -particleStertScale;
		}
	}
	if ( randomParticleEndScale )
	{

		particleEndScale = RandNmber(particleRandomEndScale);
		if ( particleEndScale < 0 )
		{
			particleEndScale = -particleEndScale;
		}
	}

	if ( randomParticleStertColor )
	{
		Vector3 randomColor = { static_cast< float > ( rand() % 100 ),static_cast< float > ( rand() % 100 ) ,static_cast< float > ( rand() % 100 ) };
		randomColor.x /= 100;
		randomColor.y /= 100;
		randomColor.z /= 100;
		particleStertColor = { randomColor.x,randomColor.y,randomColor.z,1 };
	}
	if ( randomParticleEndColor )
	{
		Vector3 randomColor = { static_cast< float > ( rand() % 100 ),static_cast< float > ( rand() % 100 ) ,static_cast< float > ( rand() % 100 ) };
		randomColor.x /= 100;
		randomColor.y /= 100;
		randomColor.z /= 100;
		particleEndColor = { randomColor.x,randomColor.y,randomColor.z,1 };

	}

	for ( int i = 0; i < particleNmber; i++ )
	{
		if ( endPoint )
		{
			particle_->Add(particleLife,particlePoss,particleEndPos,endPointPos,particleStertScale,particleEndScale,easingNmb,particleStertColor,particleEndColor);
		}
		else
		{
			particle_->Add(particleLife,particlePoss,particleEndPos,particleSpeed,particleStertScale,particleEndScale,easingNmb,particleStertColor,particleEndColor);
		}
	}
	particle_->Update();
}

void ParticleLibrary::DrawImgui()
{
	ImGui::InputText("ObjectFileName",objectFileName,sizeof(objectFileName));
	loadObject = ImGui::Button("loadObject");
	if ( loadObject )
	{
		playerModel = ModelManager::GetInstance()->FindObjModel(objectFileName);
		player->SetModel(playerModel);
	}
	ImGui::SliderFloat("playerRot",&playerRot,-playerRotLimit,playerRotLimit);
	player->wtf.rotation.y = playerRot * rot;
	player->Update();

	ImGui::InputText("CSVFileName",fileName,sizeof(fileName));
	loadParticle = ImGui::Button("loadParticle");

	saveParticle = ImGui::Button("saveParticle");
	if ( ImGui::Button("ResetParticle") )
	{
		LoadCSVJudgment("Reset");
		ResetCSVfile();
	}
	if ( saveParticle )
	{
		CreateSaveFile(fileName);

	}
	if ( loadParticle )
	{
		LoadCSVJudgment(fileName);
		ResetCSVfile();

	}

	ImGui::InputText("TextureFileName",texFileName,sizeof(texFileName));
	loadTexSwich = ImGui::Button("loadTexture");
	if ( loadTexSwich )
	{
		LoadTexJudgment(texFileName);
	}
	ImGui::SliderInt("particleNmber",&particleNmber,0,70);
	ImGui::SliderFloat3("particlePos",&particlePos.x,-5.0f,5.0f);
	ImGui::Checkbox("randomParticlePosX",&randomParticlePosX);
	ImGui::Checkbox("randomParticlePosY",&randomParticlePosY);
	ImGui::Checkbox("randomParticlePosZ",&randomParticlePosZ);
	ImGui::SliderFloat3("particlerandomPosX",&randomParticlePos.x,-5.0f,5.0f);

	ImGui::Checkbox("endPoint",&endPoint);
	ImGui::SliderFloat("particleEndPointSpeed",&particleEndPointSpeed,0,3.0f);
	ImGui::Checkbox("randomParticleSpeed",&randomParticleSpeedX);
	ImGui::SliderFloat("particleRandomSpeed",&particleRandomSpeedX,-3.0f,3.0f);
	ImGui::SliderFloat3("particleEndPos",&particleEndPos.x,-5.0f,5.0f);
	ImGui::SliderFloat3("particleSpeed",&particleSpeed.x,-0.1f,0.1f);
	ImGui::Checkbox("randomParticleSpeedX",&randomParticleSpeedX);
	ImGui::Checkbox("randomParticleSpeedY",&randomParticleSpeedY);
	ImGui::Checkbox("randomParticleSpeedZ",&randomParticleSpeedZ);
	ImGui::SliderFloat("particleRandomSpeedX",&particleRandomSpeedX,-0.1f,0.1f);
	ImGui::SliderFloat("particleRandomSpeedY",&particleRandomSpeedY,-0.1f,0.1f);
	ImGui::SliderFloat("particleRandomSpeedZ",&particleRandomSpeedZ,-0.1f,0.1f);


	ImGui::SliderFloat("particleStertScale",&particleStertScale,0,5.0f);

	ImGui::Checkbox("randomParticleStertScale",&randomParticleStertScale);
	ImGui::SliderFloat("particleRandomStertScale",&particleRandomStertScale,0,5.0f);


	ImGui::SliderFloat("particleEndScale",&particleEndScale,0,5.0f);

	ImGui::Checkbox("randomParticleEndScale",&randomParticleEndScale);
	ImGui::SliderFloat("particleRandomEndScale",&particleRandomEndScale,0,5.0f);

	ImGui::SliderInt("particleLife",&particleLife,0,30);
	ImGui::SliderInt("easingNmb",&easingNmb,0,2);


	ImGui::ColorEdit4("particleStertCollor",&particleStertColor.x);
	ImGui::ColorEdit4("particleCollor",&particleEndColor.x);

	ImGui::Checkbox("randomParticleStertColor",&randomParticleStertColor);
	ImGui::Checkbox("randomParticleEndColor",&randomParticleEndColor);

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


			particleNmber = x;
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

			particlePos = { x,y,z };
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

			randomParticlePosX = x;
			randomParticlePosY = y;
			randomParticlePosZ = z;
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

			randomParticlePos = { x,y,z };
		}
		if ( word.find("EndPosButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			endPoint = x;
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

			particleEndPos = { x,y,z };
		}
		if ( word.find("EndPointSpeed") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());


			particleEndPointSpeed = x;
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

			randomParticleSpeedX = x;
			randomParticleSpeedY = y;
			randomParticleSpeedZ = z;
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

			particleSpeed = { x,y,z };
		}
		if ( word.find("StertScale") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());

			particleStertScale = x;
		}
		if ( word.find("RandomStertScaleButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			randomParticleStertScale = x;
		}
		if ( word.find("RandomStertScale") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());

			particleRandomStertScale = x;
		}

		if ( word.find("EndScale") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());

			particleEndScale = x;
		}
		if ( word.find("RandomEndScaleButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			randomParticleEndScale = x;
		}
		if ( word.find("RandomEndScale") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			float x = ( float ) std::atof(word.c_str());

			particleRandomEndScale = x;
		}
		if ( word.find("Life") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			int x = ( int ) std::atof(word.c_str());

			particleLife = x;
		}
		if ( word.find("Easing") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			int x = ( int ) std::atof(word.c_str());

			easingNmb = x;
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

			particleStertColor = { x,y,z ,w };
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

			particleEndColor = { x,y,z ,w };
		}
		if ( word.find("RandomStertColorButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			randomParticleStertColor = x;
		}
		if ( word.find("RandomEndColorButton") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			bool x = ( bool ) std::atof(word.c_str());

			randomParticleEndColor = x;
		}


	}
}

void ParticleLibrary::ResetCSVfile()
{
	particleCSV.str("");
	particleCSV.clear(std::stringstream::goodbit);
	//LoadEnemyPopData();
}

void ParticleLibrary::CreateSaveFile(const std::string& fileNames)
{
	std::string kDefaultTextureDirectoryPath = "Resources/ParticleCSVFile/";
	std::string CSVpath = ".csv";
	std::string fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	std::ofstream ofs(fullPath);

	ofs << "File" << "," << texFileName << "," << std::endl;
	ofs << "particleNmb" << "," << particleNmber << "," << std::endl;
	ofs << "Pos" << "," << particlePos.x << "," << particlePos.y << "," << particlePos.z << "," << std::endl;
	ofs << "RandomPosButton" << "," << randomParticlePosX << "," << randomParticlePosY << "," << randomParticlePosZ << "," << std::endl;
	ofs << "RandomPos" << "," << randomParticlePos.x << "," << randomParticlePos.y << "," << randomParticlePos.z << "," << std::endl;
	ofs << "EndPosButton" << "," << endPoint << "," << std::endl;
	ofs << "EndPos" << "," << particleEndPos.x << "," << particleEndPos.y << "," << particleEndPos.z << "," << std::endl;
	ofs << "EndPointSpeed" << "," << particleEndPointSpeed << "," << std::endl;
	ofs << "RandomSpeedButton" << "," << randomParticleSpeedX << "," << randomParticleSpeedY << "," << randomParticleSpeedZ << "," << std::endl;
	ofs << "Speed" << "," << particleSpeed.x << "," << particleSpeed.y << "," << particleSpeed.z << "," << std::endl;
	ofs << "RandomSpeed" << "," << particleRandomSpeedX << "," << particleRandomSpeedY << "," << particleRandomSpeedZ << "," << std::endl;
	ofs << "StertScale" << "," << particleStertScale << "," << std::endl;
	ofs << "RandomStertScaleButton" << "," << randomParticleStertScale << "," << std::endl;
	ofs << "RandomStertScale" << "," << particleRandomStertScale << "," << std::endl;
	ofs << "EndScale" << "," << particleEndScale << "," << std::endl;
	ofs << "RandomEndScaleButton" << "," << randomParticleEndScale << "," << std::endl;
	ofs << "RandomEndScale" << "," << particleRandomEndScale << "," << std::endl;
	ofs << "Life" << "," << particleLife << "," << std::endl;
	ofs << "Easing" << "," << easingNmb << "," << std::endl;
	ofs << "StertColor" << "," << particleStertColor.x << "," << particleStertColor.y << "," << particleStertColor.z << "," << particleStertColor.w << "," << std::endl;
	ofs << "EndColor" << "," << particleEndColor.x << "," << particleEndColor.y << "," << particleEndColor.z << "," << particleEndColor.w << "," << std::endl;
	ofs << "RandomStertColorButton" << "," << randomParticleStertColor << "," << std::endl;
	ofs << "RandomEndColorButton" << "," << randomParticleEndColor << "," << std::endl;
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

void ParticleLibrary::LoadCSVJudgment(const std::string& fileNames)
{
	std::string kDefaultTextureDirectoryPath = "Resources/ParticleCSVFile/";
	std::string CSVpath = ".csv";
	std::string fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
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

	
	particleBox[ nmb ].pData.easingNmb = easingNmb;
	particleBox[ nmb ].pData.particlePos = particlePos;
	particleBox[ nmb ].pData.randomParticlePos = randomParticlePos;
	//終点
	particleBox[ nmb ].pData.particleEndPos = particleEndPos;
	particleBox[ nmb ].pData.endPointPos = endPointPos;
	//大きさ
	//最初
	particleBox[ nmb ].pData.particleStertScale = particleStertScale;
	particleBox[ nmb ].pData.particleRandomStertScale = particleRandomStertScale;
	//最後
	particleBox[ nmb ].pData.particleEndScale = particleEndScale;
	particleBox[ nmb ].pData.particleRandomEndScale = particleRandomEndScale;

	//速度
	particleBox[ nmb ].pData.particleSpeed = particleSpeed;
	particleBox[ nmb ].pData.particleEndPointSpeed = particleEndPointSpeed;
	particleBox[ nmb ].pData.particleRandomSpeed = particleRandomSpeed;
	particleBox[ nmb ].pData.particleRandomSpeedX = particleRandomSpeedX;
	particleBox[ nmb ].pData.particleRandomSpeedY = particleRandomSpeedY;
	particleBox[ nmb ].pData.particleRandomSpeedZ = particleRandomSpeedZ;

	//色
	particleBox[ nmb ].pData.particleStertColor = particleStertColor;
	particleBox[ nmb ].pData.particleEndColor = particleEndColor;

	//数
	particleBox[ nmb ].pData.particleNmber = particleNmber;
	//パーティクルのライフ
	particleBox[ nmb ].pData.particleLife = particleLife;
	//イージングのナンバー
	particleBox[ nmb ].pData.easingNmb = easingNmb;

	for ( int i = 0; i < 30; i++ )
	{
		particleBox[ nmb ].pData.texFileName[ i ] = texFileName[ i ];
	}
	//パーティクルのスイッチ
	particleBox[ nmb ].pData.randomParticleStertColor = randomParticleStertColor;
	particleBox[ nmb ].pData.randomParticleEndColor = randomParticleEndColor;
	particleBox[ nmb ].pData.randomParticleSize = randomParticleSize;
	particleBox[ nmb ].pData.endPoint = endPoint;
	//ランダムpos

	particleBox[ nmb ].pData.randomParticlePosX = randomParticlePosX;
	particleBox[ nmb ].pData.randomParticlePosY = randomParticlePosY;
	particleBox[ nmb ].pData.randomParticlePosZ = randomParticlePosZ;
	//ランダムspeed
	particleBox[ nmb ].pData.randomParticleSpeedX = randomParticleSpeedX;
	particleBox[ nmb ].pData.randomParticleSpeedY = randomParticleSpeedY;
	particleBox[ nmb ].pData.randomParticleSpeedZ = randomParticleSpeedZ;
	//ランダムscale
	particleBox[ nmb ].pData.randomParticleStertScale = randomParticleStertScale;
	particleBox[ nmb ].pData.randomParticleEndScale = randomParticleEndScale;

}

void ParticleLibrary::AddParticle(uint32_t nmb,Vector3 ObjectPos)
{
	Vector3 particlePoss = particlePos;

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
		if ( endPoint )
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

ParticleLibrary::~ParticleLibrary()
{

}
