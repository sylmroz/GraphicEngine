#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/NormalDebugGraphicPileline.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLNormalDebugGraphicPipeline : public Engines::Graphic::NormalDebugGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLNormalDebugGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager);

		virtual void draw() override;
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
	};
}
