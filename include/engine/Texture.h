#pragma once

#include "engine/shared/Utils.h"
#include "SOIL2/SOIL2.h"


class Texture
{
public:
	Texture();
	Texture(const std::string &filePath);
	~Texture();

	void Generate(const std::string filePath);

	GLuint getId() const;

	void bind();
	void unbind();

	friend std::ostream &operator<<(std::ostream &os, const Texture &texture);

private:

	GLuint id;
	std::unique_ptr<GLubyte[]> pixels;
	int width;
	int height;
};

