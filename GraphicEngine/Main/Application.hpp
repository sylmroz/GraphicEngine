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
	std::shared_ptr<GraphicEngine::Window> windowFactory(std::string windowType);
	std::shared_ptr<GraphicEngine::RenderingEngine> renderingEngineFactory(std::string type, std::shared_ptr<GraphicEngine::Window> window);

private:
	std::shared_ptr<GraphicEngine::Engine> engine;
	std::shared_ptr<GraphicEngine::Core::Inputs::Keyboard> keyboard;
	std::shared_ptr<GraphicEngine::Core::Inputs::Mouse> mouse;
};

#endif // !APPLICATION_HPP

