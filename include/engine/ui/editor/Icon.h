#pragma once

#include "engine/shared/UVRect.h"

#include "glm/gtc/type_ptr.hpp"


enum class IconType : unsigned short
{
	FRIZE,
	BOUNCE_TARGET,
	TWEEN,
	TRIGGER,
	TRIGGER_INACTIVE
};

struct Icon
{
	IconType type;
	UVRect uv;
	glm::vec3 position;
	glm::vec2 size;
	float angle;
};