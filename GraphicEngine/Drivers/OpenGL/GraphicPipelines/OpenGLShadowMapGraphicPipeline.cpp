#include "OpenGLShadowMapGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::OpenGLShadowMapGraphicPipeline(std::shared_ptr<Services::LightManager> lightManager, std::shared_ptr<TextureDepth> depthTexture)
{
	m_lightManager = lightManager;
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.frag").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });

	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::LightSpaceModelMatrices>>(10, m_shaderProgram);

	m_depthTexture = depthTexture;

	glGenFramebuffers(1, &dephMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, dephMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->getTexture(), 0);
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

		m_depthTexture->use(0);

		auto dirLight = m_lightManager->getDirectionalLight(0);

		m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
			{
				vertexBufferCollection->modelDescriptor.model = vertexBufferCollection->mesh->getModelMatrix();
				vertexBufferCollection->modelDescriptor.lightSpace = dirLight.lightSpace;
				m_modelDescriptorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
				vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
			});

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CULL_FACE);
	}
}
