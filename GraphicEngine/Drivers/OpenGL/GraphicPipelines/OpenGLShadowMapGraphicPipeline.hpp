#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/ShadowMapGraphicPipeline.hpp"
#include "../OpenGLTexture.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLShadowMapGraphicPipeline : public Engines::Graphic::ShadowMapGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLShadowMapGraphicPipeline(std::shared_ptr<Texture> depthTexture, Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray);

		virtual void draw() override;

		void updateLights(Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray);
		void updateLight(Engines::Graphic::Shaders::LightSpaceMatrix lightSpaceMatrix, uint32_t index);
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
		std::shared_ptr<UniformBufferArray<Engines::Graphic::Shaders::LightSpaceMatrixArray>> m_lightSpaceMatrixArrayUniform;
		GLuint dephMapFBO;
		std::shared_ptr<Texture> m_depthTexture;

		std::string m_shaderTemplate;
		std::vector<std::shared_ptr<OpenGLShader>> m_shaders;
		Engines::Graphic::Shaders::LightSpaceMatrixArray m_lightSpaceMatrixArray;
	};
}
