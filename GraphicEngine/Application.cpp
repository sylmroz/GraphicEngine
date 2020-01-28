#include "Application.hpp"

#include "Window/Window.hpp"
#include "Window/WindowGLFW.hpp"
#include "RenderingEngine/RenderingEngine.hpp"
#include "RenderingEngine/VulkanRenderingEngine.hpp"
#include "RenderingEngine/OpenGLRenderingEngine.hpp"

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
		window->init(640, 480);
		window->registerKeyboard(keyboard);
		window->registerMouse(mouse);

		auto renderingEngine = renderingEngineFactory("vulkan", window);
		renderingEngine->init();


		// Only for debug
		keyboard->subscribe([](std::vector<Key> keys)
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

		engine = std::shared_ptr<GraphicEngine::Engine>(new GraphicEngine::Engine(window, renderingEngine, keyboard, mouse));

		engine->run();
	}

	catch (std::exception ex)
	{
		std::cout << ex.what() << "\n";
	}
}

std::shared_ptr<GraphicEngine::Window> Application::windowFactory(std::string type)
{
	if (type == "glfw")
		return std::shared_ptr<GraphicEngine::Window>(new GraphicEngine::GLFW::WindowGLFW);
	throw std::exception("Window type not supported!");
}

std::shared_ptr<GraphicEngine::RenderingEngine> Application::renderingEngineFactory(std::string type, std::shared_ptr<GraphicEngine::Window> window)
{
	std::shared_ptr<GraphicEngine::RenderingEngine> renderingEngine;
	if (type == "vulkan")
	{
		renderingEngine = std::shared_ptr<GraphicEngine::RenderingEngine>(new GraphicEngine::Vulkan::VulkanRenderingEngine);
		auto glfwWindow = std::dynamic_pointer_cast<GraphicEngine::GLFW::WindowGLFW>(window);
		if (glfwWindow != nullptr)
		{
			glfwWindow->setGLFWWindowProfile(GraphicEngine::GLFW::GLFWWindowProfile::VULKAN);
		}
		return renderingEngine;
	}

	else if (type == "opengl")
	{
		renderingEngine = std::shared_ptr<GraphicEngine::RenderingEngine>(new GraphicEngine::Vulkan::OpenGLRenderingEngine);
		auto glfwWindow = std::dynamic_pointer_cast<GraphicEngine::GLFW::WindowGLFW>(window);
		if (glfwWindow != nullptr)
		{
			glfwWindow->setGLFWWindowProfile(GraphicEngine::GLFW::GLFWWindowProfile::OPENGL);
		}
		return renderingEngine;
	}
	throw std::exception("Rendering type not supported!");
}
