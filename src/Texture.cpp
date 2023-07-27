#include "engine/Texture.h"


Texture::Texture() : id(0), width(0), height(0), pixels(nullptr) {}

Texture::Texture(const std::string &filePath)
{
	Generate(filePath);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
	SOIL_free_image_data(pixels.get());
}

void Texture::Generate(const std::string filePath)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	pixels.reset(SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA));
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
	glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.get());

	glBindTextureUnit(0, id);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind()
{
	glBindTextureUnit(0, id);
}

GLuint Texture::getId() const
{
	return id;
}

std::ostream &operator<<(std::ostream &os, const Texture &texture)
{
	return os << "\nTexture ID: " << texture.id
		<< "Texture Widht: " << texture.width
		<< "Texture Height: " << texture.height
		<< "\n";
}