#ifndef GRAPHIC_ENGINE_ENGINE_HPP
#define GRAPHIC_ENGINE_ENGINE_HPP

#include "../Common/Camera.hpp"
#include "../Common/Window.hpp"
#include "../Common/RenderingEngine.hpp"
#include "../Core/Timer.hpp"
#include "../Core/Input/Keyboard/KeyboardEventProxy.hpp"
#include "../Core/Input/Mouse/MouseEventProxy.hpp"

#include <memory>

namespace GraphicEngine
{
	class Engine
	{
	public:

		Engine(std::shared_ptr<Window> window,
			std::shared_ptr<RenderingEngine> renderingEngine,
			std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
			std::shared_ptr<Core::Inputs::MouseEventProxy> mouse,
			std::shared_ptr<Common::CameraController> cameraController);

		void setWindow(std::shared_ptr<Window> window) { _window = window; };
		void setRenderingEngine(std::shared_ptr<RenderingEngine> renderingEngine) { _renderingEngine = renderingEngine; };

		void run();
	protected:
	private:
		std::shared_ptr<Window> _window;
		std::shared_ptr<RenderingEngine> _renderingEngine;
		std::shared_ptr<Core::Inputs::KeyboardEventProxy> _keyboard;
		std::shared_ptr<Core::Inputs::MouseEventProxy> _mouse;
		std::shared_ptr<Common::CameraController> _cameraController;

		bool shutdown = false;
	};
}

#endif // !GRAPHIC_ENGINE_ENGINE_HPP

