﻿#pragma once

#include "Quantum/Rendering/Buffer.h"

namespace Quantum
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(const float* vertices, uint32_t size);
		
		~GLVertexBuffer() override;

	public:
		void Bind() const override;
		void Unbind() const override;
		bool IsBound() const override;

		const BufferLayout& GetLayout() const override { return m_Layout; }
		void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer(const uint32_t* indices, uint32_t count);

		~GLIndexBuffer() override;

	public:
		void Bind() const override;
		void Unbind() const override;
		bool IsBound() const override;

		uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}