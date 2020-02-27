#ifndef GRAPHIC_ENGINE_UTILS_SUBJECT_HPP
#define GRAPHIC_ENGINE_UTILS_SUBJECT_HPP

#include <functional>
#include <vector>

namespace GraphicEngine::Utils
{
	template <typename... Args>
	class Subject
	{
	public:
		void subscribe(std::function<void(Args...)> f)
		{
			functions.push_back(f);
		}

		void next(Args... args)
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

#endif // !GRAPHIC_ENGINE_UTILS_SUBJECT_HPP

