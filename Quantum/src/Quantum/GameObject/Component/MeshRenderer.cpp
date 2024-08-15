#include "qtmpch.h"
#include "ShapeRenderer.h"

#include "Quantum/GameObject/GameObject.h"
#include "Quantum/GameObject/Shapes.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Quantum
{
	void SpriteRenderer::Init()
	{
		GenerateSphere(36, 18, m_Vertices, m_Indices);
		
		// VAO & VBO Init
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_DYNAMIC_DRAW);
		
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), m_Indices.data(), GL_DYNAMIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		m_Shader = CreateRef<Shader>("assets/shaders/normals.glsl");
		m_Shader->Compile();
	}

	void SpriteRenderer::Update(float dt)
	{
		
	}

	void SpriteRenderer::Render()
	{
		m_Shader->Bind();
		
		const glm::mat4 model = m_Transform->GetModelMatrix();
		const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
		const glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		
		m_Shader->SetMat4("u_Model", model);
		m_Shader->SetMat4("u_View", view);
		m_Shader->SetMat4("u_Projection", projection);
		
		m_Shader->SetFloat("u_Time", 1.0f);
		
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		
		m_Shader->Unbind();
	}
}
