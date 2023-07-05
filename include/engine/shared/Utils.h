#pragma once

#define GLFW_INCLUDE_NONE

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLFW/glfw3.h"
#include "glbinding/gl/gl.h"
#include "glbinding/glbinding.h"
#include "globjects/globjects.h"
#include "glbinding/gl/extension.h"

//Std libs
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>


using namespace gl;
using uint = unsigned int;
namespace fs = std::filesystem;


namespace Custom
{
	static inline auto GetVendorInfo = []()
	{
		std::clog << "Log: OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	};


	static inline auto ConvertToOGLColor = [](GLuint r, GLuint g, GLuint b, GLuint a)
	{
		return glm::vec4
		(
			static_cast<GLfloat>(r) / 255.0f,
			static_cast<GLfloat>(g) / 255.0f,
			static_cast<GLfloat>(b) / 255.0f,
			static_cast<GLfloat>(a) / 255.0f
		);
	};


	static inline void glClearColorVec(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}


	static void glClearColorHex(const std::string &hexValue)
	{
		auto HexToRgb = [](GLfloat &r, GLfloat &g, GLfloat &b, const std::string& hexValue)
		{
			auto hex = std::stoul(hexValue, nullptr, 16);
			r = static_cast<GLfloat>((hex >> 16) & 0xFF) / 255.0f;
			g = static_cast<GLfloat>((hex >> 8)  & 0xFF) / 255.0f;
			b = static_cast<GLfloat>( hex        & 0xFF) / 255.0f;
		};

		GLfloat r, g, b;

		HexToRgb(r, g, b, hexValue);
		glClearColor(r, g, b, 1.0f);
	}

}


//GLOBAL
static GLFWwindow *window;
static inline const std::string &dataFolder = "Data";