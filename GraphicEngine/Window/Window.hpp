#ifndef GRAPHIC_ENGINE_WINDOW_HPP
#define GRAPHIC_ENGINE_WINDOW_HPP

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

		virtual bool windowShouldBeClosed() 
		{
			return shouldClose; 
		};

	protected:
		virtual void initialize() = 0;
	protected:
		size_t _width{ 0 };
		size_t _height{ 0 };

		bool shouldClose{ false };
	};
}

#endif // !GRAPHIC_ENGINE_WINDOW_HPP