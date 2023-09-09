#include "engine/TriggerBox.h"
#include "engine/shared/Utils.h"

TriggerBox::TriggerBox()
	: isInside(false), isActivated(true), mode(TriggerMode::MULTIPLE), position(glm::vec3(0.0F)), size(AABB()) {}

TriggerBox::TriggerBox(glm::vec3 position, AABB &volume, TriggerMode mode)
	: isInside(false), isActivated(true), mode(mode), position(position), size(volume) {}

bool TriggerBox::OnEnter(const glm::vec3 &point)
{
	//AABB boundingBox(position - size.getMin() / 2.0F, position + size.getMax() / 2.0F);
	AABB boundingBox(size.getMin(), size.getMax());
	boundingBox.setPosition(position);
	bool isIntersect = boundingBox.IntersectPoint(point);

	if (mode == TriggerMode::ONCE)
	{
		if (isIntersect && !isActivated && !isInside)
		{
			isInside = true;
			std::cout << "TriggerBox was re-triggered" << std::endl;
			return false;
		}

		if (isIntersect && isActivated && !isInside)
		{
			isInside = true;
			isActivated = false;
			std::cout << "TriggerBox was triggered" << std::endl;
			return true;
		}
	}
	
	if (mode == TriggerMode::MULTIPLE)
	{
		if (isIntersect && !isInside)
		{
			isInside = true;
			std::cout << "TriggerBox was triggered" << std::endl;
			return true;
		}
	}

	if (!isIntersect)
	{
		isInside = false;
	}

	return false;
}


void TriggerBox::getModeStateInfo() const
{
	switch (mode)
	{
		case TriggerMode::ONCE: std::cout << "Trigger mode -> Once\n";
			break;
		case TriggerMode::MULTIPLE: std::cout << "Trigger mode -> Multiple\n";
			break;
		case TriggerMode::ONCE_AND_RESET: std::cout << "Trigger mode -> OnceAndReset\n";
			break;
		case TriggerMode::ONCE_AND_RETRIGGER: std::cout << "Trigger mode -> OnceAndRetrigger\n";
			break;
		default: std::cout << "Trigger mode (default) -> Multiple\n";
			break;
	}
}

AABB TriggerBox::getSizeAABB() const
{
	return size;
}

glm::vec3 TriggerBox::getSizeVec3() const
{
	return size.getMax() - size.getMin();
}

glm::vec3 TriggerBox::getPosition() const
{
	return position;
}

void TriggerBox::setPosition(glm::vec3 &position)
{
	this->position = position;
}

void TriggerBox::setSize(AABB &size)
{
	this->size = size;
}

void TriggerBox::setMode(TriggerMode mode)
{
	this->mode = mode;
}
