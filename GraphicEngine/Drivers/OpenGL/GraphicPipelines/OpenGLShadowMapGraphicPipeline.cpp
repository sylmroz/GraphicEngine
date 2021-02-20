#include "OpenGLShadowMapGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"
#include "../../../Core/Utils/TokenRepleacer.hpp"

GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::OpenGLShadowMapGraphicPipeline(std::shared_ptr<Texture> depthTexture, Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray)
{
	m_lightSpaceMatrixArray = lightSpaceMatrixArray;
	auto vert = std::make_shared<OpenGLVertexShader>(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.vert").string()));
	m_shaders.push_back(vert);
	m_shaderTemplate = GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.geom.template").string());
	uint32_t lightCount = lightSpaceMatrixArray.data.size() > 0 ? lightSpaceMatrixArray.data.size() : 1;
	auto geom = std::make_shared<OpenGLGeometryShader>(Core::Utils::tokenRepleacer(m_shaderTemplate, { {"<<PLACEHOLDER_1>>", std::to_string(lightCount)} }));
	m_shaders.push_back(geom);
	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(m_shaders);

	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::LightSpaceModelMatrices>>(12, m_shaderProgram);
	m_modelMatrix = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::ModelMatrix>>(13, m_shaderProgram);
	m_lightSpaceMatrixArrayUniform = std::make_shared<UniformBufferArray<Engines::Graphic::Shaders::LightSpaceMatrixArray>>(14, m_shaderProgram);

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
		m_shaderProgram->use();
		glViewport(0, 0, m_depthTexture->getWidth(), m_depthTexture->getHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, dephMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT); 
		glCullFace(GL_FRONT);

		m_lightSpaceMatrixArrayUniform->update(m_lightSpaceMatrixArray.data.data(), m_lightSpaceMatrixArray.data.size(), 0);

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

void GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::updateLights(Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray)
{
	m_lightSpaceMatrixArray = lightSpaceMatrixArray;
	uint32_t lightCount = m_lightSpaceMatrixArray.data.size() > 0 ? m_lightSpaceMatrixArray.data.size() : 1;
	m_shaders[1] = std::make_shared<OpenGLGeometryShader>(Core::Utils::tokenRepleacer(m_shaderTemplate, { {"<<PLACEHOLDER_1>>", std::to_string(lightCount)} }));
	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(m_shaders);
	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::LightSpaceModelMatrices>>(12, m_shaderProgram);
	m_modelMatrix = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::ModelMatrix>>(13, m_shaderProgram);
	m_lightSpaceMatrixArrayUniform = std::make_shared<UniformBufferArray<Engines::Graphic::Shaders::LightSpaceMatrixArray>>(14, m_shaderProgram);
}

void GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::updateLight(Engines::Graphic::Shaders::LightSpaceMatrix lightSpaceMatrix, uint32_t index)
{
	m_lightSpaceMatrixArray.data[index] = lightSpaceMatrix;
}
