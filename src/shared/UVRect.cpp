#include "engine/shared/UVRect.h"


UVRect::UVRect()
{
	setUV(0.0F, 0.0F, 1.0F, 1.0F);
}

UVRect::UVRect(float uvMinX, float uvMinY, float uvMaxX, float uvMaxY)
{
	setUV(uvMinX, uvMinY, uvMaxX, uvMaxY);
}

UVRect::UVRect(const glm::vec2 &uvMin, const glm::vec2 &uvMax)
{
	setUV(uvMin, uvMax);
}

void UVRect::setUV(float uvMinX, float uvMinY, float uvMaxX, float uvMaxY)
{
	m_uvMin = glm::vec2(uvMinX, uvMinY);
	m_uvMax = glm::vec2(uvMaxX, uvMaxY);
}

void UVRect::setUV(const glm::vec2 &uvMin, const glm::vec2 &uvMax)
{
	m_uvMin = uvMin;
	m_uvMax = uvMax;
}

glm::vec4 UVRect::getDefault() const
{
	return glm::vec4(0.0F, 0.0F, 1.0F, 1.0F);
}

glm::vec4 UVRect::getAll() const
{
	return glm::vec4(m_uvMin, m_uvMax);
}

glm::vec2 UVRect::getMin() const
{
	return m_uvMin;
}

glm::vec2 UVRect::getMax() const
{
	return m_uvMax;
}


auto UVRect::operator+(const glm::vec2 &offset) const
{
	return UVRect(m_uvMin + offset, m_uvMax + offset);
}

auto UVRect::operator-(const glm::vec2 &offset) const
{
	return UVRect(m_uvMin - offset, m_uvMax - offset);
}

auto UVRect::operator*(float scale) const
{
	return UVRect(m_uvMin * scale, m_uvMax * scale);
}

auto UVRect::operator/(float scale) const
{
	return UVRect(m_uvMin / scale, m_uvMax / scale);
}