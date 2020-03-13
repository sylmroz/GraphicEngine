#include "Application.hpp"

#include "../Common/Window.hpp"
#include "../Platform/Glew/WindowGLFW.hpp"
#include "../Common/RenderingEngine.hpp"
#include "../Drivers/Vulkan/VulkanRenderingEngine.hpp"
#include "../Drivers/OpenGL/OpenGLRenderingEngine.hpp"

#include <exception>
#include <iostream>

using namespace GraphicEngine::Core::Inputs;

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

		std::shared_ptr<GraphicEngine::Common::Camera> camera(new GraphicEngine::Common::Camera);
		cameraController = std::shared_ptr<GraphicEngine::Common::CameraController>(new GraphicEngine::Common::CameraController(camera));
		keyboard->subscribe([&](std::vector<GraphicEngine::Core::Inputs::KeyboardKey> keys) { cameraController->move(keys); });
		mouse->subscribePositionEventHandler([&](float x, float y) { cameraController->rotate(x, y, {}); });
		cameraController->setInitialMousePosition(window->getWidth() / 2, window->getHeight() / 2);

		renderingEngine->init(window->getWidth(), window->getHeight());
		window->addResizeCallbackListener([&](size_t width, size_t height) {renderingEngine->resizeFrameBuffer(width, height); });

		engine = std::shared_ptr<GraphicEngine::Engine>(new GraphicEngine::Engine(window, renderingEngine, keyboard, mouse, cameraController));

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
			glfwWindow->setGLFWWindowProfile(GraphicEngine::GLFW::GLFWWindowProfile::Vulkan);
		}
		return renderingEngine;
	}

	else if (type == "opengl")
	{
		renderingEngine = std::shared_ptr<GraphicEngine::RenderingEngine>(new GraphicEngine::OpenGL::OpenGLRenderingEngine(window));
		auto glfwWindow = std::dynamic_pointer_cast<GraphicEngine::GLFW::WindowGLFW>(window);
		if (glfwWindow != nullptr)
		{
			glfwWindow->setGLFWWindowProfile(GraphicEngine::GLFW::GLFWWindowProfile::OpenGL);
		}
		return renderingEngine;
	}
	throw std::exception("Rendering type not supported!");
}
