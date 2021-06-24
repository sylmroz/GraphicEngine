#include "Engine.hpp"

#include "../Platform/Glfw/OpenGL/GlfwOpenGLInjector.hpp"
#include "../Platform/Glfw/Vulkan/GlfwVulkanInjector.hpp"

GraphicEngine::Engine::Engine(std::shared_ptr<Common::WindowKeyboardMouse> window,
	std::shared_ptr<RenderingEngine> renderingEngine,
	std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
	std::shared_ptr<Core::Inputs::MouseEventProxy> mouse,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Common::UI> ui,
	std::shared_ptr<GUI::SettingWindow> settingWindow,
	std::shared_ptr<Core::Timer> timer,
	std::unique_ptr<Core::Logger<Engine>> logger) :
	m_window(window),
	m_renderingEngine(renderingEngine),
	m_keyboard(keyboard),
	m_mouse(mouse),
	m_ui{ ui },
	m_settingWindow{ settingWindow },
	m_eventManager(eventManager),
	m_timer(timer),
	m_logger(std::move(logger))
{
}

void GraphicEngine::Engine::initialize()
{
	m_logger->debug(__FILE__, __LINE__, __FUNCTION__, "Initialize Engine");
	m_window->init();

	m_keyboard->onKeyDown([&](Core::Inputs::KeyboardKey key)
		{
			if (key == Core::Inputs::KeyboardKey::KEY_ESCAPE)
				shutdown = true;
		});

	m_eventManager->addSubject([&]()
		{
			m_keyboard->notify(m_window->getPressedKeys());
		});

	m_window->addResizeCallbackListener([&](size_t width, size_t height) 
		{
			m_renderingEngine->resizeFrameBuffer(width, height); 
		});

	m_renderingEngine->init(m_window->getWidth(), m_window->getHeight());
	
	m_ui->addWidget(m_settingWindow);
	m_ui->initialize();
}

void GraphicEngine::Engine::run()
{
	m_logger->debug(__FILE__, __LINE__, __FUNCTION__, "Run Engine");
	m_timer->start();
	while (!m_window->windowShouldBeClosed() && !shutdown)
	{
		m_renderingEngine->drawFrame();
		m_window->swapBuffer();
		m_timer->updateTime();
		m_window->poolEvents();
		m_eventManager->call();
	}
	m_renderingEngine->cleanup();
	m_ui->shutdown();
}

std::unique_ptr<GraphicEngine::Engine> GraphicEngine::Engine::createEngine(std::string driverType, std::string windowType)
{
	auto createEngine = [](const auto& injector) -> std::unique_ptr<GraphicEngine::Engine>
	{
		return injector.template create<std::unique_ptr<GraphicEngine::Engine>>();
	};

	auto engine = driverType == "vulkan" ?
		createEngine(GraphicEngine::GLFW::injectGlfwVulkanResources()) :
		createEngine(GraphicEngine::GLFW::injectGlfwOpenGlResources());

	return engine;
}

GraphicEngine::Engine::~Engine()
{
	m_logger->debug(__FILE__, __LINE__, __FUNCTION__, "Shutdown Engine");
}
