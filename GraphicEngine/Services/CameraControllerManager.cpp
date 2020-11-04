#include "CameraControllerManager.hpp"

GraphicEngine::Services::CameraControllerManager::CameraControllerManager(std::shared_ptr<Core::Configuration> cfg, std::shared_ptr<Common::WindowKeyboardMouse> window,
	std::shared_ptr<Core::EventManager> eventManager, std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
	std::shared_ptr<Core::Timer> timer, std::shared_ptr<Core::Logger<CameraControllerManager>> logger) :
	m_window(window),
	m_eventManager(eventManager),
	m_keyboard(keyboard),
	m_timer{ timer },
	m_logger{ logger }
{
	auto cameras = cfg->getProperty<std::vector<json>>("cameras");
	for (auto cameraParameters : cameras)
	{
		m_cameraControllers.push_back(std::move(createCameraController(std::move(createCamera(cameraParameters)))));
	}

	auto activeCameraIndex = cfg->getProperty<int>("active camera") - 1;
	m_activeCamera = m_cameraControllers[activeCameraIndex];
	m_activeCamera->activate();

	// TODO - dynamic
	m_keyboard->onKeyUp([&](Core::Inputs::KeyboardKey key)
	{
		if (key == Core::Inputs::KeyboardKey::KEY_1)
		{
			activateCameraController(1);
		}

		if (key == Core::Inputs::KeyboardKey::KEY_2)
		{
			activateCameraController(2);
		}
	});
}

void GraphicEngine::Services::CameraControllerManager::addCameraController(std::unique_ptr<Common::CameraController> cameraController)
{
	// TODO
}

void GraphicEngine::Services::CameraControllerManager::activateCameraController(uint32_t index)
{
	m_activeCamera->deactivate();
	m_activeCamera = m_cameraControllers[index - 1];
	m_activeCamera->activate();
}

std::shared_ptr<GraphicEngine::Common::Camera> GraphicEngine::Services::CameraControllerManager::getActiveCamera()
{
	return m_activeCamera->getCamera();
}

std::shared_ptr<GraphicEngine::Common::Camera> GraphicEngine::Services::CameraControllerManager::createCamera(json parameters)
{
	return std::make_shared<Common::Camera>(std::make_shared<Core::Configuration>(parameters));;
}

std::shared_ptr<GraphicEngine::Common::CameraController> GraphicEngine::Services::CameraControllerManager::createCameraController(std::shared_ptr<Common::Camera> camera)
{
	return std::make_shared<Common::CameraController>(camera, m_window, m_eventManager, m_keyboard, m_timer, m_logger);
}
