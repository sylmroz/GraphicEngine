#ifndef GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP

#include "RenderingEngine.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		OpenGLRenderingEngine(std::shared_ptr<Window> window);
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
		virtual void cleanup() override;
	private:

	};
}

#endif // !GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP