#ifndef GRAPHIC_ENGINE_CORE_TIMER_HPP
#define GRAPHIC_ENGINE_CORE_TIMER_HPP

#include <chrono>

namespace GraphicEngine::Core
{
	class Timer
	{
	public:
		void start()
		{
			_actualTime = std::chrono::system_clock::now();
		}

		void updateTime()
		{
			auto stop = std::chrono::system_clock::now();
			_interval = stop - _actualTime;
			_actualTime = stop;
		}

		double getInterval()
		{
			return _interval.count();
		}

	protected:
		std::chrono::time_point<std::chrono::system_clock> _actualTime;
		std::chrono::duration<double> _interval;
	};
}

#endif //!GRAPHIC_ENGINE_CORE_TIMER_HPP
