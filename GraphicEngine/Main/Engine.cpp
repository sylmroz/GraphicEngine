#include "Engine.hpp"
#include "../Core/Timer.hpp"

#include <utility>

GraphicEngine::Engine::Engine(std::shared_ptr<Window> window,
	std::shared_ptr<RenderingEngine> renderingEngine,
	std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
	std::shared_ptr<Core::Inputs::MouseEventProxy> mouse,
	std::shared_ptr<Common::CameraController> cameraController):
	_window(std::move(window)),
	_renderingEngine(std::move(renderingEngine)),
	_keyboard(std::move(keyboard)),
	_mouse(std::move(mouse)),
	_cameraController(std::move(cameraController))
{
	_keyboard->subscribe([&](std::vector<Core::Inputs::KeyboardKey> keys)
		{
			const auto escKey = std::find(std::begin(keys), std::end(keys), Core::Inputs::KeyboardKey::KEY_ESCAPE);
			if (escKey != std::end(keys))
				shutdown = true;
		}
	);
}

void GraphicEngine::Engine::run()
{
	Core::Timer timer;
	timer.start();
	while (!_window->windowShouldBeClosed() && !shutdown)
	{
		_renderingEngine->drawFrame();
		_window->swapBuffer();
		timer.updateTime();
		_cameraController->setDt(timer.getInterval());
		_window->poolEvents();
	}
	_renderingEngine->cleanup();
}
