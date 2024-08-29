#pragma once

#include "Quantum/Rendering/Shader.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace Quantum
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);

		~OpenGLShader() override;
	
	public:
		void Compile() override;
		
		void Bind() const override;
		void Unbind() const override;
		bool IsBound() const override;

		void SetUniformInt(const char* name, int value) override;
		void SetUniformInt2(const char* name, const glm::ivec2& value) override;
		void SetUniformInt3(const char* name, const glm::ivec3& value) override;
		void SetUniformInt4(const char* name, const glm::ivec4& value) override;
		void SetUniformFloat(const char* name, float value) override;
		void SetUniformFloat2(const char* name, const glm::vec2& value) override;
		void SetUniformFloat3(const char* name, const glm::vec3& value) override;
		void SetUniformFloat4(const char* name, const glm::vec4& value) override;
		void SetUniformMat2(const char* name, const glm::mat2& value) override;
		void SetUniformMat3(const char* name, const glm::mat3& value) override;
		void SetUniformMat4(const char* name, const glm::mat4& value) override;

		const std::string& GetName() const override { return m_Name; }
	private:
		void PreProcess(const std::string& src);
		std::string ReadFile(const std::string& filepath) const;

		int GetUniformLocation(const char* name) const;
		
	private:
		std::unordered_map<GLenum, std::string> m_Sources;

		std::string m_Name;
		uint32_t m_RendererID = 0;	
	};
}
