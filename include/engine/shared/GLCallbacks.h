#pragma once

#include "engine/shared/Utils.h"

#include <unordered_set>


namespace GLCallbacks
{
	static inline std::unordered_set<std::string> uniqueMessages;
	static inline void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message, void const *user_param)
	{
		std::string messageStr(message);

		if (uniqueMessages.find(messageStr) == uniqueMessages.end())
		{
			const auto srcStr = [source]()
			{
				switch (source)
				{
					case GL_DEBUG_SOURCE_API:			    return "API";
					case GL_DEBUG_SOURCE_WINDOW_SYSTEM:	    return "WINDOW SYSTEM";
					case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "SHADER COMPILER";
					case GL_DEBUG_SOURCE_THIRD_PARTY:	    return "THIRD PARTY";
					case GL_DEBUG_SOURCE_APPLICATION:	    return "APPLICATION";
					case GL_DEBUG_SOURCE_OTHER:			    return "OTHER";
					default:								return "UNKNOWN";
				}
			}();

			const auto typeStr = [type]()
			{
				switch (type)
				{
					case GL_DEBUG_TYPE_ERROR:				return "ERROR";
					case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
					case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
					case GL_DEBUG_TYPE_PORTABILITY:			return "PORTABILITY";
					case GL_DEBUG_TYPE_PERFORMANCE:			return "PERFORMANCE";
					case GL_DEBUG_TYPE_MARKER:				return "MARKER";
					case GL_DEBUG_TYPE_OTHER:				return "OTHER";
					default:								return "UNKNOWN";
				}
			}();

			const auto severityStr = [severity]()
			{
				switch (severity)
				{
					case GL_DEBUG_SEVERITY_NOTIFICATION:    return "NOTIFICATION";
					case GL_DEBUG_SEVERITY_LOW:				return "LOW";
					case GL_DEBUG_SEVERITY_MEDIUM:			return "MEDIUM";
					case GL_DEBUG_SEVERITY_HIGH:			return "HIGH";
					default:								return "UNKNOWN";
				}
			}();
			
			std::cout << "[" << srcStr << "][" << typeStr << "][" << severityStr << "]->id[" << id << "]->msg: " << message << std::endl;
			
			uniqueMessages.insert(messageStr);
		}
	}
};