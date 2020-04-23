#pragma once

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Window.hpp"

namespace GraphicEngine::Common
{
	class WindowKeyboardMouse : public Window, public Keyboard, public Mouse {};
}
