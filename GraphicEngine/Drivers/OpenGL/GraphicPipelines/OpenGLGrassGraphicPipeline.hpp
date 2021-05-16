#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/GrassGraphicPipeline.hpp"
#include "../OpenGLTexture.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLGrassGraphicPipeline : public Engines::Graphic::GrassGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLGrassGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
			std::shared_ptr<Texture> directionalLighttShadowMap,
			std::shared_ptr<Texture> spotLightShadowMaps,
			std::shared_ptr<Texture> pointLightShadowMaps);

		virtual void draw() override;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;

		std::shared_ptr<Texture> m_directionalLighttShadowMap;
		std::shared_ptr<Texture> m_spotLightShadowMaps;
		std::shared_ptr<Texture> m_pointLightShadowMaps;
	};
}
