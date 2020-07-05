#pragma once

#include "../Core/Configuration.hpp"
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
		glm::vec2 m_cursorPosition = glm::vec2();
	};

	class MousePressedButtons
	{
	public:
		virtual ~MousePressedButtons() = default;
		virtual std::vector<Core::Inputs::MouseButton> getPressedButtons() = 0;
	protected:
		std::vector<Core::Inputs::MouseButton> m_pressedButtons;
	};

	class MouseScrollValue
	{
	public:
		virtual ~MouseScrollValue() = default;
		virtual glm::vec2 getScrollValue() = 0;
	protected:
		glm::vec2 m_scrollValues = glm::vec2();
	};

	class Mouse : public MouseCursorPosition, public MousePressedButtons, public MouseScrollValue
	{
	public:
		Mouse(std::shared_ptr<Core::Configuration> cfg);
		float getSensitivity();
		void setSensitivity(float sensitivity);
		virtual ~Mouse() = default;

	protected:
		float m_sensitivity;
		std::shared_ptr<Core::Configuration> m_cfg;
	};
}
