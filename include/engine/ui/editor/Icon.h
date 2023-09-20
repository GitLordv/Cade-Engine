#pragma once

#include "engine/shared/UVRect.h"

#include "glm/gtc/type_ptr.hpp"


enum class IconType : unsigned short
{
	FRIZE,
	BOUNCE_TARGET,
	TWEEN,
	TRIGGER,
	TRIGGER_INACTIVE,
	NONE
};

struct IconData
{
	IconType type      = IconType::NONE;
	UVRect uv		   = UVRect(0.0F, 0.0F, 1.0F, 1.0F);
	glm::vec3 position = glm::vec3(0.0F,0.0F,0.0F);
	glm::vec2 size	   = glm::vec2(1.0F, 1.0F);
	float angle		   = 0.0F;
};