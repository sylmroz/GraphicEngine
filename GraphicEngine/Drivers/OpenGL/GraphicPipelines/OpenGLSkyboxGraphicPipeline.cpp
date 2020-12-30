#include "OpenGLSkyboxGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLSkyboxGraphicPipeline::OpenGLSkyboxGraphicPipeline(const std::string& basePath)
{
	initialize(basePath);
	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("skybox.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("skybox.frag").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });
}

void GraphicEngine::OpenGL::OpenGLSkyboxGraphicPipeline::draw()
{
	glDepthFunc(GL_LEQUAL);
	m_shaderProgram->use();
	m_cubeVertexBuffer->bind();
	m_textureCube->use();
	m_cubeVertexBuffer->draw(GL_TRIANGLES);
	glDepthFunc(GL_LESS);
}

std::unique_ptr<GraphicEngine::OpenGL::VertexBuffer<GraphicEngine::Common::VertexP>> GraphicEngine::OpenGL::OpenGLSkyboxGraphicPipeline::produceVertexBuffer(std::vector<Common::VertexP> skyBoxVertices)
{
	auto ptr = std::make_unique<VertexBuffer<Common::VertexP>>(skyBoxVertices);
	return ptr;
}

std::shared_ptr<GraphicEngine::OpenGL::TextureCube> GraphicEngine::OpenGL::OpenGLSkyboxGraphicPipeline::produceTextureCube(std::array<std::string, 6> faces)
{
	auto ptr = std::make_shared<TextureCube>(faces);
	return ptr;
}
