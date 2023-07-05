#include "engine/shared/Utils.h"


class BufferObject
{
public:

	enum class Type 
	{
		VBO = static_cast<int>(GL_ARRAY_BUFFER),
		IBO = static_cast<int>(GL_ELEMENT_ARRAY_BUFFER),
		UBO = static_cast<int>(GL_UNIFORM_BUFFER),
		SSBO = static_cast<int>(GL_SHADER_STORAGE_BUFFER)
	};


	BufferObject(Type type) : m_type(type)
	{
		glGenBuffers(1, &m_id);
	}

	~BufferObject()
	{
		glDeleteBuffers(1, &m_id);
	}

	void Bind() const
	{
		glBindBuffer(static_cast<GLenum>(m_type), m_id);
	}

	void UnBind() const 
	{
		glBindBuffer(static_cast<GLenum>(m_type), 0);
	}

	void SetData(const void *data, size_t size, GLenum usage)
	{
		glBufferData(static_cast<GLenum>(m_type), size, data, usage);
	}

private:
	GLuint m_id;
	Type m_type;
};
