#pragma once

namespace Quantum
{
	enum class EventType
	{
		WindowCloseEvent,
		WindowResizedEvent,
		WindowFocusGainedEvent, WindowFocusLostEvent,
		WindowMinimizedEvent, WindowRestoredEvent
	};
	
	class Event
	{
	public:
		virtual ~Event() = default;

	public:
		virtual const std::string& GetName() const = 0;
	};
	
	class EventManager
	{
	public:
		using EventCallback = std::function<void(const Ref<Event>&)>;

		template<typename T>
		static void AddCallback(EventCallback callback)
		{
			GetInstance().m_Callbacks[T::GetStaticType()].push_back(callback);
		}

		template<typename EventType>
		static void Dispatch(const EventType& event)
		{
			auto& callbacks = GetCallbacks<EventType>();
			for (const auto& callback : callbacks)
				callback(event);
		}

		template<typename EventType, typename... Args>
		static void Dispatch(Args&&... args)
		{
			EventType event{std::forward<Args>(args)...};
			auto& callbacks = GetCallbacks<EventType>();
			for (const auto& callback : callbacks)
				callback(event);
		}

	private:
		static EventManager& GetInstance()
		{
			static EventManager instance;
			return instance;
		}

		std::unordered_map<EventType, std::vector<EventCallback>> m_Callbacks;
	};

}