#pragma once

#include "engine/shared/Utils.h"


struct Vertex
{
	GLfloat x, y, z;

	Vertex(GLfloat x, GLfloat y, GLfloat z = 0.0f)
		: x(x), y(y), z(z) {}
};


struct Color
{
	GLfloat r, g, b, a;

	Color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0f)
		: r(red), g(green), b(blue), a(alpha) {}
};


struct UV
{
	GLfloat u, v;

	UV(GLfloat u, GLfloat v)
		: u(u), v(v) {}
};


struct LevelTexture
{
	std::string name;
	std::string path;
};


struct SpriteData
{
	std::string name;
	std::string texture;
	glm::vec3 position;
	glm::vec2 scrollSpeed;
	float angle;
	glm::vec2 size;
	glm::vec4 tint;
};

