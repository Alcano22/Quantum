#include "GameObject.h"

#include "Shapes.h"

#include "Quantum/Core/Assertion.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Quantum
{
	GameObject::~GameObject()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	void GameObject::Init()
	{
		GenerateSphere(36, 18, m_SphereVertices, m_SphereIndices);

		// VAO & VBO Init
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_SphereVertices.size() * sizeof(float), m_SphereVertices.data(), GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_SphereIndices.size() * sizeof(uint32_t), m_SphereIndices.data(), GL_DYNAMIC_DRAW);

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

	void GameObject::Update(const float dt)
	{
	}
		
	void GameObject::Input(const int key, const int action)
	{
		
	}

	void GameObject::Render()
	{
		m_Shader->Bind();

		const glm::mat4 model = Transform.GetModelMatrix();
		const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
		const glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		m_Shader->SetMat4("u_Model", model);
		m_Shader->SetMat4("u_View", view);
		m_Shader->SetMat4("u_Projection", projection);

		m_Shader->SetFloat("u_Time", 1.0f);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_SphereIndices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		
		m_Shader->Unbind();
	}

	void GameObject::UpdateSphereBuffers()
	{
		GenerateSphere(m_NumSphereSectors, m_NumSphereStacks, m_SphereVertices, m_SphereIndices);

		glBindVertexArray(m_VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_SphereVertices.size() * sizeof(float), m_SphereVertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_SphereIndices.size() * sizeof(uint32_t), m_SphereIndices.data());

		glBindVertexArray(0);
	}
}
