#include "Application.hpp"

#include "../Common/Window.hpp"
#include "../Drivers/Vulkan/VulkanRenderingEngine.hpp"
#include "../Drivers/OpenGL/OpenGLRenderingEngine.hpp"
#include "../Platform/Glfw/GlfwWindow.hpp"
#include "../Common/RenderingEngine.hpp"

#include <exception>
#include <iostream>
#include <utility>

#include "../Common/WindowKeyboardMouse.hpp"

using namespace GraphicEngine::Core::Inputs;

Application::Application(int argc, char** argv)
{
	// TODO - parse argc and argv to get parameters

}

void Application::exec()
{
	try
	{
		keyboard = std::shared_ptr<KeyboardEventProxy>(new KeyboardEventProxy);
		mouse = std::shared_ptr<MouseEventProxy>(new MouseEventProxy);
		
		auto window = windowFactory("glfw");
		auto renderingEngine = renderingEngineFactory("vulkan", window);

		window->init(640, 480);

		GraphicEngine::Common::PerspectiveParameters perspectiveParameters;
		perspectiveParameters.aspectRatio = static_cast<float>(window->getWidth()) / static_cast<float>(window->getHeight());
		perspectiveParameters.fov = 45;
		perspectiveParameters.zFar = 1000;
		perspectiveParameters.zNear = 0.01;
		camera = std::make_shared<GraphicEngine::Common::Camera>(perspectiveParameters);

		cameraController = std::shared_ptr<GraphicEngine::Common::CameraController>(new GraphicEngine::Common::CameraController(camera));
		cameraController->setInitialMousePosition(glm::vec2(window->getWidth() / 2, window->getHeight() / 2));

		eventManager = std::shared_ptr<GraphicEngine::Core::EventManager>(new GraphicEngine::Core::EventManager);

		eventManager->addSubject([&]()
			{
				cameraController->updateCamera(
					window->getCursorPosition(),
					window->getScrollValue(),
					window->getPressedButtons(),
					window->getPressedKeys());
			});
		eventManager->addSubject([&](glm::vec2 pos)
			{
				window->setCursorPosition(pos);
			}, glm::vec2(window->getWidth() / 2, window->getHeight() / 2));
		eventManager->addSubject([&]()
			{
				keyboard->notify(window->getPressedKeys());
			});


		renderingEngine->init(window->getWidth(), window->getHeight());
		window->addResizeCallbackListener([&](size_t width, size_t height) {renderingEngine->resizeFrameBuffer(width, height); });
		renderingEngine->setCamera(camera);
		//engine = std::shared_ptr<GraphicEngine::Engine>(new GraphicEngine::Engine(window, renderingEngine, keyboard, mouse, cameraController, eventManager));
		engine = std::shared_ptr<GraphicEngine::Engine>(new GraphicEngine::Engine(window, renderingEngine, keyboard, mouse, cameraController, eventManager));
		engine->run();
	}

	catch (std::exception ex)
	{
		std::cout << ex.what() << "\n";
	}
}

std::shared_ptr<GraphicEngine::Common::WindowKeyboardMouse> Application::windowFactory(const std::string& windowType)
{
	if (windowType == "glfw")
		return std::shared_ptr<GraphicEngine::Common::WindowKeyboardMouse>(new GraphicEngine::GLFW::GlfwWindow);

	throw std::exception("Window type not supported!");
}

std::shared_ptr<GraphicEngine::RenderingEngine> Application::renderingEngineFactory(std::string type, std::shared_ptr<GraphicEngine::Common::WindowKeyboardMouse>& window)
{
	std::shared_ptr<GraphicEngine::RenderingEngine> renderingEngine;
	if (type == "vulkan")
	{
		renderingEngine = std::shared_ptr<GraphicEngine::RenderingEngine>(new GraphicEngine::Vulkan::VulkanRenderingEngine(window, camera, eventManager));
		auto glfwWindow = std::dynamic_pointer_cast<GraphicEngine::GLFW::GlfwWindow>(window);
		if (glfwWindow != nullptr)
		{
			glfwWindow->setGLFWWindowProfile(GraphicEngine::GLFW::GLFWWindowProfile::Vulkan);
		}
		return renderingEngine;
	}

	else if (type == "opengl")
	{
		renderingEngine = std::shared_ptr<GraphicEngine::RenderingEngine>(new GraphicEngine::OpenGL::OpenGLRenderingEngine(window, camera, eventManager));
		auto glfwWindow = std::dynamic_pointer_cast<GraphicEngine::GLFW::GlfwWindow>(window);
		if (glfwWindow != nullptr)
		{
			glfwWindow->setGLFWWindowProfile(GraphicEngine::GLFW::GLFWWindowProfile::OpenGL);
		}
		return renderingEngine;
	}
	throw std::exception("Rendering type not supported!");
}
