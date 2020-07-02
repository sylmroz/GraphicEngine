#pragma once

#include "MouseEnumButton.hpp"

#include "../GenericClickEvent.hpp"

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

	class MouseButtonEventHandler : public Subject<std::vector<MouseButton>> 
	{
	public:
		MouseButtonEventHandler()
		{
			subscribe([&](std::vector<MouseButton> buttons)
				{
					m_buttonEvents.setNewEvents(buttons);
				});
		}
		void onButtonDown(std::function<void(MouseButton)> callback)
		{
			m_buttonEvents.onDown(callback);
		}

		void onButtonsDown(std::function<void(std::vector<MouseButton>)> callback)
		{
			m_buttonEvents.onAllDown(callback);
		}

		void onButtonUp(std::function<void(MouseButton)> callback)
		{
			m_buttonEvents.onUp(callback);
		}

		void onButtonsUp(std::function<void(std::vector<MouseButton>)> callback)
		{
			m_buttonEvents.onAllUp(callback);
		}
	private:
		GenericClickEvent<MouseButton, std::vector> m_buttonEvents;
	};

	class MouseButtonPositionScrollEventHandler : public Subject<MouseButtonPositionScroll>{};
	
	class MouseEventProxy
	{
	public:

		MousePositionEventHandler positionEventHandler() const
		{
			return m_positionEventHandlers;
		}

		MouseScrollEventHandler scrollEventHandler() const
		{
			return m_scrollEventHandlers;
		}

		MouseButtonEventHandler buttonEventHandler() const
		{
			return m_buttonEventHandlers;
		}

		MouseButtonPositionScrollEventHandler buttonPositionScrollEventHandler() const
		{
			return m_buttonPositionScrollEventHandler;
		}
		
	private:
		MousePositionEventHandler m_positionEventHandlers;
		MouseScrollEventHandler m_scrollEventHandlers;
		MouseButtonEventHandler m_buttonEventHandlers;
		MouseButtonPositionScrollEventHandler m_buttonPositionScrollEventHandler;
	};
}
