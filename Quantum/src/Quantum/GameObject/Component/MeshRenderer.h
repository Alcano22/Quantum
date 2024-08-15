#pragma once

#include "Component.h"
#include "Quantum/Core/Base.h"
#include "Quantum/Rendering/Shader.h"

#include <glad/glad.h>

#include <vector>

namespace Quantum
{
	class SpriteRenderer : public Component
	{
	public:
		void Init() override;
		void Update(float dt) override;
		void Render() override;

	private:
		GLuint m_VAO = 0, m_VBO = 0, m_EBO = 0;
		
		std::vector<float> m_Vertices;
		std::vector<uint32_t> m_Indices;

		Ref<Shader> m_Shader;
	};
}