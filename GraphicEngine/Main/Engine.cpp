#include "Engine.hpp"
#include "../Core/Timer.hpp"

GraphicEngine::Engine::Engine(std::shared_ptr<Window> window,
	std::shared_ptr<RenderingEngine> renderingEngine,
	std::shared_ptr<Core::Inputs::Keyboard> keyboard,
	std::shared_ptr<Core::Inputs::Mouse> mouse,
	std::shared_ptr<Common::CameraController> cameraController):
	_window(window),
	_renderingEngine(renderingEngine),
	_keyboard(keyboard),
	_mouse(mouse),
	_cameraController(cameraController)
{
	_keyboard->subscribe([&](std::vector<Core::Inputs::KeyboardKey> keys)
		{
			auto escKey = std::find(std::begin(keys), std::end(keys), Core::Inputs::KeyboardKey::KEY_ESCAPE);
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
