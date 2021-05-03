#include "OpenGLShadowMapGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"
#include "../../../Core/Utils/TokenRepleacer.hpp"

#include "../../../Common/ShaderEnums.hpp"

GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::OpenGLShadowMapGraphicPipeline(std::shared_ptr<Texture> depthTexture, Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray, LightTypeShadow type, Engines::Graphic::Shaders::LightPositionFarPlaneArray lightPositionFarPlaneArray)
{
	m_lightSpaceMatrixArray = lightSpaceMatrixArray;
	m_lightPositionFarPlaneArray = lightPositionFarPlaneArray;
	m_type = type;
	auto vert = std::make_shared<OpenGLVertexShader>(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.vert").string()));
	m_shaders.push_back(vert);
	m_shaderTemplate = GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.geom.template").string());
	uint32_t lightCount = lightSpaceMatrixArray.data.size() > 0 ? lightSpaceMatrixArray.data.size() : 1;
	if (lightPositionFarPlaneArray.data.size() < lightSpaceMatrixArray.data.size())
	{
		lightCount = lightPositionFarPlaneArray.data.size() > 0 ? lightPositionFarPlaneArray.data.size() : 1;
	}
	auto geom = std::make_shared<OpenGLGeometryShader>(Core::Utils::tokenRepleacer(m_shaderTemplate,
		{
			{"<<PLACEHOLDER_1>>", std::to_string(lightCount)},
			{"<<PLACEHOLDER_2>>", getShaderTypePlaceholder()},
			{"<<PLACEHOLDER_3>>", getDepth()}
		}));
	m_shaders.push_back(geom);
	if (m_type == LightTypeShadow::point)
	{
		m_shaders.push_back(std::make_shared<OpenGLFragmentShader>(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.frag").string())));
	}
	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(m_shaders);

	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::LightSpaceModelMatrices>>(ShaderBinding::ShadowMap_LightSpaceModelMatrices + getOffset(), m_shaderProgram);
	m_modelMatrix = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::ModelMatrix>>(ShaderBinding::ShadowMap_ModelMatrix + getOffset(), m_shaderProgram);
	m_lightSpaceMatrixArrayUniform = std::make_shared<UniformBufferArray<Engines::Graphic::Shaders::LightSpaceMatrixArray>>(ShaderBinding::ShadowMap_LightSpaceMatrixArray + getOffset(), m_shaderProgram);

	m_lightSpaceMatrixArrayUniform->update(m_lightSpaceMatrixArray.data.data(), m_lightSpaceMatrixArray.data.size(), 0);

	if (m_type != LightTypeShadow::directional)
	{
		m_lightPositionFarPlaneArrayUniform = std::make_shared<UniformBufferArray<Engines::Graphic::Shaders::LightPositionFarPlaneArray>>(ShaderBinding::ShadowMap_LightPositionFarPlaneArray + getOffset(), m_shaderProgram);
		m_lightPositionFarPlaneArrayUniform->update(m_lightPositionFarPlaneArray.data.data(), m_lightPositionFarPlaneArray.data.size(), 0);
	}

	m_depthTexture = depthTexture;

	glGenFramebuffers(1, &dephMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, dephMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture->getTexture(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::draw()
{
	if (m_lightSpaceMatrixArray.data.size() > 0)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		m_shaderProgram->use();
		glViewport(0, 0, m_depthTexture->getWidth(), m_depthTexture->getHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, dephMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT); 

		m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
			{
				vertexBufferCollection->modelDescriptor.model = vertexBufferCollection->mesh->getModelMatrix();
				m_modelDescriptorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
				Engines::Graphic::Shaders::ModelMatrix m(vertexBufferCollection->modelDescriptor.model);
				m_modelMatrix->update(&m);
				vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
			});

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CULL_FACE);
	}
}

void GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::updateLights(Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray, Engines::Graphic::Shaders::LightPositionFarPlaneArray lightPositionFarPlaneArray)
{
	m_lightSpaceMatrixArray = lightSpaceMatrixArray;
	m_lightPositionFarPlaneArray = lightPositionFarPlaneArray;
	uint32_t lightCount = m_lightSpaceMatrixArray.data.size() > 0 ? m_lightSpaceMatrixArray.data.size() : 1;
	m_shaders[1] = std::make_shared<OpenGLGeometryShader>(Core::Utils::tokenRepleacer(m_shaderTemplate,
		{
			{"<<PLACEHOLDER_1>>", std::to_string(lightCount)},
			{"<<PLACEHOLDER_2>>", getShaderTypePlaceholder()},
			{"<<PLACEHOLDER_3>>", getDepth()}
		}));
	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(m_shaders);
	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::LightSpaceModelMatrices>>(ShaderBinding::ShadowMap_LightSpaceModelMatrices + getOffset(), m_shaderProgram);
	m_modelMatrix = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::ModelMatrix>>(ShaderBinding::ShadowMap_ModelMatrix + getOffset(), m_shaderProgram);
	m_lightSpaceMatrixArrayUniform = std::make_shared<UniformBufferArray<Engines::Graphic::Shaders::LightSpaceMatrixArray>>(ShaderBinding::ShadowMap_LightSpaceMatrixArray + getOffset(), m_shaderProgram);

	m_lightSpaceMatrixArrayUniform->update(m_lightSpaceMatrixArray.data.data(), m_lightSpaceMatrixArray.data.size(), 0);

	if (m_type != LightTypeShadow::directional)
	{
		m_lightPositionFarPlaneArrayUniform = std::make_shared<UniformBufferArray<Engines::Graphic::Shaders::LightPositionFarPlaneArray>>(ShaderBinding::ShadowMap_LightPositionFarPlaneArray + getOffset(), m_shaderProgram);
		m_lightPositionFarPlaneArrayUniform->update(m_lightPositionFarPlaneArray.data.data(), m_lightPositionFarPlaneArray.data.size(), 0);
	}
}

void GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::updateLight(Engines::Graphic::Shaders::LightSpaceMatrix lightSpaceMatrix, uint32_t index, Engines::Graphic::Shaders::LightPositionFarPlane lightPositionFarPlane)
{
	m_lightSpaceMatrixArray.data[index] = lightSpaceMatrix;
	m_lightSpaceMatrixArrayUniform->update(m_lightSpaceMatrixArray.data.data(), m_lightSpaceMatrixArray.data.size(), 0);

	if (m_lightPositionFarPlaneArray.data.size() > 0)
	{
		m_lightPositionFarPlaneArray.data[index] = lightPositionFarPlane;
		m_lightPositionFarPlaneArrayUniform->update(m_lightPositionFarPlaneArray.data.data(), m_lightPositionFarPlaneArray.data.size(), 0);
	}
}

void GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::updateLight(std::array<Engines::Graphic::Shaders::LightSpaceMatrix, 6> lightSpaceMatrices, uint32_t index, Engines::Graphic::Shaders::LightPositionFarPlane lightPositionFarPlane)
{
	uint32_t startIndex = m_type == LightTypeShadow::point ? index * 6 : index;
	for (auto& lightSpaceMatrix : lightSpaceMatrices)
	{
		m_lightSpaceMatrixArray.data[startIndex] = lightSpaceMatrix;
		++startIndex;
	}
	m_lightSpaceMatrixArrayUniform->update(m_lightSpaceMatrixArray.data.data(), m_lightSpaceMatrixArray.data.size(), 0);

	if (m_lightPositionFarPlaneArray.data.size() > 0)
	{
		m_lightPositionFarPlaneArray.data[index] = lightPositionFarPlane;
		m_lightPositionFarPlaneArrayUniform->update(m_lightPositionFarPlaneArray.data.data(), m_lightPositionFarPlaneArray.data.size(), 0);
	}
}

uint32_t GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::getOffset()
{
	if (m_type == LightTypeShadow::directional)
		return 0;
	if (m_type == LightTypeShadow::point)
		return 3;
	if (m_type == LightTypeShadow::spot)
		return 7;
}

std::string GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::getShaderTypePlaceholder()
{
	return (m_type == LightTypeShadow::directional || m_type == LightTypeShadow::spot) ? "NON_FRAG" : "FRAG";
}

std::string GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::getDepth()
{
	if (m_type == LightTypeShadow::point)
		return "6";
	return "1";
}
