#include "qtmpch.h"
#include "GameLayer.h"

#include "Quantum/Core/Engine.h"
#include "Quantum/Event/Event.h"
#include "Quantum/Event/WindowEvent.h"
#include "Quantum/GameObject/Component/MeshRenderer.h"
#include "Quantum/Rendering/Renderer.h"

#include <imgui.h>

namespace Quantum
{
	void GameLayer::OnAttach()
	{
		FramebufferSpecs fbSpecs;
		fbSpecs.Width = Engine::GetWindow().GetProps().Width;
		fbSpecs.Height = Engine::GetWindow().GetProps().Height;

		m_Framebuffer = Framebuffer::Create(fbSpecs);
		
		EventManager::AddCallback<WindowResizedEvent>([this](const Ref<WindowResizedEvent>& event)
		{
			m_Framebuffer->Resize(event->GetWidth(), event->GetHeight());
		});
		
		m_Camera = CreateRef<Camera>(Camera::ProjectionType::Perspective);
		m_Camera->Position = { 0.0f, 0.0f, 5.0f };
		m_Camera->SetPerspective(60.0f, 16.0f / 9.0f, 0.01f, 100.0f);
		m_Camera->SetOrthographic(5.0f, 16.0f / 9.0f, 0.01f, 100.0f);
		m_Camera->Recalculate();

		const Ref<Shader>& shader = Shader::Create("assets/shaders/lit.glsl");
		shader->Compile();

		m_Material = CreateRef<Material>(shader);
		m_Material->SetUniform("u_Albedo", glm::vec3(0.5f, 0.5f, 0.75f));
		m_Material->SetUniform("u_Metallic", 0.75f);
		m_Material->SetUniform("u_Smoothness", 0.5f);

		m_Scene = CreateRef<Scene>("Game");

		for (int i = 0; i < 5; i++)
		{
			const float x = i * 1.5f - 3.0f;
			const float size = (i + 1) * 0.25f;

			const auto gameObject = CreateRef<GameObject>(Transform(
				glm::vec3(x, 0.0f, 0.0f),
				glm::vec3(0.0f),
				glm::vec3(size)
			));

			const Ref<MeshRenderer>& meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->SetMaterial(m_Material);

			m_Scene->AddGameObject(gameObject);
		}

		m_Scene->Init();
	}

	void GameLayer::OnDetach()
	{
		
	}
	
	void GameLayer::Update(const float dt)
	{
		m_Scene->Update(dt);
	}

	void GameLayer::Render()
	{
		m_Framebuffer->Bind();
		
		Renderer::BeginScene(*m_Camera);

		Renderer::GetAPI().SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		Renderer::GetAPI().Clear();

		m_Scene->Render();

		Renderer::EndScene();

		m_Framebuffer->Unbind();
	}

	ImVec2 GetViewportLargestSize()
	{
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		windowSize.x -= ImGui::GetScrollX();
		windowSize.y -= ImGui::GetScrollY();

		constexpr float aspectRatio = 16.0f / 9.0f;

		float aspectWidth = windowSize.x;
		float aspectHeight = aspectWidth / aspectRatio;
		if (aspectHeight > windowSize.y)
		{
			aspectHeight = windowSize.y;
			aspectWidth = aspectHeight * aspectRatio;
		}

		return ImVec2(aspectWidth, aspectHeight);
	}

	ImVec2 GetViewportCenteredPosition(const ImVec2& aspectSize)
	{
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		windowSize.x -= ImGui::GetScrollX();
		windowSize.y -= ImGui::GetScrollY();

		const float viewportX = (windowSize.x - aspectSize.x) / 2.0f;
		const float viewportY = (windowSize.y - aspectSize.y) / 2.0f;

		return ImVec2(viewportX + ImGui::GetCursorPosX(), viewportY + ImGui::GetCursorPosY());
	}

	void GameLayer::RenderImGui()
	{
		ImGui::Begin("Settings");

		static bool showCameraHeader = true;
		if (ImGui::CollapsingHeader("Camera", showCameraHeader))
		{
			const char* projectionTypeNames[] = { "Orthographic", "Perspective" };
			
			int projectionType = (int)m_Camera->GetProjectionType();
			if (ImGui::Combo("Projection Type", &projectionType, projectionTypeNames, IM_ARRAYSIZE(projectionTypeNames)))
				m_Camera->SetProjectionType((Camera::ProjectionType)projectionType);

			ImGui::Separator();

			if (m_Camera->GetProjectionType() == Camera::ProjectionType::Orthographic)
			{
				if (ImGui::DragFloat2("Position", glm::value_ptr(m_Camera->Position), 0.1f))
					m_Camera->Recalculate();
			} else if (m_Camera->GetProjectionType() == Camera::ProjectionType::Perspective)
			{
				if (ImGui::DragFloat3("Position", glm::value_ptr(m_Camera->Position), 0.1f))
					m_Camera->Recalculate();
			}
			if (ImGui::DragFloat3("Rotation", glm::value_ptr(m_Camera->EulerAngles)))
				m_Camera->RecalculateEuler();

			ImGui::Separator();
			
			if (m_Camera->GetProjectionType() == Camera::ProjectionType::Orthographic)
			{
				if (ImGui::DragFloat("Orthographic Size", &m_Camera->OrthographicSize))
					m_Camera->Recalculate();
			} else if (m_Camera->GetProjectionType() == Camera::ProjectionType::Perspective)
			{
				if (ImGui::DragFloat("Field of View", &m_Camera->PerspectiveFOV))
					m_Camera->Recalculate();
			}
		}

		ImGui::Spacing();

		static bool showMaterialHeader = true;
		if (ImGui::CollapsingHeader("Material", showMaterialHeader))
		{
			glm::vec3 albedo;
			if (m_Material->GetUniformValue("u_Albedo", albedo))
			{
				if (ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo)))
					m_Material->SetUniformValue("u_Albedo", albedo);
			}

			float metallic;
			if (m_Material->GetUniformValue("u_Metallic", metallic))
			{
				if (ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f, "%.2f"))
					m_Material->SetUniformValue("u_Metallic", metallic);
			}

			float smoothness;
			if (m_Material->GetUniformValue("u_Smoothness", smoothness))
			{
				if (ImGui::SliderFloat("Smoothness", &smoothness, 0.0f, 1.0f, "%.2f"))
					m_Material->SetUniformValue("u_Smoothness", smoothness);
			}
		}

		ImGui::End();

		ImGui::Begin("Viewport");
		
		const ImVec2 viewportSize = GetViewportLargestSize();
		const ImVec2 viewportPos = GetViewportCenteredPosition(viewportSize);

		ImGui::SetCursorPos(viewportPos);
		const uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)(intptr_t)texID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		
		ImGui::End();

		m_Scene->RenderImGui();
	}
}
