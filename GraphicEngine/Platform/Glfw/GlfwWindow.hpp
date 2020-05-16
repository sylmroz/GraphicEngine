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
	};
}
