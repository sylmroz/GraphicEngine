#include "OpenGLShadowMapGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::OpenGLShadowMapGraphicPipeline(std::shared_ptr<Services::LightManager> lightManager)
{
	m_lightManager = lightManager;
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("shadowmap.frag").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });

	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::LightSpaceModelMatrices>>(10, m_shaderProgram);

	m_depthTexture = std::make_shared<TextureDepth>(1024, 1024);

	glBindFramebuffer(GL_FRAMEBUFFER, dephMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->getTexture(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicEngine::OpenGL::OpenGLShadowMapGraphicPipeline::draw()
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, dephMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	m_shaderProgram->use();

	auto dirLight = m_lightManager->getDirectionalLight(0);
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(10.0f * glm::vec3(-dirLight.direction), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.model = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.lightSpace = lightProjection * lightView;
		m_modelDescriptorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
		vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
	});

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
