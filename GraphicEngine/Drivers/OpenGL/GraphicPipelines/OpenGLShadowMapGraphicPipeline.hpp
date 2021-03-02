#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Pipelines/ShadowMapGraphicPipeline.hpp"
#include "../OpenGLTexture.hpp"
#include "../../../Engines/Graphic/Shaders/Models/LightPositionFarPlane.hpp"

namespace GraphicEngine::OpenGL
{
	enum class LightTypeShadow
	{
		directional,
		point,
		spot
	};

	class OpenGLShadowMapGraphicPipeline : public Engines::Graphic::ShadowMapGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>
	{
	public:
		OpenGLShadowMapGraphicPipeline(std::shared_ptr<Texture> depthTexture, Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray, LightTypeShadow type = LightTypeShadow::directional, Engines::Graphic::Shaders::LightPositionFarPlaneArray lightPositionFarPlaneArray = {});

		virtual void draw() override;

		void updateLights(Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray, Engines::Graphic::Shaders::LightPositionFarPlaneArray lightPositionFarPlaneArray = {});
		void updateLight(Engines::Graphic::Shaders::LightSpaceMatrix lightSpaceMatrix, uint32_t index, Engines::Graphic::Shaders::LightPositionFarPlane lightPositionFarPlane = {});

		uint32_t getOffset();
		std::string getShaderTypePlaceholder();
	private:
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
		std::shared_ptr<UniformBufferArray<Engines::Graphic::Shaders::LightSpaceMatrixArray>> m_lightSpaceMatrixArrayUniform;
		std::shared_ptr<UniformBufferArray<Engines::Graphic::Shaders::LightPositionFarPlaneArray>> m_lightPositionFarPlaneArrayUniform;
		GLuint dephMapFBO;
		std::shared_ptr<Texture> m_depthTexture;

		std::string m_shaderTemplate;
		std::vector<std::shared_ptr<OpenGLShader>> m_shaders;
		Engines::Graphic::Shaders::LightSpaceMatrixArray m_lightSpaceMatrixArray;
		Engines::Graphic::Shaders::LightPositionFarPlaneArray m_lightPositionFarPlaneArray;

		LightTypeShadow m_type;
	};
}
