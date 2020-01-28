#include "Engine.hpp"

GraphicEngine::Engine::Engine(std::shared_ptr<Window> window,
	std::shared_ptr<RenderingEngine> renderingEngine,
	std::shared_ptr<HID::Keyboard> keyboard,
	std::shared_ptr<HID::Mouse> mouse):
	_window(window),
	_renderingEngine(renderingEngine),
	_keyboard(keyboard),
	_mouse(mouse)
{
	_keyboard->subscribe([&](std::vector<HID::Key> keys)
		{
			auto escKey = std::find(std::begin(keys), std::end(keys), HID::Key::KEY_ESCAPE);
			if (escKey != std::end(keys))
				shutdown = true;
		}
	);
}

void GraphicEngine::Engine::run()
{
	while (!_window->windowShouldBeClosed() && !shutdown)
	{
		_renderingEngine->drawFrame();
		_window->swapBuffer();
		_window->poolEvents();
	}
}
