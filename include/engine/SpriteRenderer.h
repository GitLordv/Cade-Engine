#pragma once

#include "engine/shared/Utils.h"
#include "engine/Texture.h"
#include "engine/shared/Shader.h"
#include "engine/shared/Types.h"
#include "engine/shared/UVRegion.h"
#include <type_traits>


class SpriteRenderer
{
public:

	SpriteRenderer(const Shader &shader);
	~SpriteRenderer();


	void DrawSprite(Texture &texture, UVRegion &uvRegion, glm::vec3 position,
		glm::vec2 scrollSpeed, glm::vec3 rotation, glm::vec2 size, glm::vec4 tint);

	void DrawSprite(Texture &texture, SpriteData &data);
	void DrawSpriteInstanced(SpriteData &data);

	void setInstancesCount(const GLuint &count);
	
private:

	Shader shader;
	GLuint quadVAO;

	GLuint instancesCount;

	void InitRenderData();
	void InitRenderDataInstanced();

	
};
