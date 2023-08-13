#pragma once

#include "engine/shared/AABB.h"
#include "engine/shared/Shader.h"

#include "glm/glm.hpp"
#include <iostream>

enum class TriggerMode : unsigned short
{
	ONCE,
	ONCE_AND_RETRIGGER, //Temp not used
	ONCE_AND_RESET,		//Temp not used
	MULTIPLE
};


class TriggerBox
{
public:
	TriggerBox();
	TriggerBox(glm::vec3 position, AABB &volume, TriggerMode mode = TriggerMode::MULTIPLE);

	bool Intersect(const glm::vec3 &point);

	void getModeStateInfo() const;
	glm::vec3 getPosition() const;

	void setPosition(glm::vec3 &position);
	void setSize(AABB &size);
	void setMode(TriggerMode mode);

private:

	glm::vec3 position;
	AABB size;

	TriggerMode mode;
	bool isActivated;
	bool isInside;
};