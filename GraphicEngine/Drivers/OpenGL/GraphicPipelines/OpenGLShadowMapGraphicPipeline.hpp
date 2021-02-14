#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/ShadowMapGraphicPipeline.hpp"
#include "../OpenGLTexture.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLShadowMapGraphicPipeline : public Engines::Graphic::ShadowMapGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLShadowMapGraphicPipeline(std::shared_ptr<Services::LightManager> cameraControllerManager, std::shared_ptr<Texture> depthTexture);

		virtual void draw() override;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
		std::shared_ptr<UniformBufferArray<Engines::Graphic::Shaders::LightSpaceMatrixArray>> m_lightSpaceMatrix;
		GLuint dephMapFBO;
		std::shared_ptr<Texture> m_depthTexture;
	};
}
