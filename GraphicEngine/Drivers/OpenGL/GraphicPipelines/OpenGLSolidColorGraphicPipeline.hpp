#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/SolidColorGraphicPipeline.hpp"
#include "../../../Services/RenderingOptionsManager.hpp"
#include "../OpenGLTexture.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLSolidColorGraphicPipeline : public Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLSolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager, std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager, std::shared_ptr<Texture> depthTexture, std::shared_ptr<Texture> spotLightShadowMaps);

		virtual void draw() override;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
		GLuint m_diffuseOnlyIndex;
		std::shared_ptr<Texture> m_depthTexture;
		std::shared_ptr<Texture> m_spotLightShadowMaps;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::RenderingOptions>> m_renderingOptionsUniformBuffer;
		std::shared_ptr<Services::RenderingOptionsManager> m_renderingOptionsManager;
	};
}