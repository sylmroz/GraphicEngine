#pragma once

#include "Camera.hpp"
#include "../Core/Timer.hpp"

namespace GraphicEngine::Common
{
	class CameraController
	{
	public:
		CameraController(std::shared_ptr<Camera> camera, std::shared_ptr<WindowKeyboardMouse> window,
			std::shared_ptr<Core::EventManager> eventManager, std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
			std::shared_ptr<Core::Timer> timer, std::shared_ptr<Core::Logger<Services::CameraControllerManager>> logger);

		void setCameraType(CameraType cameraType);

		void setDt(float dt);

		void setInitialMousePosition(glm::vec2 pos);

		bool isCameraActive();

		void updateCamera(glm::vec2 cursorPosition, glm::vec2 scrollPosition,
			const std::vector<Core::Inputs::MouseButton>& buttons,
			std::vector<Core::Inputs::KeyboardKey> keys);

		void rotate(glm::vec2 pos, const std::vector<GraphicEngine::Core::Inputs::MouseButton>& buttons);

		void move(std::vector<Core::Inputs::KeyboardKey> keys);

		void zoom(double offset);

		std::shared_ptr<Camera> getCamera();

		void switchCameraType();

		void activate();
		void deactivate();

	private:
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<WindowKeyboardMouse> m_window;
		std::shared_ptr<Core::EventManager> m_eventManager;
		std::shared_ptr<Core::Inputs::KeyboardEventProxy> m_keyboard;
		std::shared_ptr<Core::Timer> m_timer;
		std::shared_ptr<Core::Logger<Services::CameraControllerManager>> m_logger;

		float m_dt{ 0.0f };
		glm::vec2 m_prevMousePosition;
		Core::Inputs::MouseButton m_rotateButton = Core::Inputs::MouseButton::buttonLeft;
		

		bool isActive{ false };
	};
}