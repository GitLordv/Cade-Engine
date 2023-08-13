#include "engine/Raycast.h"

Raycast::Raycast(const glm::vec3 &origin, const glm::vec3 &direction)
    : origin(origin), direction(glm::normalize(direction)) {}

std::optional<float> Raycast::IntersectAABB(const AABB &aabb) const
{
    glm::vec3 invDirection = 1.0F / direction;
    glm::vec3 tMin = (aabb.getMin() - origin) * invDirection;
    glm::vec3 tMax = (aabb.getMax() - origin) * invDirection;

    glm::vec3 tEntry = glm::min(tMin, tMax);
    glm::vec3 tExit = glm::max(tMin, tMax);

    float maxEntry = glm::max(tEntry.x, glm::max(tEntry.y, tEntry.z));
    float minExit = glm::min(tExit.x, glm::min(tExit.y, tExit.z));

    if (maxEntry > minExit || minExit < 0.0F)
    {
        return std::nullopt;
    }

    return maxEntry;
}