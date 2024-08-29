#pragma once

#include "Quantum/Rendering/Framebuffer.h"

namespace Quantum
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		explicit OpenGLFramebuffer(const FramebufferSpecs& specs);
		~OpenGLFramebuffer() override;

		void Bind() override;
		void Unbind() override;
		bool IsBound() const override;

		void Resize(uint32_t width, uint32_t height) override;

		uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	private:
		void Invalidate();
		
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
	};
}