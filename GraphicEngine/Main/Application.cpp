#include "Application.hpp"

#include "../Common/Window.hpp"
#include "../Platform/Glew/WindowGLFW.hpp"
#include "../Common/RenderingEngine.hpp"
#include "../Drivers/Vulkan/VulkanRenderingEngine.hpp"
#include "../Drivers/OpenGL/OpenGLRenderingEngine.hpp"

#include <exception>
#include <iostream>

using namespace GraphicEngine::HID;

Application::Application(int argc, char** argv)
{
	// TODO - parse argc and argv to get parameters

}

void Application::exec()
{
	try
	{
		keyboard = std::shared_ptr<Keyboard>(new Keyboard);
		mouse = std::shared_ptr<Mouse>(new Mouse);
		
		auto window = windowFactory("glfw");
		auto renderingEngine = renderingEngineFactory("vulkan", window);
		
		window->init(640, 480);
		window->registerKeyboard(keyboard);
		window->registerMouse(mouse);

		renderingEngine->init(window->getWidth(), window->getHeight());
		window->addResizeCallbackListener([&](size_t width, size_t height) {renderingEngine->resizeFrameBuffer(width, height); });


		// Only for debug
		/*keyboard->subscribe([](std::vector<Key> keys)
			{
				if (!keys.empty())
				{
					for (auto key : keys)
					{
						std::cout << key << " ";
					}
					std::cout << "\n";
				}
			}
		);

		mouse->subscribePositionEventHandler([](double x, double y)
			{
				std::cout << "Position: " << x << " " << y << "\n";
			});

		mouse->subscribeScrollEventHandler([](double x, double y)
			{
				std::cout << "Scroll: " << x << " " << y << "\n";
			});

		mouse->subscribe([](std::vector<Button> button)
			{
				if (!button.empty())
				{
					for (auto button : button)
					{
						std::cout << button << " ";
					}
					std::cout << "\n";
				}
			});

		window->addResizeCallbackListener([](size_t w, size_t h) {std::cout << w << " " << h << "\n"; });*/

		engine = std::shared_ptr<GraphicEngine::Engine>(new GraphicEngine::Engine(window, renderingEngine, keyboard, mouse));

		engine->run();
	}

	catch (std::exception ex)
	{
		std::cout << ex.what() << "\n";
	}
}

std::shared_ptr<GraphicEngine::Window> Application::windowFactory(std::string windowType)
{
	if (windowType == "glfw")
		return std::shared_ptr<GraphicEngine::Window>(new GraphicEngine::GLFW::WindowGLFW);

	throw std::exception("Window type not supported!");
}

std::shared_ptr<GraphicEngine::RenderingEngine> Application::renderingEngineFactory(std::string type, std::shared_ptr<GraphicEngine::Window> window)
{
	std::shared_ptr<GraphicEngine::RenderingEngine> renderingEngine;
	if (type == "vulkan")
	{
		renderingEngine = std::shared_ptr<GraphicEngine::RenderingEngine>(new GraphicEngine::Vulkan::VulkanRenderingEngine(window));
		auto glfwWindow = std::dynamic_pointer_cast<GraphicEngine::GLFW::WindowGLFW>(window);
		if (glfwWindow != nullptr)
		{
			glfwWindow->setGLFWWindowProfile(GraphicEngine::GLFW::GLFWWindowProfile::VULKAN);
		}
		return renderingEngine;
	}

	else if (type == "opengl")
	{
		renderingEngine = std::shared_ptr<GraphicEngine::RenderingEngine>(new GraphicEngine::OpenGL::OpenGLRenderingEngine(window));
		auto glfwWindow = std::dynamic_pointer_cast<GraphicEngine::GLFW::WindowGLFW>(window);
		if (glfwWindow != nullptr)
		{
			glfwWindow->setGLFWWindowProfile(GraphicEngine::GLFW::GLFWWindowProfile::OPENGL);
		}
		return renderingEngine;
	}
	throw std::exception("Rendering type not supported!");
}
