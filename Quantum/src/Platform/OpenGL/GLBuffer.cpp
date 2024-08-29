#include "qtmpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Quantum
{
#pragma region VertexBuffer

	GLVertexBuffer::GLVertexBuffer(const float* vertices, const uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void GLVertexBuffer::Bind() const
	{
		if (IsBound()) return;
		
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void GLVertexBuffer::Unbind() const
	{
		if (!IsBound()) return;
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bool GLVertexBuffer::IsBound() const
	{
		int currentVertexBuffer;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVertexBuffer);
		return (uint32_t)currentVertexBuffer == m_RendererID;
	}
	
#pragma endregion

#pragma region IndexBuffer

	GLIndexBuffer::GLIndexBuffer(const uint32_t* indices, const uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void GLIndexBuffer::Bind() const
	{
		if (IsBound()) return;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void GLIndexBuffer::Unbind() const
	{
		if (!IsBound()) return;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	bool GLIndexBuffer::IsBound() const
	{
		int currentIndexBuffer;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER, &currentIndexBuffer);
		return (uint32_t)currentIndexBuffer == m_RendererID;
	}

#pragma endregion
}
