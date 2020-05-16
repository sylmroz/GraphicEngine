#include "Application.hpp"
#include "Engine.hpp"

#include "../Platform/Glfw/OpenGL/GlfwOpenGLInjector.hpp"
#include "../Platform/Glfw/Vulkan/GlfwVulkanInjector.hpp"
#include "../Core/Configuration.hpp"

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
		std::shared_ptr<GraphicEngine::Core::Configuration> cfg = std::make_shared<GraphicEngine::Core::Configuration>();
		
		std::string engineType = cfg->getProperty<std::string>("engine");

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

	catch (std::runtime_error& err)
	{
		GraphicEngine::Core::Logger<Application> logger;
		logger.error(__FILE__, __LINE__, __FUNCTION__, err.what());
	}
	
	catch (std::exception ex)
	{
		GraphicEngine::Core::Logger<Application> logger;
		logger.warn(__FILE__, __LINE__, __FUNCTION__, ex.what());
	}
}
