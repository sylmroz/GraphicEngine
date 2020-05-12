#pragma once

#include "MouseEnumButton.hpp"

#include <functional>
#include <vector>
#include "glm/vec2.hpp"

namespace GraphicEngine::Core::Inputs
{
	struct MouseButtonPositionScroll
	{
		glm::vec2 cursorPosition;
		glm::vec2 scrollPosition;
		std::vector<MouseButton> pressedButtons;
	};
	class MousePositionEventHandler : public Subject<double, double> {};

	class MouseScrollEventHandler : public Subject<double, double> {};

	class MouseButtonEventHandler : public Subject<std::vector<MouseButton>> {};

	class MouseButtonPositionScrollEventHandler : public Subject<MouseButtonPositionScroll>{};
	
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

		MouseButtonPositionScrollEventHandler buttonPositionScrollEventHandler() const
		{
			return _buttonPositionScrollEventHandler;
		}
		
	private:
		MousePositionEventHandler _positionEventHandlers;
		MouseScrollEventHandler _scrollEventHandlers;
		MouseButtonEventHandler _buttonEventHandlers;
		MouseButtonPositionScrollEventHandler _buttonPositionScrollEventHandler;
	};
}
