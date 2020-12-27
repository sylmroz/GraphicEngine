#include "OpenGLNormalDebugGraphicPileline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLNormalDebugGraphicPipeline::OpenGLNormalDebugGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager)
{
	m_cameraControllerManager = cameraControllerManager;
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("normals.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("normals.frag").string()));
	OpenGLGeometryShader geom(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("normals.geom").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, geom, frag });

	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::ModelMartices>>(5, m_shaderProgram);
}

void GraphicEngine::OpenGL::OpenGLNormalDebugGraphicPipeline::draw()
{
	m_shaderProgram->use();

	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();
	auto projectionMatrix = m_cameraControllerManager->getActiveCamera()->getProjectionMatrix();

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(m_cameraControllerManager->getActiveCamera()->getViewMatrix() * vertexBufferCollection->modelDescriptor.modelMatrix));
		m_modelDescriptorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
		vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
	});
}
