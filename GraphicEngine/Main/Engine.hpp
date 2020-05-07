#pragma once

#include "../Common/WindowKeyboardMouse.hpp"

#include "../Common/Camera.hpp"
#include "../Common/RenderingEngine.hpp"
#include "../Core/EventManager.hpp"
#include "../Core/Input/Keyboard/KeyboardEventProxy.hpp"
#include "../Core/Input/Mouse/MouseEventProxy.hpp"
#include "../Core/Logger.hpp"

#include <memory>


namespace GraphicEngine
{
	class Engine
	{
	public:

		Engine(std::shared_ptr<Common::WindowKeyboardMouse> window,
			std::shared_ptr<RenderingEngine> renderingEngine,
			std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
			std::shared_ptr<Core::Inputs::MouseEventProxy> mouse,
			std::shared_ptr<Common::CameraController> cameraController,
			std::shared_ptr<Core::EventManager> eventManager,
			std::unique_ptr<Core::Logger<Engine>> logger);

		void initialize();
		void run();

		~Engine();
	protected:
	private:
		std::shared_ptr<Common::WindowKeyboardMouse> m_window;
		std::shared_ptr<RenderingEngine> m_renderingEngine;
		std::shared_ptr<Core::Inputs::KeyboardEventProxy> m_keyboard;
		std::shared_ptr<Core::Inputs::MouseEventProxy> m_mouse;
		std::shared_ptr<Common::CameraController> m_cameraController;
		std::shared_ptr<Core::EventManager> m_eventManager;
		std::unique_ptr<Core::Logger<Engine>> m_logger;

		bool shutdown = false;
	};
}
