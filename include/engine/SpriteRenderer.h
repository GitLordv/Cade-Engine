#pragma once

#include "engine/shared/Utils.h"
#include "engine/shared/Shader.h"
#include "engine/shared/Types.h"
#include "engine/Texture.h"
#include "engine/shared/UVRegion.h"


class SpriteRenderer
{
public:

	SpriteRenderer(const Shader &shader);
	~SpriteRenderer();

	void DrawSprite(Texture &texture, UVRegion &uvRegion, glm::vec3 position, glm::vec2 scrollSpeed,
		float rotation, glm::vec2 size, glm::vec4 tint);
	
private:

	Shader shader;
	GLuint quadVAO;

	void InitRenderData();

	
};
