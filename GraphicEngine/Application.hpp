#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Engine.hpp"

#include <string>

class Application
{
public:
	Application(int argc, char** argv);

	void exec();

private:
	std::shared_ptr<GraphicEngine::Engine> engine;
	std::shared_ptr<GraphicEngine::Window> windowFactory(std::string type);
	std::shared_ptr < GraphicEngine::RenderingEngine> renderingEngineFactory(std::string type, std::shared_ptr<GraphicEngine::Window> window);
};

#endif // !APPLICATION_HPP

