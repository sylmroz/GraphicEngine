#include "SourceFormatter.hpp"

#include <spdlog/fmt/fmt.h>


GraphicEngine::Core::LoggerCore::LoggerFormatter::LoggerFormatter(const std::string& fileName, uint32_t codeLine, const std::string functionName):
	m_fileName(fileName),
	m_codeLine(codeLine),
	m_functionName(functionName)
{
	auto lastSlash = m_fileName.find_last_of('\\');
	auto shorterFileName = lastSlash != std::string::npos ? m_fileName.substr(lastSlash + 1) : m_fileName;

	m_result = fmt::format(m_format, shorterFileName, m_codeLine, functionName);
}

std::string GraphicEngine::Core::LoggerCore::LoggerFormatter::operator()()
{
	return m_result;
}
