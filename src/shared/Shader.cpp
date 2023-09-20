#include "engine/shared/Shader.h"


Shader::Shader() : program(0) {}

Shader::Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath)
{
	LoadShader(vertexPath, fragmentPath, geometryPath);
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
	LoadShader(vertexPath, fragmentPath);
}

//Shader::Shader(const GLchar* vertexCode, const GLchar* fragmentCode)
//{
//	LoadShaderStr(vertexCode, fragmentCode);
//}

void Shader::LoadShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	//Ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath.c_str());
		fShaderFile.open(fragmentPath.c_str());
		gShaderFile.open(geometryPath.c_str());
		std::stringstream vShaderStream, fShaderStream, gShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "Error: Failed shader/s loaded\n";
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();

	//Vertex shader
	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vShaderCode, nullptr);
	glCompileShader(VertexShader);
	CheckCompileErrors(VertexShader, "VERTEX");

	//Pixel shader
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fShaderCode, nullptr);
	glCompileShader(FragmentShader);
	CheckCompileErrors(FragmentShader, "FRAGMENT");

	//Geometry shader
	GLuint GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(GeometryShader, 1, &gShaderCode, nullptr);
	glCompileShader(GeometryShader);
	CheckCompileErrors(GeometryShader, "GEOMETRY");

	//Shader Program
	program = glCreateProgram();
	glAttachShader(program, VertexShader);
	glAttachShader(program, FragmentShader);
	glAttachShader(program, GeometryShader);
	glLinkProgram(program);
	CheckCompileErrors(program, "PROGRAM");

	//Delete
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	glDeleteShader(GeometryShader);
}

void Shader::LoadShader(std::string vertexPath, std::string fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//Ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath.c_str());
		fShaderFile.open(fragmentPath.c_str());
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "Error: Failed shader/s loaded\n";
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();

	//Vertex shader
	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vShaderCode, nullptr);
	glCompileShader(VertexShader);
	CheckCompileErrors(VertexShader, "VERTEX");

	//Pixel shader
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fShaderCode, nullptr);
	glCompileShader(FragmentShader);
	CheckCompileErrors(FragmentShader, "FRAGMENT");

	//Shader Program
	program = glCreateProgram();
	glAttachShader(program, VertexShader);
	glAttachShader(program, FragmentShader);
	glLinkProgram(program);
	CheckCompileErrors(program, "PROGRAM");

	//Delete
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

//void Shader::LoadShaderStr(const GLchar* vertexCode, const GLchar* fragmentCode)
//{
//	//Vertex shader
//	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(VertexShader, 1, &vertexCode, nullptr);
//	glCompileShader(VertexShader);
//	CheckCompileErrors(VertexShader, "VERTEX");
//
//	//Pixel shader
//	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(FragmentShader, 1, &fragmentCode, nullptr);
//	glCompileShader(FragmentShader);
//	CheckCompileErrors(FragmentShader, "FRAGMENT");
//
//	//Shader Program
//	program = glCreateProgram();
//	glAttachShader(program, VertexShader);
//	glAttachShader(program, FragmentShader);
//	glLinkProgram(program);
//	CheckCompileErrors(program, "PROGRAM");
//
//	//Delete
//	glDeleteShader(VertexShader);
//	glDeleteShader(FragmentShader);
//}

void Shader::use()
{
	glUseProgram(program);
}

void Shader::setInt(const std::string &name, GLint value) const
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
};

void Shader::setUint(const std::string &name, GLuint value) const
{
	glUniform1ui(glGetUniformLocation(program, name.c_str()), value);
};

void Shader::setFloat(const std::string &name, GLfloat value) const
{
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
};

void Shader::setBool(const std::string &name, GLboolean value) const
{
	glUniform1ui(glGetUniformLocation(program, name.c_str()), static_cast<int>(value));
};

void Shader::setMat2(const std::string &name, glm::mat2 &value) const
{
	glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
};

void Shader::setMat4(const std::string &name, glm::mat4 &value) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
};

void Shader::setVec2(const std::string &name, glm::vec2 &value) const
{
	glUniform2f(glGetUniformLocation(program, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string &name, glm::vec3 &value) const
{
	glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string &name, glm::vec4 &value) const
{
	glUniform4f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::CheckCompileErrors(GLuint shader, const std::string &type)
{
	GLint success;
	GLchar infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "\nCompile error: Shader type -> " << type << "\n" << infoLog
				<< "\n----------------------------------------------------------------" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "\nLinking error: Program type -> " << type << "\n" << infoLog
				<< "\n-----------------------------------------------------------------" << std::endl;
		}
	}
}