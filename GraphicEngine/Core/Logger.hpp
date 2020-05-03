#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace GameEngine::Core
{
	template <typename T>
	class Logger
	{
	public:
		Logger()
		{
			auto loggerName = typeid(T).name();
			m_logger = spdlog::stdout_color_mt(loggerName, spdlog::color_mode::always);
			m_logger->set_level(spdlog::level::trace);
		}

		template <typename... Args>
		void trace(const std::string& fileName, uint32_t line, const std::string& functionName, const std::string& msg, Args... args)
		{
			auto format = m_format + msg;
			m_logger->trace(format, fileName, line, functionName, args...);
		}

		template <typename... Args>
		void debug(const std::string& fileName, uint32_t line, const std::string& functionName, const std::string& msg, Args... args)
		{
			auto format = m_format + msg;
			m_logger->debug(format, fileName, line, functionName, args...);
		}

		template <typename... Args>
		void info(const std::string& fileName, uint32_t line, const std::string& functionName, const std::string& msg, Args... args)
		{
			auto format = m_format + msg;
			m_logger->info(format, fileName, line, functionName, args...);
		}

		template <typename... Args>
		void warn(const std::string& fileName, uint32_t line, const std::string& functionName, const std::string& msg, Args... args)
		{
			auto format = m_format + msg;
			m_logger->warn(format, fileName, line, functionName, args...);
		}

		template <typename... Args>
		void error(const std::string& fileName, uint32_t line, const std::string& functionName, const std::string& msg, Args... args)
		{
			auto format = m_format + msg;
			m_logger->error(format, fileName, line, functionName, args...);
		}

		template <typename... Args>
		void critical(const std::string& fileName, uint32_t line, const std::string& functionName, const std::string& msg, Args... args)
		{
			auto format = m_format + msg;
			m_logger->critical(format, fileName, line, functionName, args);
		}
		
	private:
		std::shared_ptr<spdlog::logger> m_logger;
		std::string m_format = "[{}:{} -- {}] : ";
	};
}
