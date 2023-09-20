#include "engine/shared/AABB.h"

AABB::AABB() : min(glm::vec3(0.0F)), max(glm::vec3(1.0F)), position(glm::vec3(0.0F)) {}

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max), position(glm::vec3(0.0F)) {}

bool AABB::IntersectAABB(const AABB &another) const
{
    if (max.x < another.min.x || min.x > another.max.x) return false;
    if (max.y < another.min.y || min.y > another.max.y) return false;
    if (max.z < another.min.z || min.z > another.max.z) return false;
    return true;
}

bool AABB::IntersectPoint(const glm::vec2 &point) const
{
    return (point.x >= min.x && point.x <= max.x) &&
           (point.y >= min.y && point.y <= max.y);
}

bool AABB::IntersectPoint(const glm::vec3 &point) const
{
    return (point.x >= min.x && point.x <= max.x) &&
           (point.y >= min.y && point.y <= max.y) &&
           (point.z >= min.z && point.z <= max.z);
}

glm::vec3 AABB::getPosition() const
{
    return position;
}

glm::vec3 AABB::getSize() const
{
    return max - min;
}

glm::vec3 AABB::getMin() const
{
    return min;
}

glm::vec3 AABB::getMax() const
{
    return max; 
}

void AABB::setPosition(const glm::vec3 newPosition)
{
    glm::vec3 size = getSize();
    min = newPosition - size / 2.0F;
    max = newPosition + size / 2.0F;
    position = newPosition;
}

void AABB::setSize(const glm::vec3 newSize)
{
    glm::vec3 center = (min + max) / 2.0F;
    glm::vec3 halfSize = newSize / 2.0F;
    min = center - halfSize;
    max = center + halfSize;
}


void AABB::setMin(const glm::vec3 &min)
{
    this->min = min;
}

void AABB::setMax(const glm::vec3 &max) 
{
    this->max = max;
}
