#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/SolidColorGraphicPipeline.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLSolidColorGraphicPipeline : public Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLSolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager);

		virtual void draw() override;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
	};
}