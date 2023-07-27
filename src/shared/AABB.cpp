#include "engine/shared/AABB.h"

AABB::AABB() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {}

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max) {}


glm::vec3 AABB::getMin() const
{
    return min;
}

glm::vec3 AABB::getMax() const
{
    return max; 
}

void AABB::setMin(const glm::vec3 &min)
{
    this->min = min;
}

void AABB::setMax(const glm::vec3 &max) 
{
    this->max = max;
}

bool AABB::intersects(const AABB &other) const
{
    if (max.x < other.min.x || min.x > other.max.x) 
    {
        return false;
    }
    if (max.y < other.min.y || min.y > other.max.y)
    {
        return false;
    }
    if (max.z < other.min.z || min.z > other.max.z)
    {
        return false;
    }
    return true;
}
