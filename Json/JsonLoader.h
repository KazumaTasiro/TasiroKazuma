#pragma once

#include"Vector3.h"
#include"Vector4.h"
#include <string>
#include <vector>

struct LevelData
{

	struct ObjectData
	{
		std::string filename;

		Vector4 rotation;
		Vector4 scaling;
		Vector4 translation;
	};
	std::vector<ObjectData> objects;
};

class JsonLoader {
public:
	static LevelData* LoadFile(const std::string& filename);

private:

	static const std::string kDefaultBaseDirectory;
	static const std::string kExtension;
};