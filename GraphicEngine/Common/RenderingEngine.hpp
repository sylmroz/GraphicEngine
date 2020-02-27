#ifndef GRAPHIC_ENGINE_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_RENDERING_ENGINE_HPP

#include "Window.hpp"

namespace GraphicEngine
{
	class RenderingEngine
	{
	public:
		RenderingEngine(std::shared_ptr<Window> window) :
			_window(window) {}

		virtual bool drawFrame() = 0;
		virtual void init(size_t width, size_t height) = 0;
		virtual void resizeFrameBuffer(size_t width, size_t height) = 0;
		virtual void cleanup() = 0;
	protected:
		std::shared_ptr<Window> _window;
	};
}

#endif // !GRAPHIC_ENGINE_RENDERING_ENGINE_HPP
