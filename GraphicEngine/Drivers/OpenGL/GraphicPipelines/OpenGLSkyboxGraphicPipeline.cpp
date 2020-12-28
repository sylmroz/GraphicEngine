#include "OpenGLSkyboxGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::OpenGL::OpenGLSkyboxGraphicPipeline::OpenGLSkyboxGraphicPipeline(const std::string& basePath):
	Engines::Graphic::SkyboxGraphicPipeline<VertexBuffer, TextureCube>{ basePath }
{
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
