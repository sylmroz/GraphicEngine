#pragma once

#include <string>

namespace GraphicEngine::Core::LoggerCore
{
	template <typename T>
	class LoggerName
	{
	public:
		auto operator()()
		{
			std::string loggerName = typeid(T).name();
			auto lastDoubeDot = loggerName.find_last_of(':');
			return lastDoubeDot != std::string::npos ? loggerName.substr(lastDoubeDot + 1) : loggerName;
		}
	};
}