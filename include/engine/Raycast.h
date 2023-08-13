#include "engine/shared/AABB.h"

#include "glm/vec3.hpp"
#include <optional>


class Raycast
{
public:
    Raycast(const glm::vec3 &origin, const glm::vec3 &direction);

    std::optional<float> IntersectAABB(const AABB &aabb) const;

private:
    glm::vec3 origin;
    glm::vec3 direction;
};
      