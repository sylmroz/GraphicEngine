#include "OpenGLShadowMapGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"
#include "../../../Core/Utils/TokenRepleacer.hpp"

GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::OpenGLShadowMapGraphicPipeline(std::shared_ptr<Services::LightManager> lightManager, std::shared_ptr<Texture> depthTexture)
{
	m_lightManager = lightManager;
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.vert").string()));
	std::string baseShader = GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.geom.template").string());
	OpenGLGeometryShader geom(Core::Utils::tokenRepleacer(baseShader, { {"<<PLACEHOLDER_1>>", "5"} }));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, geom });

	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::LightSpaceModelMatrices>>(12, m_shaderProgram);
	m_modelMatrix = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::ModelMatrix>>(13, m_shaderProgram);
	m_lightSpaceMatrix = std::make_shared<UniformBufferArray<Engines::Graphic::Shaders::LightSpaceMatrixArray>>(14, m_shaderProgram);

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
	if (m_lightManager->getDirectionalLights().size() > 0)
	{
		glEnable(GL_CULL_FACE);
		m_shaderProgram->use();
		glViewport(0, 0, m_depthTexture->getWidth(), m_depthTexture->getHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, dephMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT); 
		glCullFace(GL_FRONT);

		//m_depthTexture->use(0);

		Engines::Graphic::Shaders::LightSpaceMatrixArray lsm;// (dirLight.lightSpace);
		auto dirLights = m_lightManager->getDirectionalLights();
		for (auto dirLight : dirLights)
		{
			lsm.data.push_back(dirLight.lightSpace);
		}
		
		
		m_lightSpaceMatrix->update(lsm.data.data(), lsm.data.size(), 0);

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
