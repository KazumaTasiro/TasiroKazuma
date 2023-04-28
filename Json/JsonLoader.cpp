#include "JsonLoader.h"

#include "json.hpp"
#include <fstream>
#include <cassert>

const std::string JsonLoader::kDefaultBaseDirectory = "Resources/";
const std::string JsonLoader::kExtension = ".json";

LevelData* JsonLoader::LoadFile(const std::string& filename)
{
	//�t�@�C���ǂ݂���
	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = kDefaultBaseDirectory + filename + kExtension;

	//�t�@�C���X�g���[��
	std::ifstream file;

	// �t�@�C�����J��
	file.open(fullpath);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//�I�u�W�F�N�g�̑���
	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* levelData = new LevelData();

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//��ނ��Ƃ̏���
		//MESH
		if (type.compare("MESH") == 0) {
			//�v�f�ǉ�
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// ���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//�t�@�C����
				objectData.filename = object["file_name"];
			}

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			objectData.translation.x = transform["translation"][1];
			objectData.translation.y = transform["translation"][2];
			objectData.translation.z = transform["translation"][0];
			objectData.translation.z = -objectData.translation.z;
			objectData.translation.w = 1.0f;


			//��]�p
			objectData.rotation.x = transform["rotation"][1];
			objectData.rotation.x = -objectData.rotation.x;
			objectData.rotation.y = transform["rotation"][2];
			objectData.rotation.y = -objectData.rotation.y;
			objectData.rotation.z = transform["rotation"][0];
			objectData.rotation.w = 0.0f;


			//�X�P�[�����O
			objectData.scaling.x = transform["scaling"][1];
			objectData.scaling.y = transform["scaling"][2];
			objectData.scaling.z = transform["scaling"][0];
			objectData.scaling.w = 0.0f;
		}

		//�ċA����
		//TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
		if (object.contains("children")) {

		}
	}

	return levelData;
}