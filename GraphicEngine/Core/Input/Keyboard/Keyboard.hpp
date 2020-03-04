#ifndef GRAPHIC_ENGINE_HID_KEYBOARD_HPP
#define GRAPHIC_ENGINE_HID_KEYBOARD_HPP

#include "KeyboardEnumKeys.hpp"

#include "../../Subject.hpp"

#include <vector>

namespace GraphicEngine::Core::Inputs
{
	class Keyboard: public Subject<std::vector<KeyboardKey>>
	{
	public:

		/*void notify(std::vector<KeyboardKey> keys)
		{
			for (auto eventHandler : _eventHandlers)
			{
				eventHandler(keys);
			}
		}

		void subscribe(std::function<void(std::vector<KeyboardKey>)> eventHandler)
		{
			_eventHandlers.push_back(eventHandler);
		}*/
	private:
		//std::vector<std::function<void(std::vector<KeyboardKey>)>> _eventHandlers;
	};
}

#endif // !GRAPHIC_ENGINE_HID_KEYBOARD_HPP

