#pragma once

namespace Quantum
{
	class Layer
	{
	public:
		explicit Layer(const std::string& name = "Layer")
			: m_Name(name) {}
		
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void Update(float dt) {}
		virtual void Render() {}
		virtual void RenderImGui() {}

		const std::string& GetName() const { return m_Name; }

	private:
		std::string m_Name;
	};
}