#include "OpenGLSolidColorGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLSolidColorGraphicPipeline::OpenGLSolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager, std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager, std::shared_ptr<Texture> depthTexture, std::shared_ptr<Texture> spotLightShadowMaps) :
	Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>{ cameraControllerManager }
{
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("solid.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("solid.frag").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });

	m_depthTexture = depthTexture;
	m_spotLightShadowMaps = spotLightShadowMaps;
	m_renderingOptionsManager = renderingOptionsManager;
	m_eyePositionUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::Eye>>(2, m_shaderProgram);
	m_solidColorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::SolidColorModelDescriptor>>(4, m_shaderProgram);
	
	m_renderingOptionsUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::RenderingOptions>>(100, m_shaderProgram);
	m_renderingOptionsUniformBuffer->update(&m_renderingOptionsManager->renderingOptions);
	m_renderingOptionsManager->onUpdateOptions([&](Engines::Graphic::Shaders::RenderingOptions renderingOptions) {
		m_renderingOptionsUniformBuffer->update(&m_renderingOptionsManager->renderingOptions);
	});
	
	m_shaderProgram->use();
	m_diffuseOnlyIndex = glGetSubroutineIndex(m_shaderProgram->getShaderProgramId(), GL_FRAGMENT_SHADER, "BlinnPhong");
	auto id = glGetUniformLocation(m_shaderProgram->getShaderProgramId(), "shadowMap");
	glUniform1i(id, 0);
	m_depthTexture->use(0);

	auto id2 = glGetUniformLocation(m_shaderProgram->getShaderProgramId(), "spotLightShadowMap");
	glUniform1i(id2, 1);
	m_spotLightShadowMaps->use(1);
}

void GraphicEngine::OpenGL::OpenGLSolidColorGraphicPipeline::draw()
{
	m_shaderProgram->use();

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_diffuseOnlyIndex);

	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();
	auto projectionMatrix = m_cameraControllerManager->getActiveCamera()->getProjectionMatrix();
	auto eyePosition = m_cameraControllerManager->getActiveCamera()->getPosition();
	Engines::Graphic::Shaders::Eye eye{ glm::vec4(eyePosition, 1.0) };
	
	m_eyePositionUniformBuffer->update(&eye);

	m_renderingOptionsUniformBuffer->update(&m_renderingOptionsManager->renderingOptions);

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(vertexBufferCollection->modelDescriptor.modelMatrix));
		vertexBufferCollection->modelDescriptor.color = vertexBufferCollection->mesh->getMaterial().solidColor;
		m_solidColorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
		vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
	});
}
