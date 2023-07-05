#pragma once

#include "engine/shared/Utils.h"


class Shader
{
public:
	
	Shader();
	Shader(std::string vertexPath, std::string fragmentPath);

	void LoadShader(std::string vertexPath, std::string fragmentPath);

	void use();
	void setInt(const std::string &name, GLint value) const;
	void setFloat(const std::string &name, GLfloat value) const;
	void setBool(const std::string &name, GLboolean value) const;
	void setMat4(const std::string &name, glm::mat4 &value) const;
	void setVec2(const std::string &name, glm::vec2 &value) const;
	void setVec3(const std::string &name, glm::vec3 &value) const;
	void setVec4(const std::string &name, glm::vec4 &value) const;


private:

	GLuint program;

	void CheckCompileErrors(GLuint shader, const std::string &type);
};