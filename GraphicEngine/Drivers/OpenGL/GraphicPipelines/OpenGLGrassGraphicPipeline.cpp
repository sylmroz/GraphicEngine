#include "OpenGLGrassGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"


GraphicEngine::OpenGL::OpenGLGrassGraphicPipeline::OpenGLGrassGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager)
{
	m_cameraControllerManager = cameraControllerManager;
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("grass.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("grass.frag").string()));
	OpenGLGeometryShader geom(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("grass.geom").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, geom, frag });

	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::ModelMartices>>(27, m_shaderProgram);
	//m_materialUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::Material>>(28, m_shaderProgram);
	m_grassParametersUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::GrassParameters>>(29, m_shaderProgram);

	m_shaderProgram->use();
	/*Engines::Graphic::Shaders::Material grass;
	grass.diffuse = glm::vec4(0.07058f, 0.509803f, 0.078431f, 0.85f);
	grass.ambient = glm::vec4(0.07058f, 0.509803f, 0.078431f, 0.85f);
	grass.specular = glm::vec4(0.63137f, 0.89019f, 0.71372f, 0.85f);
	m_materialUniformBuffer->update(&grass);*/
	

	Engines::Graphic::Shaders::GrassParameters grassParameters(0.005f, 0.2f, 1000);
	m_grassParametersUniformBuffer->update(&grassParameters);
}

void GraphicEngine::OpenGL::OpenGLGrassGraphicPipeline::draw()
{
	m_shaderProgram->use();

	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
		{
			vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
			vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(vertexBufferCollection->modelDescriptor.modelMatrix));
			m_modelDescriptorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
			vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
		});
}

