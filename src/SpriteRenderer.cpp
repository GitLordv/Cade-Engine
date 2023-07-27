#include "engine/SpriteRenderer.h"



SpriteRenderer::SpriteRenderer(const Shader &shader)
{
	this->shader = shader;
	//this->InitRenderData();
	this->InitRenderDataInstanced();
};

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &quadVAO);
}

void SpriteRenderer::DrawSprite(Texture &texture, UVRegion &uvRegion, glm::vec3 position = glm::vec3(0.0f), glm::vec2 scrollSpeed = glm::vec2(0.0f),
	glm::vec3 rotation = glm::vec3(0.0f), glm::vec2 size = glm::vec2(1.0f), glm::vec4 tint = glm::vec4(1.0f))
{
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::translate(model, position);
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(size, 0.0f));

	static auto m_tint = tint;
	//ImGui::Begin("Sprite options", nullptr);
	//ImGui::ColorEdit4("Tint", &m_tint[4], ImGuiColorEditFlags_AlphaBar);
	//ImGui::End();

	auto uvMin = uvRegion.getMin();
	auto uvMax = uvRegion.getMax();

	this->shader.use();
	this->shader.setMat4("model", model);
	this->shader.setVec4("tint", tint);
	this->shader.setVec2("scrollSpeed", scrollSpeed);
	this->shader.setVec2("uvMin", uvMin);
	this->shader.setVec2("uvMax", uvMax);


	glActiveTexture(GL_TEXTURE0 + 1);

	texture.bind();

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	texture.unbind();
}

void SpriteRenderer::DrawSprite(Texture &texture, SpriteData &data)
{
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f * data.size.x, -0.5f * data.size.y, 0.0f));
	model = glm::translate(model, data.position);
	model = glm::translate(model, glm::vec3(0.5f * data.size.x, 0.5f * data.size.y, 0.0f));
	model = glm::rotate(model, glm::radians(data.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(data.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(data.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * data.size.x, -0.5f * data.size.y, 0.0f));
	model = glm::scale(model, glm::vec3(data.size, 0.0f));



	static auto m_tint = data.tint;
	//ImGui::Begin("Sprite options", nullptr);
	//ImGui::ColorEdit4("Tint", &m_tint[4], ImGuiColorEditFlags_AlphaBar);
	//ImGui::End();

	//auto uvMin = data.uvRegion.getMin();
	//auto uvMax = data.uvRegion.getMax();
	auto a = glm::vec2(0.0f, 0.0f);
	auto b = glm::vec2(1.0f, 1.0f);

	this->shader.use();
	this->shader.setMat4("model", model);
	this->shader.setVec4("tint", data.tint);
	this->shader.setVec2("scrollSpeed", data.scSpeed);
	this->shader.setVec2("uvMin", a);
	this->shader.setVec2("uvMax", b);


	glActiveTexture(GL_TEXTURE0 + 1);

	texture.bind();

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	texture.unbind();
}

void SpriteRenderer::DrawSpriteInstanced(SpriteData &data)
{
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f * data.size.x, -0.5f * data.size.y, 0.0f));
	model = glm::translate(model, data.position);
	model = glm::translate(model, glm::vec3(0.5f * data.size.x, 0.5f * data.size.y, 0.0f));
	model = glm::rotate(model, glm::radians(data.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(data.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(data.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * data.size.x, -0.5f * data.size.y, 0.0f));
	model = glm::scale(model, glm::vec3(data.size, 0.0f));

	static auto m_tint = data.tint;
	//ImGui::Begin("Sprite options", nullptr);
	//ImGui::ColorEdit4("Tint", &m_tint[4], ImGuiColorEditFlags_AlphaBar);
	//ImGui::End();

	//Concept:
	//SpriteOptionsUI(tint, F_RECT_PIKER or F_TRI_PICKER, F_USE_ALPHA);

	auto uvMin = data.uvRegion.getMin();
	auto uvMax = data.uvRegion.getMax();

	shader.use();
	shader.setMat4("model", model);
	shader.setVec4("tint", data.tint);
	shader.setVec2("scrollSpeed", data.scSpeed);
	shader.setVec2("uvMin", uvMin);
	shader.setVec2("uvMax", uvMax);

	glActiveTexture(GL_TEXTURE0 + 1);

	data.texture->bind();

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//Temp disable: glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instancesCount);

	//Clear
	glBindVertexArray(0);

	data.texture->unbind();
}

void SpriteRenderer::InitRenderData()
{
	constexpr GLfloat quadVertices[] =
	{
		//POS       //UV
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	GLuint vbo;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glBindVertexArray(quadVAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<void *>(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::InitRenderDataInstanced()
{
	constexpr GLfloat quadVertices[] =
	{
		//POS       //UV
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	constexpr GLuint indices[] =
	{
		1, 2, 3,  // Первый треугольник
		1, 3, 4   // Второй треугольник
	};

	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(quadVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	//glVertexAttribDivisor(0, 1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Using
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<void *>(0));
	glEnableVertexAttribArray(0);

	//Clear
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::setInstancesCount(const GLuint &count)
{
	instancesCount = count;
}
