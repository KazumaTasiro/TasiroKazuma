#include "JsonLoader.h"

#include "json.hpp"
#include <fstream>
#include <cassert>

const std::string JsonLoader::kDefaultBaseDirectory = "Resources/";
const std::string JsonLoader::kExtension = ".json";

LevelData* JsonLoader::LoadFile(const std::string& filename)
{
	//ファイル読みこみ
	//連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + filename + kExtension;

	//ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//オブジェクトの走査
	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESH
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.filename = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.x = transform["translation"][1];
			objectData.translation.y = transform["translation"][2];
			objectData.translation.z = transform["translation"][0];
			objectData.translation.z = -objectData.translation.z;
			objectData.translation.w = 1.0f;


			//回転角
			objectData.rotation.x = transform["rotation"][1];
			objectData.rotation.x = -objectData.rotation.x;
			objectData.rotation.y = transform["rotation"][2];
			objectData.rotation.y = -objectData.rotation.y;
			objectData.rotation.z = transform["rotation"][0];
			objectData.rotation.w = 0.0f;


			//スケーリング
			objectData.scaling.x = transform["scaling"][1];
			objectData.scaling.y = transform["scaling"][2];
			objectData.scaling.z = transform["scaling"][0];
			objectData.scaling.w = 0.0f;
		}

		//再帰処理
		//TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
	}

	return levelData;
}
