#include "Engine.hpp"
#include "../Core/Timer.hpp"

#include <utility>

GraphicEngine::Engine::Engine(std::shared_ptr<Common::WindowKeyboardMouse> window,
	std::shared_ptr<RenderingEngine> renderingEngine,
	std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
	std::shared_ptr<Core::Inputs::MouseEventProxy> mouse,
	std::shared_ptr<Common::CameraController> cameraController,
	std::shared_ptr<Core::EventManager> eventManager):
	_window(window),
	_renderingEngine(renderingEngine),
	_keyboard(keyboard),
	_mouse(mouse),
	_cameraController(cameraController),
	_eventManager(eventManager)

{
}

void GraphicEngine::Engine::initialize()
{
	_window->init(640, 480);

	_cameraController->setInitialMousePosition(glm::vec2(_window->getWidth() / 2, _window->getHeight() / 2));

	_keyboard->subscribe([&](std::vector<Core::Inputs::KeyboardKey> keys)
		{
			const auto escKey = std::find(std::begin(keys), std::end(keys), Core::Inputs::KeyboardKey::KEY_ESCAPE);
			if (escKey != std::end(keys))
				shutdown = true;
		}
	);

	_eventManager = std::shared_ptr<Core::EventManager>(new Core::EventManager);

	_eventManager->addSubject([&]()
		{
			_cameraController->updateCamera(
				_window->getCursorPosition(),
				_window->getScrollValue(),
				_window->getPressedButtons(),
				_window->getPressedKeys());
		});
	_eventManager->addSubject([&]()
		{
			_window->setCursorPosition(glm::vec2(_window->getWidth() / 2, _window->getHeight() / 2));
		});
	_eventManager->addSubject([&]()
		{
			_keyboard->notify(_window->getPressedKeys());
		});

	_renderingEngine->init(_window->getWidth(), _window->getHeight());
	_window->addResizeCallbackListener([&](size_t width, size_t height) {_renderingEngine->resizeFrameBuffer(width, height); });
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
		_eventManager->call();
	}
	_renderingEngine->cleanup();
}
