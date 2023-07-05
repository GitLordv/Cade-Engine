#include "engine/shared/UVRegion.h"


UVRegion::UVRegion()
{
    setUV(0.0f, 0.0f, 1.0f, 1.0f);
}

UVRegion::UVRegion(float uvMinX, float uvMinY, float uvMaxX, float uvMaxY) 
{
    setUV(uvMinX, uvMinY, uvMaxX, uvMaxY);
}

UVRegion::UVRegion(const glm::vec2 &uvMin, const glm::vec2 &uvMax) 
{
    setUV(uvMin, uvMax);
}

void UVRegion::setUV(float uvMinX, float uvMinY, float uvMaxX, float uvMaxY)
{
    m_uvMin = glm::vec2(uvMinX, uvMinY);
    m_uvMax = glm::vec2(uvMaxX, uvMaxY);
}

void UVRegion::setUV(const glm::vec2 &uvMin, const glm::vec2 &uvMax)
{
    m_uvMin = uvMin;
    m_uvMax = uvMax;
}

glm::vec2 UVRegion::getMin()
{
    return m_uvMin;
}

glm::vec2 UVRegion::getMax()
{
    return m_uvMax;
}


auto UVRegion::operator+(const glm::vec2 &offset) const
{
    return UVRegion(m_uvMin + offset, m_uvMax + offset);
}

auto UVRegion::operator-(const glm::vec2 &offset) const
{
    return UVRegion(m_uvMin - offset, m_uvMax - offset);
}

auto UVRegion::operator*(float scale) const
{
    return UVRegion(m_uvMin * scale, m_uvMax * scale);
}

auto UVRegion::operator/(float scale) const
{
    return UVRegion(m_uvMin / scale, m_uvMax / scale);
}