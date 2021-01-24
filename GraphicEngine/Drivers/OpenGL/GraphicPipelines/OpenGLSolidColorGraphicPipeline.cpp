#include "OpenGLSolidColorGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLSolidColorGraphicPipeline::OpenGLSolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager, std::shared_ptr<TextureDepth> depthTexture) :
	Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>{ cameraControllerManager }
{
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("solid.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("solid.frag").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });

	m_eyePositionUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::Eye>>(2, m_shaderProgram);
	m_solidColorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::SolidColorModelDescriptor>>(4, m_shaderProgram);
	m_shaderProgram->use();
	m_diffuseOnlyIndex = glGetSubroutineIndex(m_shaderProgram->getShaderProgramId(), GL_FRAGMENT_SHADER, "BlinnPhong");
	auto id = glGetUniformLocation(m_shaderProgram->getShaderProgramId(), "shadowMap");
	glUniform1i(id, 0);

	m_depthTexture = depthTexture;
}

void GraphicEngine::OpenGL::OpenGLSolidColorGraphicPipeline::draw()
{
	m_shaderProgram->use();

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_diffuseOnlyIndex);

	//m_depthTexture->use(0);

	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();
	auto projectionMatrix = m_cameraControllerManager->getActiveCamera()->getProjectionMatrix();
	auto eyePosition = m_cameraControllerManager->getActiveCamera()->getPosition();
	Engines::Graphic::Shaders::Eye eye{ glm::vec4(eyePosition, 1.0) };
	
	m_eyePositionUniformBuffer->update(&eye);

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(vertexBufferCollection->modelDescriptor.modelMatrix));
		vertexBufferCollection->modelDescriptor.color = vertexBufferCollection->mesh->getMaterial().solidColor;
		m_solidColorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
		vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
	});
}
