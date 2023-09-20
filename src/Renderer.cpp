#include "engine/Renderer.h"


#include "engine/shared/Time.h"
#include "engine/shared/UVRect.h"
#include "engine/shared/Config.h"


Renderer::Renderer(const Shader &shader)
{
	this->shader = shader;
	InitRenderData();
};
Renderer::~Renderer()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &quadVAO);
}

void Renderer::DrawIcon(IconData &data, Camera &cam, Texture &atlas)
{
	auto model = glm::mat4(1.0F);
	model = glm::translate(model, data.position);

	glm::vec3 facing = glm::normalize(cam.getPosition() - data.position);
	glm::vec3 cameraRight = glm::cross(cam.getUp(), facing);
	glm::vec3 cameraUp = glm::cross(facing, cameraRight);
	glm::mat4 billboardRotation = glm::mat4(glm::vec4(cameraRight, 0.0F),
		glm::vec4(cameraUp, 0.0F),
		glm::vec4(facing, 0.0F),
		glm::vec4(0.0F, 0.0F, 0.0F, 1.0F));

	model *= billboardRotation;

	GLfloat desiredSizeOnScreen = 20.0F;
	GLfloat minScaleFactor = 0.5F;
	GLfloat distanceToIcon = glm::length(cam.getPosition() - data.position);
	GLfloat scaleFactor = std::clamp
	(
		(distanceToIcon * glm::tan(glm::radians(45.0F) * 0.5F)) * 2.0F
		/ desiredSizeOnScreen, minScaleFactor, std::numeric_limits<GLfloat>::max()
	);

	model = glm::scale(model, glm::vec3(scaleFactor));
	model = glm::translate(model, glm::vec3(-0.5F * data.size.x, -0.5F * data.size.y, 0.0F));
	model = glm::scale(model, glm::vec3(data.size, 0.0F));


	auto uv = glm::vec4(0.0F);
	switch (data.type)
	{
		case IconType::BOUNCE_TARGET: uv = UVRect(0.375F, 0.563F, 0.4375F, 0.625F).getAll();
			break;
		case IconType::TRIGGER: uv = UVRect(0.375F, 0.625F, 0.4375F, 0.687F).getAll();
			break;
		case IconType::TRIGGER_INACTIVE: uv = UVRect(0.4375F, 0.625F, 0.5F, 0.687F).getAll();
			break;
		case IconType::FRIZE: uv = UVRect().getDefault();
			break;
		case IconType::TWEEN: uv = UVRect().getDefault();
			break;
		default: uv = UVRect().getDefault();
			break;
	}

	shader.use();
	shader.setMat4("model", model);
	shader.setVec4("uv", uv);
	shader.setFloat("distanceToIcon", distanceToIcon);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindVertexArray(quadVAO);

	atlas.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	atlas.Unbind();

	glDepthMask(GL_TRUE);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::DrawSprite(LevelData &levelData, Camera &cam)
{
	//Z-Depth sort sprites in level
	std::stable_sort(levelData.sprites.begin(), levelData.sprites.end(),
		[](const SpriteData &a, const SpriteData &b) { return a.position.z >= b.position.z; });

	for (auto &sprite : levelData.sprites | std::views::reverse)
	{
		auto model = glm::mat4(1.0F);
		model = glm::translate(model, sprite.position);
		model = glm::rotate(model, glm::radians(sprite.rotation.x), glm::vec3(1.0F, 0.0F, 0.0F));
		model = glm::rotate(model, glm::radians(sprite.rotation.y), glm::vec3(0.0F, 1.0F, 0.0F));
		model = glm::rotate(model, glm::radians(sprite.rotation.z), glm::vec3(0.0F, 0.0F, 1.0F));
		model = glm::translate(model, glm::vec3(-0.5F * sprite.size.x, -0.5F * sprite.size.y, 0.0F));
		model = glm::scale(model, glm::vec3(sprite.size, 0.0F));

		auto uv = sprite.uvRect.getAll();

		if (sprite.isUvAsPixels)
		{
			uv.x /= sprite.texture->getWidth();
			uv.y /= sprite.texture->getHeight();
			uv.z /= sprite.texture->getWidth();
			uv.w /= sprite.texture->getHeight();
		}

		shader.use();
		shader.setMat4("model", model);
		shader.setVec4("tint", sprite.tint);
		shader.setVec4("uv", uv);
		shader.setVec2("scSpeed", sprite.scSpeed);
		shader.setFloat("camPos", cam.getPosition().z);
		shader.setFloat("depth", sprite.position.z);
		shader.setVec2("spriteSize", sprite.size);
		shader.setBool("isUseFog", sprite.useFog);

		//Addins
		shader.setBool("isAddin", sprite.addin.isAdditional);
		shader.setBool("isFade", sprite.addin.isFade);
		shader.setFloat("fadeEnd", sprite.addin.fadeEnd);

		auto UpdateAnimationEx = [&sprite](Shader &shader)
		{
			GLuint animType = 0;
			if (sprite.isAnim)
			{
				if (sprite.animation.type == "Wind") animType = 1;
				else if (sprite.animation.type == "Rotate")
				{
					animType = 2;
					sprite.rotation.z -= glm::radians(sprite.animation.speed);
				}
				else if (sprite.animation.type == "Flick")
				{
					animType = 3;
					GLfloat flickerSpeed = sprite.animation.speed;
					GLfloat alpha = glm::sin(static_cast<GLfloat>(Time::FixedTime()) * flickerSpeed);
					sprite.tint.a = glm::clamp(alpha, 0.0F, 1.0F);
				}
			}
			shader.setUint("animType", animType);
			shader.setBool("isAnim", sprite.isAnim);
		};
		UpdateAnimationEx(shader);

		if (!isWire)
		{
			if (sprite.isLight)
			{
				glDepthMask(GL_FALSE);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}
			else
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
		}
		else
		{
			glBlendFunc(GL_ZERO, GL_ZERO);
		}

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindVertexArray(quadVAO);

		sprite.texture->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		sprite.texture->Unbind();

		glDepthMask(GL_TRUE);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}


void Renderer::InitRenderData()
{
	constexpr GLfloat quadVertices[] =
	{
		//POS       //UV
		0.0F, 1.0F, 0.0F, 1.0F,
		1.0F, 0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 0.0F, 0.0F,

		0.0F, 1.0F, 0.0F, 1.0F,
		1.0F, 1.0F, 1.0F, 1.0F,
		1.0F, 0.0F, 1.0F, 0.0F
	};

	constexpr GLuint indices[] =
	{
		1, 2, 3,
		1, 3, 4
	};

	GLuint vbo, ibo;
	glCreateVertexArrays(1, &quadVAO);

	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(quadVertices), quadVertices, GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayVertexBuffer(quadVAO, 0, vbo, 0, 4 * sizeof(GLfloat));

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(quadVAO, ibo);

	glEnableVertexArrayAttrib(quadVAO, 0);
	glVertexArrayAttribFormat(quadVAO, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(quadVAO, 0, 0);
}


//Test
void Renderer::DrawAABB(const AABB &aabb)
{
	glm::vec3 min = aabb.getMin();
	glm::vec3 max = aabb.getMax();

	GLfloat vertices[] =
	{
		min.x, min.y, min.z,
		max.x, min.y, min.z,
		max.x, min.y, max.z,
		min.x, min.y, max.z,
		min.x, max.y, min.z,
		max.x, max.y, min.z,
		max.x, max.y, max.z,
		min.x, max.y, max.z,
	};

	GLuint indices[] =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7,
		0, 1, 1, 5, 5, 6, 6, 2,
		2, 3, 3, 7, 7, 4, 4, 0
	};

	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid *>(0));

	auto model = glm::mat4(1.0F);
	model = glm::translate(model, glm::vec3(0.0F));
	model = glm::scale(model, aabb.getSize());

	shader.use();
	shader.setMat4("model", model);

	glDrawElements(GL_LINES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

//void Renderer::DrawStrip(glm::vec3 position, Texture &texture, std::vector<GLfloat> points)
//{
//	GLuint VBO;
//	glGenBuffers(1, &VBO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(), points.data(), GL_STATIC_DRAW);
//
//	//XY
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//
//	//UV
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//
//
//	auto model = glm::mat4(1.0F);
//	model = glm::translate(model, position);
//
//	shader.use();
//	shader.setMat4("model", model);
//
//	glActiveTexture(GL_TEXTURE0 + 1);
//	texture.Bind();
//
//	auto numVertices = points.size() / 4;
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(numVertices));
//
//	
//
//	texture.Unbind();
//	glDeleteBuffers(1, &VBO);
//}


void Renderer::DrawStrip(glm::vec3 position, Texture &texture, std::vector<GLfloat> points)
{
	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(), points.data(), GL_STATIC_DRAW);

	// XY
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Generate indices
	std::vector<GLuint> indices;
	for (GLuint i = 0; i < points.size() / 4 - 2; i++)
	{
		if (i % 2 == 0)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}
		else
		{
			indices.push_back(i + 1);
			indices.push_back(i);
			indices.push_back(i + 2);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	auto model = glm::mat4(1.0F);
	model = glm::translate(model, position);

	shader.use();
	shader.setMat4("model", model);

	glActiveTexture(GL_TEXTURE0 + 1);
	texture.Bind();

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

	texture.Unbind();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
