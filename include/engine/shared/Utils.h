#pragma once

#define GLFW_INCLUDE_NONE

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLFW/glfw3.h"
#include "glbinding/gl46core/gl.h"
#include "glbinding/glbinding.h"
#include "globjects/globjects.h"

#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <ranges>
#include <array>


using namespace gl;
using GLid = unsigned int;

namespace Custom
{
	static inline auto GetVendorInfo = []()
	{
		std::clog << "Log: OpenGL version: " << glGetString(GL_VERSION) << "\n";
		std::clog << "Log: OpenGL device: " << glGetString(GL_RENDERER) << "\n";
	};
	
	


	static inline auto ConvertToOGLColor = [](GLuint r, GLuint g, GLuint b, GLuint a)
	{
		return glm::vec4
		(
			static_cast<GLfloat>(r) / 255.0F,
			static_cast<GLfloat>(g) / 255.0F,
			static_cast<GLfloat>(b) / 255.0F,
			static_cast<GLfloat>(a) / 255.0F
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
			r = static_cast<GLfloat>((hex >> 16) & 0xFF) / 255.0F;
			g = static_cast<GLfloat>((hex >> 8)  & 0xFF) / 255.0F;
			b = static_cast<GLfloat>( hex        & 0xFF) / 255.0F;
		};

		GLfloat r, g, b;
		HexToRgb(r, g, b, hexValue);
		glClearColor(r, g, b, 1.0F);
	}



	static bool isLevelLoaded = false;
	static bool isDroppedFile = false;
	static inline std::string droppedFile;
	static inline void setDroppedFile(const char* path)
	{
		droppedFile = path;
	}

	static inline auto getDroppedFile()
	{
		return droppedFile;
	}

}
