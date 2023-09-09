#pragma once

#include "engine/shared/AABB.h"
#include "engine/shared/Shader.h"

#include "glm/glm.hpp"
#include <iostream>

enum class TriggerMode : unsigned short
{
	ONCE,
	ONCE_AND_RETRIGGER, 
	ONCE_AND_RESET,	
	MULTIPLE
};


class TriggerBox
{
public:
	TriggerBox();
	TriggerBox(glm::vec3 position, AABB &volume, TriggerMode mode = TriggerMode::MULTIPLE);

	bool OnEnter(const glm::vec3 &point);

	void getModeStateInfo() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSizeVec3() const;
	AABB getSizeAABB() const;

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