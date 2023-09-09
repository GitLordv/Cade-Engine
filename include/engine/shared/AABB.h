#pragma once

#include "glm/glm.hpp"


class AABB 
{
public:

    AABB();
    AABB(const glm::vec3 &min, const glm::vec3 &max);

    glm::vec3 getPosition() const;
    glm::vec3 getSize() const;
    glm::vec3 getMin() const;
    glm::vec3 getMax() const;

    void setPosition(const glm::vec3 newPosition);
    void setSize(glm::vec3 newSize);
    void setMin(const glm::vec3 &min);
    void setMax(const glm::vec3 &max);

    bool IntersectAABB(const AABB &another) const;
    bool IntersectPoint(const glm::vec2 &point) const;
    bool IntersectPoint(const glm::vec3 &point) const;

private:

    glm::vec3 position;
    glm::vec3 min;
    glm::vec3 max;
};