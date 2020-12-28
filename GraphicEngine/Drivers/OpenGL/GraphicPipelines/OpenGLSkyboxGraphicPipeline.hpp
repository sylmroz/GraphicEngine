#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/SkyboxGraphicPipeline.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLSkyboxGraphicPipeline : public Engines::Graphic::SkyboxGraphicPipeline<VertexBuffer, TextureCube>
	{
	public:
		OpenGLSkyboxGraphicPipeline(const std::string& basePath);

		virtual void draw() override;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
	};
}