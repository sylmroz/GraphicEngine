#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/GrassGraphicPipeline.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLGrassGraphicPipeline : public Engines::Graphic::GrassGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLGrassGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager);

		virtual void draw() override;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
	};
}
