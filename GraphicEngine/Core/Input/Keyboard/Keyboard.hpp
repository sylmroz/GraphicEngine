#ifndef GRAPHIC_ENGINE_HID_KEYBOARD_HPP
#define GRAPHIC_ENGINE_HID_KEYBOARD_HPP

#include "KeyboardEnumKeys.hpp"

#include <functional>
#include <vector>

namespace GraphicEngine::HID
{
	class Keyboard
	{
	public:

		void notify(std::vector<Key> keys)
		{
			for (auto eventHandler : _eventHandlers)
			{
				eventHandler(keys);
			}
		}

		void subscribe(std::function<void(std::vector<Key>)> eventHandler)
		{
			_eventHandlers.push_back(eventHandler);
		}
	private:
		std::vector<std::function<void(std::vector<Key>)>> _eventHandlers;
	};
}

#endif // !GRAPHIC_ENGINE_HID_KEYBOARD_HPP

