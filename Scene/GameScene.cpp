#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete camera;
	delete levelData;
}
void GameScene::Initialize(WinApp* winApp, DirectXCommon* dxcomon, Input* input_)
{
	assert(dxcomon);
	assert(input_);

	input = input_;

	winApp_ = winApp;

	dxCommon_ = dxcomon;

	audio = new Audio();
	audio->Initialize();

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	camWtf.Initialize();
	camWtf.position = { 0.0f, 5.0f, -15.0f };
	camera->MoveVector(camWtf.position);
	camera->SetTarget({ 0,0,0 });

	camera->Update();
	ParticleManager::SetCamera(camera);
	ParticleManager2::SetCamera(camera);
	Object3d::SetCamera(camera);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

	spriteCommon->LoadTexture(0, "meemu.jpg");
	spriteCommon->LoadTexture(1, "wakka.jpg");

	//���x���f�[�^
	levelData = JsonLoader::LoadFile("jsonTest");

	modelBox = Model::LoadFromOBJ("cube");
	modelSphere = Model::LoadFromOBJ("gorilla");

	models.insert(std::make_pair("spher", modelSphere));
	models.insert(std::make_pair("box", modelBox));

	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		// �t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end()) {
			model = it->second;
		}

		// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(model);

		// ���W
		Vector3 pos;
		pos.x = objectData.translation.x;
		pos.y = objectData.translation.y;
		pos.z = objectData.translation.z;
		newObject->wtf.position = pos;

		// ��]�p
		Vector3 rot;
		rot.x = objectData.rotation.x;
		rot.y = objectData.rotation.y;
		rot.z = objectData.rotation.z;
		newObject->wtf.rotation = rot;

		// ���W
		Vector3 scale;
		scale.x = objectData.scaling.x;
		scale.y = objectData.scaling.y;
		scale.z = objectData.scaling.z;
		newObject->wtf.scale = scale;

		// �z��ɓo�^
		objects.push_back(newObject);
	}
}
void GameScene::Update()
{
	for (auto& object : objects) {
		object->Update();
	}

	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	//�f���E�B���h�E�̕\��ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();

	//EffectAdd();
	camera->Update();
}
void GameScene::Draw()
{



	Object3d::PreDraw(dxCommon_->GetCommandList());

	for (auto& object : objects) {
		object->Draw();
	}

	Object3d::PostDraw();

	/*ImGuiMan->Draw();*/

	spriteCommon->PreDraw();

	// �p�[�e�B�N���`��O����
	ParticleManager::PreDraw(dxCommon_->GetCommandList());


	// �p�[�e�B�N���`��㏈��
	ParticleManager::PostDraw();

	ParticleManager2::PreDraw(dxCommon_->GetCommandList());
	ParticleManager2::PostDraw();
}


void GameScene::Finalize()
{
	ImGuiMan->Finalize();
	audio->Finalize();

	delete audio;
	//ImGui�̊J��
	delete ImGuiMan;



	//�X�v���C�g�̊J��
	delete spriteCommon;
}

void GameScene::EffectAdd()
{
}
