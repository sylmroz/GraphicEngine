#ifndef GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP

#include "RenderingEngine.hpp"

namespace GraphicEngine::Vulkan
{
	class OpenGLRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		virtual bool drawFrame();
		virtual void init();
	private:

	};
}

#endif // !GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP