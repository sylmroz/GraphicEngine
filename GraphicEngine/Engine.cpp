#include "Engine.hpp"

GraphicEngine::Engine::Engine(std::shared_ptr<Window> window, std::shared_ptr<RenderingEngine> renderingEngine):
	_window(window),
	_renderingEngine(renderingEngine)
{
}

void GraphicEngine::Engine::run()
{
	_window->init(640, 480);
	_renderingEngine->init();

	while (!_window->windowShouldBeClosed() && !shutdown)
	{
		_renderingEngine->drawFrame();
		_window->swapBuffer();
		_window->poolEvents();
	}
}
