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
			std::string loggerName = Utils::getClassName<T>();
			auto lastDoubeDot = loggerName.find_last_of(':');
			std::string className = lastDoubeDot != std::string::npos ? loggerName.substr(lastDoubeDot + 1) : loggerName;
			auto threadId = std::this_thread::get_id();
			std::stringstream ss;
			ss << threadId;
			return fmt::format("[id:{}]:[{}]", ss.str(), className);
		}
	};
}