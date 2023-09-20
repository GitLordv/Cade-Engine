#pragma once

#include "pugixml.hpp"
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "cppfs/FilePath.h"

#include "engine/shared/Utils.h"
#include "engine/shared/UVRect.h"
#include "engine/Texture.h"
#include "engine/Camera.h"


struct ExtData
{
    bool isAdditional;
    bool isFade;
    float fadeEnd;
};

struct Anim
{
    std::string type;
    float speed;
};

struct SpriteData
{
    SpriteData() : name({}), texture(nullptr), uvRect({}), position({}),
        scSpeed({}), rotation({}), size(glm::vec2(1.0F)), tint(glm::vec4(1.0F)),
        uvScale(1.0F), isAnim(false), isLight(false), isUvAsPixels(false),
        useFog(true), isWorldUV(false), animation({}), addin({})
    {};

    std::string name;
    std::shared_ptr<Texture> texture;
    UVRect uvRect;
    glm::vec3 position;
    glm::vec2 scSpeed;
    glm::vec3 rotation;
    glm::vec2 size;
    glm::vec4 tint;
    float uvScale;
    bool isAnim;
    bool isLight;
    bool isUvAsPixels;
    bool useFog;
    bool isWorldUV;
    Anim animation;
    ExtData addin;
};

struct LevelData
{
    std::vector<SpriteData> sprites;
};


class Level
{
public:

	Level(const std::string &path, Camera &cam);

	auto LoadLevel(const std::string &path) -> LevelData;
	auto SaveLevel(std::string_view path) -> void;

	auto getLoadedLevel() -> std::optional<LevelData>;


private:

	template <typename T>
	auto SplitStringToVec(const std::string &str) -> T;


	std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
	std::shared_ptr<Texture> missingTexture;

	Camera &cam;
	LevelData loadedLevel;
};