#include "OpenGLSolidColorGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLSolidColorGraphicPipeline::OpenGLSolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager) :
	Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>{ cameraControllerManager }
{
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("solid.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("solid.frag").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });

	m_eyePositionUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::Eye>>(2, m_shaderProgram);
	// m_ligthUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::Light>>(3, m_shaderProgram);
	m_solidColorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::SolidColorModelDescriptor>>(4, m_shaderProgram);
}

void GraphicEngine::OpenGL::OpenGLSolidColorGraphicPipeline::draw()
{
	m_shaderProgram->use();
	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();
	auto projectionMatrix = m_cameraControllerManager->getActiveCamera()->getProjectionMatrix();
	auto eyePosition = m_cameraControllerManager->getActiveCamera()->getPosition();
	//Engines::Graphic::Shaders::Light light{ projectionMatrix * viewMatrix * glm::vec4(eyePosition, 0.0), glm::vec4{ 1.0f } };
	Engines::Graphic::Shaders::Eye eye{ glm::vec4(eyePosition, 1.0) };
	
	m_eyePositionUniformBuffer->update(&eye);
	// m_ligthUniformBuffer->update(&light);

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(vertexBufferCollection->modelDescriptor.modelMatrix));
		vertexBufferCollection->modelDescriptor.color = vertexBufferCollection->mesh->getMaterial().solidColor;
		m_solidColorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
		vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
	});
}
