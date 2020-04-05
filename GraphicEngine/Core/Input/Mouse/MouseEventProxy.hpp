#ifndef GRAPHIC_ENGINE_HID_MOUSE_HPP
#define GRAPHIC_ENGINE_HID_MOUSE_HPP

#include "MouseEnumButton.hpp"

#include <functional>
#include <vector>

namespace GraphicEngine::Core::Inputs
{
	class MousePositionEventHandler : public Subject<double, double> {};

	class MouseScrollEventHandler : public Subject<double, double> {};

	class MouseButtonEventHandler : public Subject<std::vector<MouseButton>> {};
	
	class MouseEventProxy
	{
	public:

		MousePositionEventHandler positionEventHandler() const
		{
			return _positionEventHandlers;
		}

		MouseScrollEventHandler scrollEventHandler() const
		{
			return _scrollEventHandlers;
		}

		MouseButtonEventHandler buttonEventHandler() const
		{
			return _buttonEventHandlers;
		}
		
	private:
		MousePositionEventHandler _positionEventHandlers;
		MouseScrollEventHandler _scrollEventHandlers;
		MouseButtonEventHandler _buttonEventHandlers;
	};
}

#endif // !GRAPHIC_ENGINE_HID_MOUSE_HPP
