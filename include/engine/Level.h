#pragma once

#include "engine/shared/Utils.h"
#include "engine/shared/Types.h"
#include "pugixml.hpp"

#define LEVEL

class Level
{
public:

	static std::vector<SpriteData> LoadLevel(std::string_view filePath);
	static void SaveLevel(const std::string &filename);

	static inline std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;

private:

};