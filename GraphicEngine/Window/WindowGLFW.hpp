#ifndef GRAPHIC_ENGINE_WINDOW_GLFW_HPP
#define GRAPHIC_ENGINE_WINDOW_GLFW_HPP

#include "Window.hpp"

#include <GLFW/glfw3.h>

#include <memory>

namespace GraphicEngine::GLFW
{
	enum GLFWWindowProfile
	{
		OPENGL,
		VULKAN,
		NONE
	};

	class WindowGLFW : public GraphicEngine::Window
	{
		struct WindowGLFWApi
		{
			virtual void init() = 0;
			virtual void swapBuffers(GLFWwindow* window) = 0;
		};

		struct WindowGLFWOpenGl : WindowGLFWApi
		{
			virtual void init() override;
			virtual void swapBuffers(GLFWwindow* window) override;
		};

		struct WindowGLFWVulkan : WindowGLFWApi
		{
			virtual void init() override;
			virtual void swapBuffers(GLFWwindow* window) override;
		};

	public:

		virtual ~WindowGLFW();

		void setGLFWWindowProfile(GLFWWindowProfile windowProfile) { _windowProfile = windowProfile; };

		// Inherited via Window
		virtual void swapBuffer() override;
		virtual void initialize() override;

		virtual void poolEvents();

		virtual bool windowShouldBeClosed() override;

		virtual void registerMouse(std::shared_ptr<GraphicEngine::HID::Mouse> mouse) override;

	private:
		void grabAllKeys();
		void grabAllPressedMouseButtons();

	private:
		std::shared_ptr<GLFWwindow> _glfwWindow;
		GLFWWindowProfile _windowProfile = GLFWWindowProfile::NONE;

		std::shared_ptr<WindowGLFWApi> _specialApi;
	};
}

#endif // !#ifndef GRAPHIC_ENGINE_WINDOW_HPP

