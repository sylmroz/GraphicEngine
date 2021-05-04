#pragma once

#include "Subject.hpp"
#include "Logger.hpp"

#include <chrono>

namespace GraphicEngine::Core
{
	class Timer
	{
	public:
		void start()
		{
			m_actualTime = std::chrono::system_clock::now();
			m_startTime = m_actualTime;
		}

		void updateTime()
		{
			auto stop = std::chrono::system_clock::now();
			m_interval = stop - m_actualTime;
			m_actualTime = stop;
			m_currentTimeNotifier.notify((stop - m_startTime).count());
			m_updateNotifier.notify(m_interval.count());
		}

		double getInterval()
		{
			return m_interval.count();
		}

		template <typename Callback>
		void onTimeUpdate(Callback callback)
		{
			m_updateNotifier.subscribe(callback);
		}

		template <typename Callback>
		void onCurrentTimeUpdate(Callback callback)
		{
			m_currentTimeNotifier.subscribe(callback);
		}

	protected:
		std::chrono::time_point<std::chrono::system_clock> m_actualTime;
		std::chrono::time_point<std::chrono::system_clock> m_startTime;
		std::chrono::duration<double> m_interval;

		Subject<double> m_updateNotifier;
		Subject<double> m_currentTimeNotifier;
	};
}
