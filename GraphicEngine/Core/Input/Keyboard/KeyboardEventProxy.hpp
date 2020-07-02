#pragma once

#include "KeyboardEnumKeys.hpp"

#include "../GenericClickEvent.hpp"

#include <vector>

namespace GraphicEngine::Core::Inputs
{
	class KeyboardEventProxy: public Subject<std::vector<KeyboardKey>>
	{
	public:

		KeyboardEventProxy()
		{
			subscribe([&](std::vector<KeyboardKey> keys)
				{
					m_clickEvent.setNewEvents(keys);
				});
		}

		void onKeyDown(std::function<void(KeyboardKey)> callback)
		{
			m_clickEvent.onDown(callback);
		}

		void onKeysDown(std::function<void(std::vector<KeyboardKey>)> callback)
		{
			m_clickEvent.onAllDown(callback);
		}

		void onKeyUp(std::function<void(KeyboardKey)> callback)
		{
			m_clickEvent.onUp(callback);
		}

		void onKeysUp(std::function<void(std::vector<KeyboardKey>)> callback)
		{
			m_clickEvent.onAllUp(callback);
		}
		
	private:
		GenericClickEvent<KeyboardKey, std::vector> m_clickEvent;
	};
}
