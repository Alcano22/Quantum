#include "qtmpch.h"
#include "OpenGLVertexArray.h"

#include <magic_enum.hpp>

namespace Quantum
{
	static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:	    return GL_FLOAT;
		case ShaderDataType::Mat4:	    return GL_FLOAT;
		default: break;
		}

		QTM_ERROR("Unknown ShaderDataType '{}'", magic_enum::enum_name(type));
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		if (IsBound()) return;
		
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		if (!IsBound()) return;

		glBindVertexArray(0);
	}

	bool OpenGLVertexArray::IsBound() const
	{
		int currentVertexArray;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVertexArray);
		return currentVertexArray == m_RendererID;
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		Bind();
		vertexBuffer->Bind();

		QTM_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "VertexBuffer has no layout!")

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}