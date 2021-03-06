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
		OpenGLSolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
			std::shared_ptr<Texture> depthTexture,
			std::shared_ptr<Texture> spotLightShadowMaps,
			std::shared_ptr<Texture> pointLightShadowMaps);

		virtual void draw() override;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
		GLuint m_diffuseOnlyIndex;
		std::shared_ptr<Texture> m_directionalLightDepthTexture;
		std::shared_ptr<Texture> m_spotLightShadowMaps;
		std::shared_ptr<Texture> m_pointLightShadowMaps;
	};
}