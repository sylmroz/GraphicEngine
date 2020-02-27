#ifndef GRAPHIC_ENGINE_ENGINE_HPP
#define GRAPHIC_ENGINE_ENGINE_HPP

#include "../Common/Window.hpp"
#include "../Common/RenderingEngine.hpp"
#include "../Core/Input/Keyboard/Keyboard.hpp"
#include "../Core/Input/Mouse/Mouse.hpp"

#include <memory>

namespace GraphicEngine
{
	class Engine
	{
	public:

		Engine(std::shared_ptr<Window> window,
			std::shared_ptr<RenderingEngine> renderingEngine,
			std::shared_ptr<HID::Keyboard> keyboard,
			std::shared_ptr<HID::Mouse> mouse);

		void setWindow(std::shared_ptr<Window> window) { _window = window; };
		void setRenderingEngine(std::shared_ptr<RenderingEngine> renderingEngine) { _renderingEngine = renderingEngine; };

		void run();
	protected:
	private:
		std::shared_ptr<Window> _window;
		std::shared_ptr<RenderingEngine> _renderingEngine;
		std::shared_ptr<HID::Keyboard> _keyboard;
		std::shared_ptr<HID::Mouse> _mouse;

		bool shutdown = false;
	};
}

#endif // !GRAPHIC_ENGINE_ENGINE_HPP

