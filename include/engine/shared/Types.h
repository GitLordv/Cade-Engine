#pragma once

#include "engine/Texture.h"
#include "engine/shared/UVRegion.h"
#include <iostream>


struct CameraConfig
{
	glm::vec3 position{0.0f};
	float speed = 0.9f;
	float sens = 0.09f;
};

struct SpriteData
{
	std::string name;
	std::shared_ptr<Texture> texture;
	UVRegion uvRegion;
	glm::vec3 position;
	glm::vec2 scSpeed;
	glm::vec3 rotation;
	glm::vec2 size;
	glm::vec4 tint;
};
