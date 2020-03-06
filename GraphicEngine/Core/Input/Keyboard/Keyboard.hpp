#ifndef GRAPHIC_ENGINE_HID_KEYBOARD_HPP
#define GRAPHIC_ENGINE_HID_KEYBOARD_HPP

#include "KeyboardEnumKeys.hpp"

#include "../../Subject.hpp"

#include <vector>

namespace GraphicEngine::Core::Inputs
{
	class Keyboard: public Subject<std::vector<KeyboardKey>>
	{
	};
}

#endif // !GRAPHIC_ENGINE_HID_KEYBOARD_HPP

