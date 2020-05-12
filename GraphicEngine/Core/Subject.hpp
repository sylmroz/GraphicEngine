#pragma once

#include <functional>
#include <vector>

namespace GraphicEngine::Core
{
	template <typename... Args>
	class Subject
	{
	public:
		void subscribe(std::function<void(Args...)> f)
		{
			functions.push_back(f);
		}

		void notify(Args... args)
		{
			for (auto& f : functions)
			{
				f(args...);
			}
		}

	private:
		std::vector<std::function<void(Args...)>> functions;
	};
}
