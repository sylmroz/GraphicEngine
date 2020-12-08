#include "OpenGLSolidColorGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLSolidColorGraphicPipeline::OpenGLSolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager) :
	Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBuffer>{ cameraControllerManager }
{
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("solid.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("solid.frag").string()));

	m_shaderProgram = std::make_unique<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });

	m_cameraUniformBuffer = std::make_shared<UniformBuffer<glm::mat4>>(0);
	m_eyePositionUniformBuffer = std::make_shared<UniformBuffer<glm::vec3>>(2);
	m_ligthUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::Light>>(3);
	m_solidColorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::SolidColorModelDescriptor>>(4);
}

void GraphicEngine::OpenGL::OpenGLSolidColorGraphicPipeline::draw()
{
	m_shaderProgram->use();
	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();
	auto eyePosition = m_cameraControllerManager->getActiveCamera()->getPosition();
	Engines::Graphic::Shaders::Light light{ eyePosition, glm::vec3{ 1.0f } };
	auto cameraMatrix = m_cameraControllerManager->getActiveCamera()->getViewProjectionMatrix();
	m_cameraUniformBuffer->update(&cameraMatrix);
	m_eyePositionUniformBuffer->update(&eyePosition);
	m_ligthUniformBuffer->update(&light);

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(vertexBufferCollection->modelDescriptor.modelMatrix));
		vertexBufferCollection->modelDescriptor.color = vertexBufferCollection->mesh->getMaterial().solidColor;
		m_solidColorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
		vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
	});
}
