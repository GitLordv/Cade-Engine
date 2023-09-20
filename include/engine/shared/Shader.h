#pragma once

#include "engine/shared/Utils.h"


class Shader
{
public:

	Shader();
	Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath);
	Shader(std::string vertexPath, std::string fragmentPath);
	//Shader(const GLchar* vertexCode, const GLchar* fragmentCode);

	void LoadShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath);
	void LoadShader(std::string vertexPath, std::string fragmentPath);
	void LoadShaderStr(const GLchar* vertexCode, const GLchar* fragmentCode);

	void use();
	void setInt(const std::string &name, GLint value) const;
	void setUint(const std::string &name, GLuint value) const;
	void setFloat(const std::string &name, GLfloat value) const;
	void setBool(const std::string &name, GLboolean value) const;
	void setMat2(const std::string &name, glm::mat2 &value) const;
	void setMat4(const std::string &name, glm::mat4 &value) const;
	void setVec2(const std::string &name, glm::vec2 &value) const;
	void setVec3(const std::string &name, glm::vec3 &value) const;
	void setVec4(const std::string &name, glm::vec4 &value) const;

private:

	void CheckCompileErrors(GLuint shader, const std::string &type);

	GLuint program;
};