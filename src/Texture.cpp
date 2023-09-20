#include "engine/Texture.h"

Texture::Texture() : id(0), width(0), height(0), pixels(nullptr) {}

Texture::Texture(std::string_view path, std::string_view wrap)
{
	Generate(path, wrap);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
	SOIL_free_image_data(pixels);
}


void Texture::Generate(const std::string_view path, std::string_view wrap)
{
	pixels = SOIL_load_image(path.data(), &width, &height, 0, SOIL_LOAD_RGBA);
	if (pixels == nullptr)
	{
		std::cerr << "Info: Texture data loading failed: " << path.data() << std::endl;
		return;
	}

	auto wrapS = GL_REPEAT;
	auto wrapT = GL_REPEAT;

	if (!wrap.empty())
	{
		if (wrap == "CC")
		{
			wrapS = GL_CLAMP_TO_EDGE;
			wrapT = GL_CLAMP_TO_EDGE;
		}
		else if (wrap == "CR")
		{
			wrapS = GL_CLAMP_TO_EDGE;
			wrapT = GL_REPEAT;
		}
		else if (wrap == "RC")
		{
			wrapS = GL_REPEAT;
			wrapT = GL_CLAMP_TO_EDGE;
		}
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(id, GL_TEXTURE_WRAP_S, wrapS);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, wrapT);

	glTextureParameterf(id, GL_TEXTURE_MAX_ANISOTROPY, 4.0F);

	int mipLevels = 1 + (int)std::floor(std::log2(std::max(width, height)));

	glTextureStorage2D(id, mipLevels, GL_RGBA8, width, height);
	glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glGenerateTextureMipmap(id);

	std::cout << "\nID: " << id << " | " << width << "x" << height << " | " << path;
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTextureUnit(0, id);
}

GLid Texture::getId() const
{
	return id;
}

GLint Texture::getWidth() const
{
	return width;
}

GLint Texture::getHeight() const
{
	return height;
}
