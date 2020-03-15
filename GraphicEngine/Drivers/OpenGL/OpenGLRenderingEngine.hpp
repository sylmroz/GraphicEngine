#ifndef GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP

#include "../../Common/RenderingEngine.hpp"

#include "OpenGLHelper.hpp"
#include "OpenGLShader.hpp"

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
		std::unique_ptr<OpenGLShaderProgram> program;
		std::unique_ptr<VertexBuffer<Common::VertexPC>> vertexBuffer;
	};
}

#endif // !GRAPHIC_ENGINE_OPENGL_RENDERING_ENGINE_HPP