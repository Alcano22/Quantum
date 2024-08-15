#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Quantum
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>&
	};
}