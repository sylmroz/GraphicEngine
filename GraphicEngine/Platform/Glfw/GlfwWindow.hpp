#pragma once

#include <GLFW/glfw3.h>

#include <memory>

#include "../../Common/WindowKeyboardMouse.hpp"

namespace GraphicEngine::GLFW
{
	enum class WindowGlfwProfile
	{
		OpenGL,
		Vulkan,
		None
	};

	class GlfwWindow : public Common::WindowKeyboardMouse
	{
	public:
		GlfwWindow(std::shared_ptr<Core::Configuration> cfg) :WindowKeyboardMouse(cfg) {}
		//From Keyboard interface
		std::vector<Core::Inputs::KeyboardKey> getPressedKeys() override;

		// From Mouse Interface
		std::vector<Core::Inputs::MouseButton> getPressedButtons() override;

		void setCursorPosition(const glm::vec2& pos) override;

		glm::vec2 getCursorPosition() override;

		glm::vec2 getScrollValue() override;

	private:
		struct WindowGlfwApi
		{
			virtual ~WindowGlfwApi() = default;
			virtual void init() = 0;
			virtual void swapBuffers(GLFWwindow* window) = 0;
		};

		struct WindowGlfwOpenGL final : WindowGlfwApi
		{
			virtual void init() override;
			virtual void swapBuffers(GLFWwindow* window) override;
		};

		struct WindowGlfwVulkan final : WindowGlfwApi
		{
			virtual void init() override;
			virtual void swapBuffers(GLFWwindow* window) override;
		};

	public:

		virtual ~GlfwWindow();

		void setWindowGlfwProfile(const WindowGlfwProfile windowProfile) { m_windowProfile = windowProfile; };

		// Inherited via Window
		virtual void swapBuffer() override;
		virtual void initialize() override;

		virtual void poolEvents() override;

		virtual bool windowShouldBeClosed() override;

		virtual std::pair<uint32_t, uint32_t> getFrameBufferSize() override;

		std::shared_ptr<GLFWwindow> getGlfwWindow();

	protected:
		std::shared_ptr<GLFWwindow> m_glfwWindow;
		WindowGlfwProfile m_windowProfile = WindowGlfwProfile::None;

		std::shared_ptr<WindowGlfwApi> m_specialApi;

		const std::map<int, Core::Inputs::MouseButton> m_mappedButtons = {
			{ GLFW_MOUSE_BUTTON_LEFT, Core::Inputs::MouseButton::buttonLeft },
			{ GLFW_MOUSE_BUTTON_MIDDLE, Core::Inputs::MouseButton::butonMiddle },
			{ GLFW_MOUSE_BUTTON_RIGHT, Core::Inputs::MouseButton::buttonRight }
		};

		const std::map<int, Core::Inputs::KeyboardKey> m_mappedKeys = {
		   { GLFW_KEY_ESCAPE, Core::Inputs::KeyboardKey::KEY_ESCAPE },
		   { GLFW_KEY_W, Core::Inputs::KeyboardKey::KEY_W },
		   { GLFW_KEY_A, Core::Inputs::KeyboardKey::KEY_A },
		   { GLFW_KEY_S, Core::Inputs::KeyboardKey::KEY_S },
		   { GLFW_KEY_D, Core::Inputs::KeyboardKey::KEY_D },
		   { GLFW_KEY_C, Core::Inputs::KeyboardKey::KEY_C },
		   { GLFW_KEY_ENTER, Core::Inputs::KeyboardKey::KEY_ENTER },
		   { GLFW_KEY_0, Core::Inputs::KeyboardKey::KEY_0 },
		   { GLFW_KEY_1, Core::Inputs::KeyboardKey::KEY_1 },
		   { GLFW_KEY_2, Core::Inputs::KeyboardKey::KEY_2 },
		   { GLFW_KEY_3, Core::Inputs::KeyboardKey::KEY_3 },
		   { GLFW_KEY_4, Core::Inputs::KeyboardKey::KEY_4 },
		   { GLFW_KEY_5, Core::Inputs::KeyboardKey::KEY_5 },
		   { GLFW_KEY_6, Core::Inputs::KeyboardKey::KEY_6 },
		   { GLFW_KEY_7, Core::Inputs::KeyboardKey::KEY_7 },
		   { GLFW_KEY_8, Core::Inputs::KeyboardKey::KEY_8 },
		   { GLFW_KEY_9, Core::Inputs::KeyboardKey::KEY_9 }
		};
	};
}
