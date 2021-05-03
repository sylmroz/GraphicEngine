#include "OpenGLWireframeGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

#include "../../../Common/ShaderEnums.hpp"

GraphicEngine::OpenGL::OpenGLWireframeGraphicPipeline::OpenGLWireframeGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager)
{
	m_cameraControllerManager = cameraControllerManager;
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("wireframe.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("wireframe.frag").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });
	
	m_wireframeModelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::WireframeModelDescriptor>>(ShaderBinding::Wireframe_WireframeModelDescriptor, m_shaderProgram);
}

void GraphicEngine::OpenGL::OpenGLWireframeGraphicPipeline::draw()
{
	m_shaderProgram->use();

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.wireframeColor = glm::vec4(Core::changeContrast(glm::vec3(vertexBufferCollection->mesh->getMaterial().solidColor), glm::vec3(1.2f)), 1.0f);
		m_wireframeModelDescriptorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
		vertexBufferCollection->vertexBuffer->drawEdges(GL_LINES);
	});
}
