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
		template <typename WindowApi>
		struct WindowGLFWApi
		{
			void initialize()
			{
				static_cast<WindowApi&>(*this).init();
			}
		};

		struct WindowGLFWOpenGl : WindowGLFWApi<WindowGLFWOpenGl>
		{
			void init();
		};

		struct WindowGLFWVulkan : WindowGLFWApi<WindowGLFWVulkan>
		{
			void init();
		};

	public:

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
	};
}

#endif // !#ifndef GRAPHIC_ENGINE_WINDOW_HPP

