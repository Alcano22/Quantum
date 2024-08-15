#pragma once

#include <memory>

#ifdef QTM_DEBUG
#	if defined(QTM_PLATFORM_WINDOWS)
#		define QTM_DEBUGBREAK() __debugbreak()
#	elif defined(QTM_PLATFORM_LINUX)
#		include <signal.h>
#		define QTM_DEBUGBREAK() raise(SIGTRAP)
#	else
#		error "Platform doesn't support debugbreak yet!"
#	endif
#	define QTM_ENABLE_ASSERTIONS
#else
#	define QTM_DEBUGBREAK()
#endif

#define BIT(x) (1 << (x))

namespace Quantum
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	
}

#include "Quantum/"