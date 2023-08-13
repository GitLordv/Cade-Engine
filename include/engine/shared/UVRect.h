#pragma once

#include "engine/shared/Utils.h"


class UVRect
{
public:
    UVRect();
    UVRect(float uvMinX, float uvMinY, float uvMaxX, float uvMaxY);
    UVRect(const glm::vec2 &uvMin, const glm::vec2 &uvMax);

    void setUV(float uvMinX, float uvMinY, float uvMaxX, float uvMaxY);
    void setUV(const glm::vec2 &uvMin, const glm::vec2 &uvMax);
  
    glm::vec4 getDefault() const;
    glm::vec4 getAll() const;
    glm::vec2 getMin() const;
    glm::vec2 getMax() const;

    auto operator+(const glm::vec2 &offset) const;
    auto operator-(const glm::vec2 &offset) const;
    auto operator*(float scale) const;
    auto operator/(float scale) const;
    auto operator<=>(const UVRect &other) const = default;

private:
    glm::vec2 m_uvMin;
    glm::vec2 m_uvMax;
};