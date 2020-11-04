#pragma once

#include "../Common/CameraController.hpp"
#include "../Common/WindowKeyboardMouse.hpp"

namespace GraphicEngine::Services
{
	class CameraControllerManager
	{
	public:
		CameraControllerManager(std::shared_ptr<Core::Configuration> cfg, std::shared_ptr<Common::WindowKeyboardMouse> window,
			std::shared_ptr<Core::EventManager> eventManager, std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
			std::shared_ptr<Core::Timer> timer, std::shared_ptr<Core::Logger<CameraControllerManager>> logger);

		void addCameraController(std::unique_ptr<Common::CameraController> cameraController);

		void activateCameraController(uint32_t index);

		std::shared_ptr<Common::Camera> getActiveCamera();

	private:
		std::shared_ptr<Common::Camera> createCamera(json parameters);
		std::shared_ptr<Common::CameraController> createCameraController(std::shared_ptr<Common::Camera> camera);

	private:
		std::vector<std::shared_ptr<Common::CameraController>> m_cameraControllers;
		std::shared_ptr<Common::WindowKeyboardMouse> m_window;
		std::shared_ptr<Core::EventManager> m_eventManager;
		std::shared_ptr<Core::Inputs::KeyboardEventProxy> m_keyboard;
		std::shared_ptr<Common::CameraController> m_activeCamera;
		std::shared_ptr<Core::Timer> m_timer;
		std::shared_ptr<Core::Logger<CameraControllerManager>> m_logger;
	};
}