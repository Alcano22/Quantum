#pragma once

#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Quantum
{
	struct Transform
	{
		glm::vec3 Position;
		glm::quat Rotation;
		glm::vec3 Scale;

		Transform(
			const glm::vec3& position	= glm::vec3(0.0f),
			const glm::quat& rotation	= glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			const glm::vec3& scale		= glm::vec3(1.0f)
		) : Position(position), Rotation(rotation), Scale(scale) {}

		Transform(
			const glm::vec3& position,
			const glm::vec3& rotation,
			const glm::vec3& scale		= glm::vec3(1.0f)
		) : Position(position), Rotation(glm::quat(glm::radians(rotation))), Scale(scale) {}

		void SetEulerAngles(const glm::vec3& eulerAngles) { Rotation = glm::quat(glm::radians(eulerAngles)); }
		glm::vec3 GetEulerAngles() const { return glm::degrees(glm::eulerAngles(Rotation)); }

		glm::mat4 GetModelMatrix() const
		{
			return glm::translate(glm::mat4(1.0f), Position) *
				glm::mat4_cast(Rotation) *
				glm::scale(glm::mat4(1.0f), Scale);
		}
	};
	
	class GameObject
	{
	public:
		GameObject(const Transform& transform)
			: Transform(transform) {}

		~GameObject();

	public:
		void Init();
		void Update(float dt);
		void Render();
		
	public:
		Transform Transform;

	private:
		std::vector<
	};
}
