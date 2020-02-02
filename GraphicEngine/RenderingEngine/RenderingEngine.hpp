#ifndef GRAPHIC_ENGINE_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_RENDERING_ENGINE_HPP

namespace GraphicEngine
{
	class RenderingEngine
	{
	public:
		virtual bool drawFrame() = 0;
		virtual void init(size_t width, size_t height) = 0;
		virtual void resizeFrameBuffer(size_t width, size_t height) = 0;
	private:
		
	};
}

#endif // !GRAPHIC_ENGINE_RENDERING_ENGINE_HPP
