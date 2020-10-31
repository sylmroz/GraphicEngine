#include "Application.hpp"
#include "Engine.hpp"

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
		GraphicEngine::Core::Configuration cfg;

		std::string engineType = cfg.getProperty<std::string>("engine");

		auto engine = GraphicEngine::Engine::createEngine(engineType, "glfw");

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
