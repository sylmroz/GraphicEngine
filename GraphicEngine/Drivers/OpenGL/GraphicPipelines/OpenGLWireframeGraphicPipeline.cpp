#include "OpenGLWireframeGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLWireframeGraphicPipeline::OpenGLWireframeGraphicPipeline()
{
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("wireframe.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("wireframe.frag").string()));

	m_shaderProgram = std::make_unique<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });
	
	m_cameraUniformBuffer = std::make_shared<UniformBuffer<glm::mat4>>(0);
	m_wireframeModelDescriptorUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::WireframeModelDescriptor>>(1);
}

void GraphicEngine::OpenGL::OpenGLWireframeGraphicPipeline::draw()
{
	m_shaderProgram->use();

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		m_wireframeModelDescriptorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
		vertexBufferCollection->vertexBuffer->drawEdges(GL_LINES);
	});
}
