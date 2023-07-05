#include "engine/Texture.h"


Texture::Texture() {}

Texture::Texture(const std::string &filePath)
{
	Generate(filePath);
}


void Texture::Generate(const std::string filePath)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	pixels = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	if (pixels == nullptr)
	{
		std::cerr << "Error: Texture loading Failed: " << filePath << std::endl;
		return;
	}

	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(id, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::GenerateModern(const std::string filePath)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	pixels = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	if (pixels == nullptr)
	{
		std::cerr << "Error: Texture (modern func) loading Failed: " << filePath << std::endl;
		return;
	}

	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(id, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glBindTextureUnit(0, id);
}


void Texture::LoadWithSOIL(const std::string &filePath)
{
	id = SOIL_load_OGL_texture
	(
		filePath.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS |
		SOIL_FLAG_INVERT_Y
	);
}


void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}


void Texture::unbind()
{
	glBindTextureUnit(0, id);
}


void Texture::getId()
{
	std::clog << "Log: Loaded texture ID: " << id << std::endl;
}


Texture::~Texture()
{
	glDeleteTextures(1, &id);
	SOIL_free_image_data(pixels);
}
