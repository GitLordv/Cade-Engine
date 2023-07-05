#pragma once

#include "engine/shared/Utils.h"
#include "engine/shared/Types.h"
#include "yaml-cpp/yaml.h"


class Level
{
public:
	Level();
	Level(const std::string& filePath);

	void load(const std::string filePath);
	void save();

private:

	YAML::Node level;
	std::vector<LevelTexture> texturesList;
	std::vector<SpriteData> spritesDataList;
};

