#include "qtmpch.h"
#include "LayerStack.h"

namespace Quantum
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}
	
	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}
	
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}
	
	void LayerStack::PopLayer(Layer* layer)
	{
		const auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it == m_Layers.begin() + m_LayerInsertIndex) return;

		layer->OnDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
	
	void LayerStack::PopOverlay(Layer* overlay)
	{
		const auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it == m_Layers.end()) return;

		overlay->OnDetach();
		m_Layers.erase(it);
	}
}
