#ifndef GRAPHIC_ENGINE_COMMON_WINDOW_KEYBOARD_MOUSE_HPP
#define GRAPHIC_ENGINE_COMMON_WINDOW_KEYBOARD_MOUSE_HPP

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Window.hpp"

namespace GraphicEngine::Common
{
	class WindowKeyboardMouse : public Window, public Keyboard, public Mouse {};
}

#endif // !GRAPHIC_ENGINE_COMMON_WINDOW_KEYBOARD_MOUSE_HPP
