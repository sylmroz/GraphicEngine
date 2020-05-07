#pragma once
#include <string>

namespace GraphicEngine::Core::LoggerCore
{
	class LoggerFormatter
	{
	public:
		LoggerFormatter(const std::string& fileName, uint32_t codeLine, const std::string functionName);

		std::string operator()();

	private:
		std::string m_fileName;
		uint32_t m_codeLine{ 0 };
		std::string m_functionName;

		std::string m_format = "[{}:{} {}()] : ";

		std::string m_result;
	};
}