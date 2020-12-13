#pragma once

#include "../Utils/GetClassName.hpp"
#include <spdlog/fmt/fmt.h>
#include <sstream>
#include <thread>

namespace GraphicEngine::Core::LoggerCore
{
	template <typename T>
	class LoggerName
	{
	public:
		auto operator()()
		{
			std::string className = Utils::getClassName<T>();
			auto threadId = std::this_thread::get_id();
			std::stringstream ss;
			ss << threadId;
			return fmt::format("[id:{}]:[{}]", ss.str(), className);
		}
	};
}