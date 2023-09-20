#pragma once

#include "engine/shared/Utils.h"
#include "SOIL2/SOIL2.h"

#include "cppfs/fs.h"
#include "cppfs/FilePath.h"

class Texture
{
public:
	Texture();
	Texture(std::string_view path, std::string_view wrap = "");
	~Texture();

	void Generate(std::string_view path, std::string_view wrap = "");

	void Bind();
	void Unbind();

	GLid getId() const;
	GLint getWidth() const;
	GLint getHeight() const;


private:

	GLid id;
	GLint width;
	GLint height;
	GLubyte* pixels;
};

