#pragma once

#include "../Utils/GetClassName.hpp"

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
			return lastDoubeDot != std::string::npos ? loggerName.substr(lastDoubeDot + 1) : loggerName;
		}
	};
}