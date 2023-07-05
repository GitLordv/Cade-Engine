#include "engine\SpriteRenderer.h"



SpriteRenderer::SpriteRenderer(const Shader &shader)
{
	this->shader = shader;
	this->InitRenderData();
};

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}


void SpriteRenderer::DrawSprite(Texture &texture, UVRegion &uvRegion, glm::vec3 position = glm::vec3(0.0f), glm::vec2 scrollSpeed = glm::vec2(0.0f),
	float rotation = 0.0f, glm::vec2 size = glm::vec2(1.0f), glm::vec4 tint = glm::vec4(1.0f))
{
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, position.z));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, position.z));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	static auto m_tint = tint;
	//ImGui::Begin("Sprite options", nullptr);
	//ImGui::ColorEdit4("Tint", &m_tint[4], ImGuiColorEditFlags_AlphaBar);
	//ImGui::End();

	auto uvMin = uvRegion.getMin();
	auto uvMax = uvRegion.getMax();
	
	this->shader.use();
	this->shader.setMat4("model", model);
	this->shader.setVec4("spriteTint", tint);
	this->shader.setVec2("scrollSpeed", scrollSpeed);
	this->shader.setVec2("uvMin", uvMin);
	this->shader.setVec2("uvMax", uvMax);


	glActiveTexture(GL_TEXTURE0 + 1);

	texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	texture.unbind();
}


void SpriteRenderer::InitRenderData()
{
	GLuint vbo;
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

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<void*>(0));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}