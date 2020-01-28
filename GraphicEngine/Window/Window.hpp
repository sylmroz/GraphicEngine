#ifndef GRAPHIC_ENGINE_WINDOW_HPP
#define GRAPHIC_ENGINE_WINDOW_HPP

#include "../HID/Keyboard/Keyboard.hpp"
#include "../HID/Mouse/Mouse.hpp"

#include <exception>

namespace GraphicEngine
{
	class Window
	{
	public:
		virtual void init(size_t width, size_t height)
		{
			_width = width;
			_height = height;
			try
			{
				initialize();
			}

			catch (std::exception e)
			{
				throw e;
			}
		}

		virtual void swapBuffer() = 0;

		virtual void poolEvents() = 0;

		virtual void registerKeyboard(std::shared_ptr<GraphicEngine::HID::Keyboard> keyboard) { _keyboard = keyboard; };

		virtual void registerMouse(std::shared_ptr<GraphicEngine::HID::Mouse> mouse) { _mouse = mouse; };

		virtual bool windowShouldBeClosed() { return shouldClose; };

	protected:
		virtual void initialize() = 0;
	protected:
		size_t _width{ 0 };
		size_t _height{ 0 };

		bool shouldClose{ false };

		std::shared_ptr<GraphicEngine::HID::Keyboard> _keyboard;
		std::shared_ptr<GraphicEngine::HID::Mouse> _mouse;
	};
}

#endif // !GRAPHIC_ENGINE_WINDOW_HPP