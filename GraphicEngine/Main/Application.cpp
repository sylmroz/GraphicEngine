#include "Application.hpp"

#include <any>


#include "../Common/Window.hpp"
#include "../Drivers/Vulkan/VulkanRenderingEngine.hpp"
#include "../Drivers/OpenGL/OpenGLRenderingEngine.hpp"
#include "../Platform/Glfw/Vulkan/GlfwVulkanWindow.hpp"
#include "../Platform/Glfw/OpenGL/GlfwOpenGLWindow.hpp"
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

		auto _windowController = std::make_shared<GraphicEngine::GLFW::GlfwVulkanWindow>();
		auto vulkanWindowContext = std::make_shared<GraphicEngine::GLFW::GlfwVulkanWindowContext>(_windowController);
		

		_windowController->init(640, 480);

		GraphicEngine::Common::PerspectiveParameters perspectiveParameters;
		perspectiveParameters.aspectRatio = static_cast<float>(_windowController->getWidth()) / static_cast<float>(_windowController->getHeight());
		perspectiveParameters.fov = 45;
		perspectiveParameters.zFar = 1000;
		perspectiveParameters.zNear = 0.01;
		camera = std::make_shared<GraphicEngine::Common::Camera>(perspectiveParameters);

		cameraController = std::shared_ptr<GraphicEngine::Common::CameraController>(new GraphicEngine::Common::CameraController(camera));
		cameraController->setInitialMousePosition(glm::vec2(_windowController->getWidth() / 2, _windowController->getHeight() / 2));

		eventManager = std::shared_ptr<GraphicEngine::Core::EventManager>(new GraphicEngine::Core::EventManager);

		eventManager->addSubject([&]()
			{
				cameraController->updateCamera(
					_windowController->getCursorPosition(),
					_windowController->getScrollValue(),
					_windowController->getPressedButtons(),
					_windowController->getPressedKeys());
			});
		eventManager->addSubject([&]()
			{
				_windowController->setCursorPosition(glm::vec2(_windowController->getWidth() / 2, _windowController->getHeight() / 2));
			});
		eventManager->addSubject([&]()
			{
				keyboard->notify(_windowController->getPressedKeys());
			});

		auto renderingEngine = std::make_shared<GraphicEngine::Vulkan::VulkanRenderingEngine>(vulkanWindowContext, camera, eventManager);
		renderingEngine->init(_windowController->getWidth(), _windowController->getHeight());
		_windowController->addResizeCallbackListener([&](size_t width, size_t height) {renderingEngine->resizeFrameBuffer(width, height); });

		engine = std::shared_ptr<GraphicEngine::Engine>(new GraphicEngine::Engine(_windowController, renderingEngine, keyboard, mouse, cameraController, eventManager));
		engine->run();
	}

	catch (std::exception ex)
	{
		std::cout << ex.what() << "\n";
	}
}
