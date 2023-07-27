#pragma once

#include "glm/glm.hpp"


class AABB 
{
public:

    AABB();
    AABB(const glm::vec3 &min, const glm::vec3 &max);

    glm::vec3 getMin() const;
    glm::vec3 getMax() const;

    void setMin(const glm::vec3 &min);
    void setMax(const glm::vec3 &max);

    bool intersects(const AABB &other) const;

private:

    glm::vec3 min;
    glm::vec3 max;
};