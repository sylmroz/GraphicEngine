#include "Application.hpp"
#include "Engine.hpp"

#include "../Platform/Glfw/OpenGL/GlfwOpenGLInjector.hpp"
#include "../Platform/Glfw/Vulkan/GlfwVulkanInjector.hpp"

#include <exception>
#include <iostream>

Application::Application(int argc, char** argv)
{
	// TODO - parse argc and argv to get parameters

}

void Application::exec()
{
	try
	{
		std::string engineType = "opengl";

		auto createEngine = [](const auto& injector) -> std::unique_ptr<GraphicEngine::Engine>
		{
			return injector.template create<std::unique_ptr<GraphicEngine::Engine>>();
		};

		auto engine = engineType == "vulkan" ?
			createEngine(GraphicEngine::GLFW::injectGlfwVulkanResources()) :
			createEngine(GraphicEngine::GLFW::injectGlfwOpenGlResources());
		
		engine->initialize();
		engine->run();
	}

	catch (std::exception ex)
	{
		std::cout << ex.what() << "\n";
	}
}
