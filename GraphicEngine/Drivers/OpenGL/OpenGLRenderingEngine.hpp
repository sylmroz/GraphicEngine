#pragma once

#include "../../Common/RenderingEngine.hpp"
#include "../../Core/Logger.hpp"
#include "../../Engines/Graphic/Shaders/Models/ModelMatrices.hpp"

#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLUniformBuffer.hpp"
#include "OpenGLVertexBuffer.hpp"

// Pipelines
#include "GraphicPipelines/OpenGLWireframeGraphicPipeline.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		OpenGLRenderingEngine(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
			std::shared_ptr<Core::EventManager> eventManager,
			std::shared_ptr<Core::Configuration> cfg,
			std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> logger);
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
		virtual void cleanup() override;

		virtual ~OpenGLRenderingEngine() = default;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_program;
		std::shared_ptr<UniformBuffer<glm::mat4>> m_uniformBufferMatrix;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::Light>> m_lightUniformBuffer;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::ModelMartices>> m_modelMatrix;

		std::vector<std::vector<std::shared_ptr<VertexBuffer<decltype(m_models)::value_type::element_type::vertex_type>>>> m_vertexBuffers;

		std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> m_logger;

		std::unique_ptr<OpenGLWireframeGraphicPipeline> m_wireframeGraphicPipeline;
	};
}
