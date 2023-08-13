#include "engine/Renderer.h"
#include "engine/shared/Time.h"
#include "engine/shared/UVRect.h"
#include "engine/Texture.h"
#include "engine/shared/Config.h"
#include <type_traits>


Renderer::Renderer(const Shader &shader)
{
	this->shader = shader;
	this->InitRenderData();
};
Renderer::~Renderer()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &quadVAO);
}

void Renderer::DrawIcon(Icon &iconData, Camera &cam, Texture &atlas)
{
	auto model = glm::mat4(1.0F);
	model = glm::translate(model, iconData.position);

	glm::vec3 facing = glm::normalize(cam.getPosition() - iconData.position);
	glm::vec3 cameraRight = glm::cross(cam.getUp(), facing);
	glm::vec3 cameraUp = glm::cross(facing, cameraRight);
	glm::mat4 billboardRotation = glm::mat4(glm::vec4(cameraRight, 0.0F),
		glm::vec4(cameraUp, 0.0F),
		glm::vec4(facing, 0.0F),
		glm::vec4(0.0F, 0.0F, 0.0F, 1.0F));

	model *= billboardRotation;

	GLfloat desiredSizeOnScreen = 20.0F;
	GLfloat minScaleFactor = 0.5F;
	GLfloat distanceToIcon = glm::length(cam.getPosition() - iconData.position);
	GLfloat scaleFactor = std::clamp
	(
		(distanceToIcon * glm::tan(glm::radians(45.0F) * 0.5F)) * 2.0F 
		/ desiredSizeOnScreen, minScaleFactor, std::numeric_limits<GLfloat>::max()
	);

	model = glm::scale(model, glm::vec3(scaleFactor));
	model = glm::translate(model, glm::vec3(-0.5F * iconData.size.x, -0.5F * iconData.size.y, 0.0F));
	model = glm::scale(model, glm::vec3(iconData.size, 0.0F));


	auto uv = glm::vec4(0.0F);
	switch (iconData.type)
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

void Renderer::DrawSprite(Level::Sprite &spriteData, Camera &cam)
{
	auto model = glm::mat4(1.0F);
	model = glm::translate(model, spriteData.position);
	model = glm::rotate(model, glm::radians(spriteData.rotation.x), glm::vec3(1.0F, 0.0F, 0.0F));
	model = glm::rotate(model, glm::radians(spriteData.rotation.y), glm::vec3(0.0F, 1.0F, 0.0F));
	model = glm::rotate(model, glm::radians(spriteData.rotation.z), glm::vec3(0.0F, 0.0F, 1.0F));
	model = glm::translate(model, glm::vec3(-0.5F * spriteData.size.x, -0.5F * spriteData.size.y, 0.0F));
	model = glm::scale(model, glm::vec3(spriteData.size, 0.0F));

	auto uv = spriteData.uvRect.getAll();

	shader.use();
	shader.setMat4("model", model);
	shader.setVec4("tint", spriteData.tint);
	shader.setVec4("uv", uv);
	shader.setVec2("scSpeed", spriteData.scSpeed);
	shader.setFloat("camPos", cam.getPosition().z);
	shader.setFloat("depth", spriteData.position.z);
	shader.setVec2("spriteSize", spriteData.size);
	shader.setBool("isUseFog", spriteData.useFog);

	//Addins
	shader.setBool("isAddin", spriteData.addin.isAdditional);
	shader.setBool("isFade", spriteData.addin.isFade);
	shader.setFloat("fadeEnd", spriteData.addin.fadeEnd);

	auto UpdateAnimationEx = [&spriteData](Shader &shader)
	{
		//For animation
		GLuint animType = 0;
		if (spriteData.animation.type == "Wind") animType = 1;
		else if (spriteData.animation.type == "Rotate") animType = 2;
		else if (spriteData.animation.type == "Flick") animType = 3;
		else { spriteData.isAnim = false; }

		if (spriteData.isAnim && animType == 1)
		{
		}
		else if (spriteData.isAnim && animType == 2)
		{
			spriteData.rotation.z -= glm::radians(spriteData.animation.speed);
		}
		else if (spriteData.isAnim && animType == 3)
		{
			GLfloat flickerSpeed = spriteData.animation.speed;
			GLfloat alpha = glm::sin(static_cast<GLfloat>(Time::FixedTime()) * flickerSpeed);
			spriteData.tint.a = glm::clamp(alpha, 0.0F, 1.0F);
		}
		shader.setUint("animType", animType);
		shader.setBool("isAnim", spriteData.isAnim);
	};
	UpdateAnimationEx(shader);

	if (!isWire)
	{
		if (spriteData.isLight)
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

	spriteData.texture->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	spriteData.texture->Unbind();

	glDepthMask(GL_TRUE);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
		1, 2, 3,  // Первый треугольник
		1, 3, 4   // Второй треугольник
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
