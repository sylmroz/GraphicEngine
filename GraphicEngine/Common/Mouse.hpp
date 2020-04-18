#pragma once

#include "../Core/Input/Mouse/MouseEnumButton.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace GraphicEngine::Common
{
	class MouseCursorPosition
	{
	public:
		virtual ~MouseCursorPosition() = default;
		virtual void setCursorPosition(const glm::vec2& pos) = 0;
		virtual glm::vec2 getCursorPosition() = 0;
	protected:
		glm::vec2 _cursorPosition = glm::vec2();
	};

	class MousePressedButtons
	{
	public:
		virtual ~MousePressedButtons() = default;
		virtual std::vector<Core::Inputs::MouseButton> getPressedButtons() = 0;
	protected:
		std::vector<Core::Inputs::MouseButton> _pressedButtons;
	};

	class MouseScrollValue
	{
	public:
		virtual ~MouseScrollValue() = default;
		virtual glm::vec2 getScrollValue() = 0;
	protected:
		glm::vec2 _scrollValues = glm::vec2();
	};

	class Mouse : public MouseCursorPosition, public MousePressedButtons, public MouseScrollValue
	{
	public:
		virtual ~Mouse() = default;
	};
}
