#ifndef GAME_ENGINE_RANGES_CORE
#define GAME_ENGINE_RANGES_CORE

namespace GameEngine::Core::Ranges
{
	template<typename _Container, typename _Predict>
	_Container filter(_Container container, _Predict predict)
	{
		_Container retContainer;
		
		for (auto c : container)
		{
			if (predict(c))
				retContainer.insert(std::end(retContainer), c);
		}

		return retContainer;
	}
}

#endif // !GAME_ENGINE_RANGES_CORE

