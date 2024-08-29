#pragma once

#include "glad/glad.h"
#include "Quantum/Rendering/Buffer.h"
#include "Quantum/Rendering/VertexArray.h"

namespace Quantum
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		~OpenGLVertexArray() override = default;

	public:
		void Bind() const override;
		void Unbind() const override;
		bool IsBound() const override;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}