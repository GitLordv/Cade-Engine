#include "engine/shared/AABB.h"

AABB::AABB() : min(glm::vec3(0.0F)), max(glm::vec3(1.0F)) {}

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max) {}

bool AABB::IntersectAABB(const AABB &another) const
{
    if (max.x < another.min.x || min.x > another.max.x)
    {
        return false;
    }
    if (max.y < another.min.y || min.y > another.max.y)
    {
        return false;
    }
    if (max.z < another.min.z || min.z > another.max.z)
    {
        return false;
    }
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
