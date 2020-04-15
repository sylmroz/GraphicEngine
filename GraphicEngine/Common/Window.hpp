#ifndef GRAPHIC_ENGINE_WINDOW_HPP
#define GRAPHIC_ENGINE_WINDOW_HPP

#include "../Core/Subject.hpp"
#include "../Core/Input/Mouse/MouseEventProxy.hpp"

#include <exception>
#include <utility>

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

		virtual std::pair<uint32_t, uint32_t> getFrameBufferSize() = 0;

		virtual void addResizeCallbackListener(std::function<void(size_t, size_t)> resizeListener)
		{
			_resizeSubject.subscribe(std::move(resizeListener));
		}

		virtual bool windowShouldBeClosed() { return _shouldClose; }

		size_t getWidth() { return _width; }
		void setWidth(size_t width) { _width = width; }

		size_t getHeight() { return _height; }
		void setHeight(size_t height) { _height = height; }

		virtual void initialize() = 0;

	protected:
		size_t _width{ 0 };
		size_t _height{ 0 };

		bool _shouldClose{ false };

		GraphicEngine::Core::Subject<size_t,size_t> _resizeSubject;
	};
}

#endif // !GRAPHIC_ENGINE_WINDOW_HPP