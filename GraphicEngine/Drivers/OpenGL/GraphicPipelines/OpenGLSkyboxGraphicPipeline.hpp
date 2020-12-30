#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/SkyboxGraphicPipeline.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLSkyboxGraphicPipeline : public Engines::Graphic::SkyboxGraphicPipeline<OpenGLSkyboxGraphicPipeline, VertexBuffer, TextureCube>
	{
	public:
		OpenGLSkyboxGraphicPipeline(const std::string& basePath);

		virtual void draw() override;
		std::unique_ptr<VertexBuffer<Common::VertexP>> produceVertexBuffer(std::vector<Common::VertexP> skyBoxVertices);
		std::shared_ptr<TextureCube> produceTextureCube(std::array<std::string, 6> faces);
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
	};
}