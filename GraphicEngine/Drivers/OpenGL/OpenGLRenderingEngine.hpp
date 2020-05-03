#pragma once

#include "../../Common/RenderingEngine.hpp"

#include "../../Core/Logger.hpp"

#include "OpenGLHelper.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLUniformBuffer.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		OpenGLRenderingEngine(std::shared_ptr<Common::Camera> camera, 
			std::shared_ptr<Core::EventManager> eventManager,
			std::unique_ptr<GameEngine::Core::Logger<OpenGLRenderingEngine>> logger);
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
		virtual void cleanup() override;

		virtual ~OpenGLRenderingEngine() = default;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_program;
		std::shared_ptr<UniformBuffer<glm::mat4>> m_uniformBufferMatrix;
		std::unique_ptr<VertexBuffer<Common::VertexPC>> m_vertexBuffer;

		std::unique_ptr<GameEngine::Core::Logger<OpenGLRenderingEngine>> m_logger;
	};
}
