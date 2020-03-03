#ifndef GRAPHIC_ENGINE_HID_MOUSE_HPP
#define GRAPHIC_ENGINE_HID_MOUSE_HPP

#include "MouseEnumButton.hpp"

#include <functional>
#include <vector>

namespace GraphicEngine::Core::Inputs
{
	class Mouse
	{
	public:

		void positionEventHandlerNotify(double xPos, double yPos)
		{
			notify(_positionEventHandlers, xPos, yPos);
		}

		void scrollnEventHandlerNotify(double xOffset, double yOffset)
		{
			notify(_scrollEventHandlers, xOffset, yOffset);
		}

		void subscribePositionEventHandler(std::function<void(double, double)> eventHandler)
		{
			_positionEventHandlers.push_back(eventHandler);
		}

		void subscribeScrollEventHandler(std::function<void(double, double)> eventHandler)
		{
			_scrollEventHandlers.push_back(eventHandler);
		}

		void notify(std::vector<MouseButton> buttons)
		{
			for (auto eventHandler : _buttonEventHandlers)
			{
				eventHandler(buttons);
			}
		}

		void subscribe(std::function<void(std::vector<MouseButton>)> eventHandler)
		{
			_buttonEventHandlers.push_back(eventHandler);
		}

	private:
		void notify(std::vector<std::function<void(double, double)>>& handlers, double x, double y)
		{
			for (auto h : handlers)
			{
				h(x, y);
			}
		}
	private:
		std::vector<std::function<void(double, double)>> _positionEventHandlers;
		std::vector<std::function<void(double, double)>> _scrollEventHandlers;
		std::vector<std::function<void(std::vector<MouseButton>)>> _buttonEventHandlers;
	};
}

#endif // !GRAPHIC_ENGINE_HID_MOUSE_HPP


