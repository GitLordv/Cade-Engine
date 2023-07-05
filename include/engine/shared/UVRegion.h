#pragma once

#include "engine/shared/Utils.h"


class UVRegion
{
public:
    UVRegion();
    UVRegion(float uvMinX, float uvMinY, float uvMaxX, float uvMaxY);
    UVRegion(const glm::vec2 &uvMin, const glm::vec2 &uvMax);

    void setUV(float uvMinX, float uvMinY, float uvMaxX, float uvMaxY);
    void setUV(const glm::vec2 &uvMin, const glm::vec2 &uvMax);
  
    glm::vec2 getMin();
    glm::vec2 getMax();

    auto operator+(const glm::vec2 &offset) const;
    auto operator-(const glm::vec2 &offset) const;
    auto operator*(float scale) const;
    auto operator/(float scale) const;
    auto operator<=>(const UVRegion &other) const = default;

private:
    glm::vec2 m_uvMin;
    glm::vec2 m_uvMax;
};