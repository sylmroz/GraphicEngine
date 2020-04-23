#include "Engine.hpp"
#include "../Core/Timer.hpp"

GraphicEngine::Engine::Engine(std::shared_ptr<Common::WindowKeyboardMouse> window,
	std::shared_ptr<RenderingEngine> renderingEngine,
	std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
	std::shared_ptr<Core::Inputs::MouseEventProxy> mouse,
	std::shared_ptr<Common::CameraController> cameraController,
	std::shared_ptr<Core::EventManager> eventManager) :
	m_window(window),
	m_renderingEngine(renderingEngine),
	m_keyboard(keyboard),
	m_mouse(mouse),
	m_cameraController(cameraController),
	m_eventManager(eventManager)

{
}

void GraphicEngine::Engine::initialize()
{
	m_window->init(640, 480);

	m_cameraController->setInitialMousePosition(glm::vec2(m_window->getWidth() / 2, m_window->getHeight() / 2));

	m_keyboard->subscribe([&](std::vector<Core::Inputs::KeyboardKey> keys)
		{
			const auto escKey = std::find(std::begin(keys), std::end(keys), Core::Inputs::KeyboardKey::KEY_ESCAPE);
			if (escKey != std::end(keys))
				shutdown = true;
		}
	);

	m_eventManager->addSubject([&]()
		{
			m_keyboard->notify(m_window->getPressedKeys());
		});

	m_renderingEngine->init(m_window->getWidth(), m_window->getHeight());
	m_window->addResizeCallbackListener([&](size_t width, size_t height) {m_renderingEngine->resizeFrameBuffer(width, height); });
}

void GraphicEngine::Engine::run()
{
	Core::Timer timer;
	timer.start();
	while (!m_window->windowShouldBeClosed() && !shutdown)
	{
		m_renderingEngine->drawFrame();
		m_window->swapBuffer();
		timer.updateTime();
		m_cameraController->setDt(timer.getInterval());
		m_window->poolEvents();
		m_eventManager->call();
	}
	m_renderingEngine->cleanup();
}
