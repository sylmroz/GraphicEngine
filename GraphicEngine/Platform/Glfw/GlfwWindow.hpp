#ifndef GRAPHIC_ENGINE_WINDOW_GLFW_HPP
#define GRAPHIC_ENGINE_WINDOW_GLFW_HPP

#include "../../Common/Window.hpp"

#include <GLFW/glfw3.h>

#include <memory>

#include "../../Common/WindowKeyboardMouse.hpp"

namespace GraphicEngine::GLFW
{
	enum class GLFWWindowProfile
	{
		OpenGL,
		Vulkan,
		None
	};

	class GlfwWindow : public Common::WindowKeyboardMouse
	{
	public:
		//From Keyboard interface
		std::vector<Core::Inputs::KeyboardKey> getPressedKeys() override;

		// From Mouse Interface
		std::vector<Core::Inputs::MouseButton> getPressedButtons() override;

		void setCursorPosition(const glm::vec2& pos) override;

		glm::vec2 getCursorPosition() override;

		glm::vec2 getScrollValue() override;

	private:
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

		virtual ~GlfwWindow();

		void setGLFWWindowProfile(GLFWWindowProfile windowProfile) { _windowProfile = windowProfile; };

		// Inherited via Window
		virtual void swapBuffer() override;
		virtual void initialize() override;

		virtual void poolEvents() override;
		
		virtual bool windowShouldBeClosed() override;

		virtual std::pair<uint32_t, uint32_t> getFrameBufferSize() override;

		std::shared_ptr<GLFWwindow> getGlfwWindow();

	protected:
		std::shared_ptr<GLFWwindow> _glfwWindow;
		GLFWWindowProfile _windowProfile = GLFWWindowProfile::None;

		std::shared_ptr<WindowGLFWApi> _specialApi;
	};
}

#endif // !#ifndef GRAPHIC_ENGINE_WINDOW_HPP

