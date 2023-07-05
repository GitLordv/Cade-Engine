#pragma once

#include "engine/shared/Utils.h"
#include "SOIL2/SOIL2.h"


class Texture
{
public:
	Texture();
	Texture(const std::string &filePath);

	~Texture();

	//Load legacy  
	void Generate(const std::string filePath);

	//Load with DSA (modern opengl >= v4.5)
	void GenerateModern(const std::string filePath);

	void LoadWithSOIL(const std::string &filePath);

	void bind();
	void unbind();

	void getId();
	
private:

	GLuint id{0};
	GLubyte *pixels{nullptr};
	int width{0}, height{0};
};

