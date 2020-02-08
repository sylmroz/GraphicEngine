#ifndef GRAPHIC_ENGINE_WINDOW_HPP
#define GRAPHIC_ENGINE_WINDOW_HPP

#include "../Utils/Subject.hpp"
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
				_resizeSubject.subscribe([&](size_t width, size_t height) 
					{
						setWidth(width);
						setHeight(height);
					}
				);
			}

			catch (std::exception e)
			{
				throw e;
			}
		}

		virtual void swapBuffer() = 0;

		virtual void poolEvents() = 0;

		virtual std::vector<std::string> getRequiredExtensions() = 0;

		virtual std::pair<uint32_t, uint32_t> getFrameBufferSize() = 0;

		virtual void registerKeyboard(std::shared_ptr<GraphicEngine::HID::Keyboard> keyboard) { _keyboard = keyboard; };

		virtual void registerMouse(std::shared_ptr<GraphicEngine::HID::Mouse> mouse) { _mouse = mouse; };

		virtual void addResizeCallbackListener(std::function<void(size_t, size_t)> resizeListener)
		{
			_resizeSubject.subscribe(resizeListener);
		}

		virtual bool windowShouldBeClosed() { return shouldClose; }

		size_t getWidth() { return _width; }
		void setWidth(size_t width) { _width = width; }

		size_t getHeight() { return _height; }
		void setHeight(size_t height) { _height = height; }

	protected:
		virtual void initialize() = 0;
	protected:
		size_t _width{ 0 };
		size_t _height{ 0 };

		bool shouldClose{ false };

		std::shared_ptr<GraphicEngine::HID::Keyboard> _keyboard;
		std::shared_ptr<GraphicEngine::HID::Mouse> _mouse;

		GraphicEngine::Utils::Subject<size_t,size_t> _resizeSubject;
	};
}

#endif // !GRAPHIC_ENGINE_WINDOW_HPP