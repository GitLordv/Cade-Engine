#pragma once

#include "pugixml.hpp"
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "cppfs/FilePath.h"

#include "engine/shared/Utils.h"
#include "engine/shared/UVRect.h"
#include "engine/Texture.h"
#include "engine/Camera.h"


struct Level
{
	struct Audio
	{
		std::string path;
		bool isLoop;
	};

	struct AdditionalBehaviour
	{
		bool isAdditional;

		//Options - Opacity Fade
		bool isFade;
		float fadeEnd;

		//Options - ...
	};

	struct Sprite
	{
		struct Animation
		{
			std::string type;
			float speed;
		};

		std::string name;
		std::shared_ptr<Texture> texture;
		UVRect uvRect;
		glm::vec3 position;
		glm::vec2 scSpeed;
		glm::vec3 rotation;
		glm::vec2 size;
		glm::vec4 tint;
		bool isAnim;
		bool isLight;
		bool useFog;
		Animation animation;
		AdditionalBehaviour addin;
	};

	Audio audio;
	std::vector<Sprite> sprites;
};


class LevelsSystem
{
public:

	LevelsSystem(Camera &cam);

	auto LoadLevel(std::string_view path) -> Level;
	auto SaveLevel(std::string_view path) -> void;
	auto PreloadEngineResources(std::string_view path) -> void;

private:

	template <typename T>
	auto SplitStringToVec(const std::string &str) -> T;
	auto SplitStringToVec2(const std::string &str) -> std::vector<glm::vec2>;
	auto GetDebugInfo() -> void;

	std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
	std::shared_ptr<Texture> missingTexture;

	Camera &cam;
};