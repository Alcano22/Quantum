#pragma once

#include "Layer.h"

#include "Quantum/Scene/Scene.h"
#include "Quantum/GameObject/GameObject.h"
#include "Quantum/Rendering/Camera.h"
#include "Quantum/Rendering/Material.h"
#include "Quantum/Rendering/Framebuffer.h"

namespace Quantum
{
	class GameLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;

		void OnWindowResized(uint32_t width, uint32_t height);

		void Update(float dt) override;
		void Render() override;
		void RenderImGui() override;

	private:
		Ref<Scene> m_Scene;
		Ref<Camera> m_Camera;
		Ref<Material> m_Material;
		Ref<Framebuffer> m_Framebuffer;
	};
}