#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/WireframeGraphicPipeline.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLWireframeGraphicPipeline : public Engines::Graphic::WireframeGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLWireframeGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager);

		virtual void draw() override;
	private:
		std::unique_ptr<OpenGLShaderProgram> m_shaderProgram;
	};
}
